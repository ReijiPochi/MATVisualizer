#include "GraphicsCore.h"
#include "GraphicsObject.h"
#include "ConstantBuffer.h"
#include "ReleasableObject.h"

#include <windows.h>
#include <cstdio>
#include <d3d11_4.h>
#include <dxgi1_5.h>
#include <d3dcompiler.h>

#pragma comment( lib, "d3d11.lib" )

bool GraphicsCore::Ready = false;
HWND GraphicsCore::hWnd = NULL;
float GraphicsCore::viewHeight = 1.0f;
float GraphicsCore::viewWidth = 1.0f;
Camera* GraphicsCore::pCamera = new Camera;
GlobalCBufferData GraphicsCore::globalCBufferData = { };
ConstantBuffer* GraphicsCore::pGlobalCBuffer = NULL;
std::vector<GraphicsObject*> GraphicsCore::pRenderingList;
D3D_FEATURE_LEVEL GraphicsCore::featureLevel = (D3D_FEATURE_LEVEL)0;
ID3D11Device* GraphicsCore::pDevice = NULL;
ID3D11DeviceContext* GraphicsCore::pDeviceContext = NULL;
IDXGISwapChain* GraphicsCore::pSwapChain = NULL;
ID3D11RenderTargetView* GraphicsCore::pBackBuffer = NULL;
Texture2D* GraphicsCore::pDepthStencilTexture = NULL;
ID3D11DepthStencilView* GraphicsCore::pDepthStencilView = NULL;

bool resizing = false, renderingListChanging = false;
bool rendering = false;
bool finalize = false;

void InitializeDevice(GraphicsCoreDescription desc);

void ReleaseIUnknown(IUnknown** target)
{
	if (*target != nullptr)
	{
		(*target)->Release();
		*target = nullptr;
	}
}

void GraphicsCore::Render()
{
	while (renderingListChanging)
	{
		rendering = false;
	}

	rendering = true;

	for (std::vector<GraphicsObject*>::iterator itr = GraphicsCore::pRenderingList.begin(); itr != GraphicsCore::pRenderingList.end(); ++itr)
	{
		if ((*itr)->isLocking)
			continue;

		// 入力アセンブラに入力レイアウトを設定
		if ((*itr)->description.inputLayout != nullptr)
			GraphicsCore::pDeviceContext->IASetInputLayout((*itr)->description.inputLayout);
		else
			continue;

		// 入力アセンブラに頂点バッファを設定
		UINT stride = GetSizeOfVertexType((*itr)->vertexType);
		UINT offset = 0;
		if ((*itr)->pVertexBuffer != nullptr)
			GraphicsCore::pDeviceContext->IASetVertexBuffers(0, 1, &(*itr)->pVertexBuffer, &stride, &offset);
		else
			continue;


		// プリミティブの種類を設定
		GraphicsCore::pDeviceContext->IASetPrimitiveTopology((*itr)->primitiveTopology);

		// シェーダを設定
		if ((*itr)->description.vs != nullptr)
			GraphicsCore::pDeviceContext->VSSetShader((*itr)->description.vs, NULL, 0);
		else
			continue;

		if ((*itr)->description.gs != nullptr)
			GraphicsCore::pDeviceContext->GSSetShader((*itr)->description.gs, NULL, 0);
		else
			GraphicsCore::pDeviceContext->GSSetShader(nullptr, NULL, 0);

		if ((*itr)->description.ps != nullptr)
			GraphicsCore::pDeviceContext->PSSetShader((*itr)->description.ps, NULL, 0);
		else
			continue;

		(*itr)->DownloadBuffers();

		if ((*itr)->numIndices != 0)
		{
			GraphicsCore::pDeviceContext->IASetIndexBuffer((*itr)->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			GraphicsCore::pDeviceContext->DrawIndexed((*itr)->numIndices, 0, 0);
		}
		else
		{
			GraphicsCore::pDeviceContext->Draw((*itr)->numVertices, 0);
		}
	}
}

void GraphicsCore::Release()
{
	ReleasableObject::ReleaseAll();

	if (GraphicsCore::pDeviceContext != nullptr)
	{
		GraphicsCore::pDeviceContext->ClearState();
		GraphicsCore::pDeviceContext->Flush();
	}

	ReleaseIUnknown((IUnknown**)&GraphicsCore::pBackBuffer);
	ReleaseIUnknown((IUnknown**)&GraphicsCore::pDepthStencilView);
	ReleaseIUnknown((IUnknown**)&GraphicsCore::pSwapChain);
	ReleaseIUnknown((IUnknown**)&GraphicsCore::pDeviceContext);
	ReleaseIUnknown((IUnknown**)&GraphicsCore::pDevice);
}

void InitializeDevice(GraphicsCoreDescription desc)
{
	if (!DirectX::XMVerifyCPUSupport()) {
		MessageBoxA(NULL, "DirectX::XMVerifyCPUSupport() returns FALSE!", NULL, MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	GraphicsCore::viewWidth = desc.width;
	GraphicsCore::viewHeight = desc.height;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 機能レベル
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
	};
	UINT numFeatureLevels = sizeof(featureLevels) / sizeof(featureLevels[0]);

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = desc.width;
	sd.BufferDesc.Height = desc.height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = GraphicsCore::hWnd;
	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&sd,
		&GraphicsCore::pSwapChain,
		&GraphicsCore::pDevice,
		&GraphicsCore::featureLevel,
		&GraphicsCore::pDeviceContext);


	// バックバッファーを取得
	ID3D11Texture2D* pBackBuffer = NULL;
	{
		GraphicsCore::pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// レンダーターゲットビューを生成
		GraphicsCore::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &GraphicsCore::pBackBuffer);
	}
	pBackBuffer->Release();
	pBackBuffer = NULL;

	Texture2DDescription depthTexDesc;
	depthTexDesc.width = desc.width;
	depthTexDesc.height = desc.height;
	depthTexDesc.textureFormat = DXGI_FORMAT_R32_TYPELESS;
	depthTexDesc.resourceFormat = DXGI_FORMAT_R32_FLOAT;
	depthTexDesc.sample = 4;
	depthTexDesc.bindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	GraphicsCore::pDepthStencilTexture = Texture2D::Create(depthTexDesc);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	if (depthTexDesc.sample == 0)
	{
		depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthDesc.Texture2D.MipSlice = 0;
	}
	else
	{
		depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}

	GraphicsCore::pDevice->CreateDepthStencilView(GraphicsCore::pDepthStencilTexture->texture, &depthDesc, &GraphicsCore::pDepthStencilView);

	// 出力マネージャにレンダーターゲットビューを設定
	GraphicsCore::pDeviceContext->OMSetRenderTargets(1, &GraphicsCore::pBackBuffer, GraphicsCore::pDepthStencilView);

	D3D11_RASTERIZER_DESC rDesc;
	ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.DepthClipEnable = true;
	rDesc.CullMode = D3D11_CULL_BACK;
	rDesc.AntialiasedLineEnable = true;
	rDesc.MultisampleEnable = true;

	ID3D11RasterizerState *rasterizerState;
	{
		GraphicsCore::pDevice->CreateRasterizerState(&rDesc, &rasterizerState);
		GraphicsCore::pDeviceContext->RSSetState(rasterizerState);
	}
	rasterizerState->Release();
	rasterizerState = nullptr;

	// ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)desc.width;
	vp.Height = (FLOAT)desc.height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	GraphicsCore::pDeviceContext->RSSetViewports(1, &vp);

	ConstantBufferDescription cbufferDesc;
	cbufferDesc.byteWidth = sizeof(GlobalCBufferData);

	GraphicsCore::pGlobalCBuffer = ConstantBuffer::Create(cbufferDesc);

	GraphicsCore::Ready = true;
}

extern "C"
{
	// デバイス等を初期化し、GraphicsCoreの機能を使用できるようにします。
	// HWND handle : 描画結果を出力するウインドウのハンドル
	DLL_API int GraphicsCore_Initialize(GraphicsCoreDescription desc)
	{
		GraphicsCore::hWnd = desc.handle;
		
		InitializeDevice(desc);
		
		// メインループ
		while (!finalize)
		{
			while (resizing)
			{
				rendering = false;
			}

			rendering = true;

			GraphicsCore::globalCBufferData.camera = GraphicsCore::pCamera->cameraMatrix;

			GraphicsCore::pGlobalCBuffer->Update(&GraphicsCore::globalCBufferData);

			GraphicsCore::pGlobalCBuffer->Set();


			// レンダーターゲットビューをクリア
			float clearColor[4] = { 0.15f, 0.15f, 0.15f, 1.0f };
			GraphicsCore::pDeviceContext->ClearRenderTargetView(GraphicsCore::pBackBuffer, clearColor);

			GraphicsCore::pDeviceContext->ClearDepthStencilView(GraphicsCore::pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			GraphicsCore::Render();

			// フリップ処理
			GraphicsCore::pSwapChain->Present(1, 0);
		}

		GraphicsCore::Release();

		return 1;
	}

	DLL_API ID3D11Device* GraphicsCore_GetDevice()
	{
		return GraphicsCore::pDevice;
	}

	DLL_API void GraphicsCore_AddToRenderingList(GraphicsObject* object)
	{
		renderingListChanging = true;

		while (rendering);

		GraphicsCore::pRenderingList.push_back(object);

		renderingListChanging = false;
	}

	DLL_API void GraphicsCore_SetCamera(Camera* camera)
	{
		GraphicsCore::pCamera = camera;
	}

	DLL_API HRESULT GraphicsCore_Resize(int width, int height)
	{
		resizing = true;

		while (rendering);


		ReleaseIUnknown((IUnknown**)&GraphicsCore::pBackBuffer);
		ReleaseIUnknown((IUnknown**)&GraphicsCore::pDepthStencilView);
		GraphicsCore::pDepthStencilTexture->Release();

		HRESULT result = GraphicsCore::pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		// バックバッファーを取得
		ID3D11Texture2D* pBackBuffer = NULL;
		{
			GraphicsCore::pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

			// レンダーターゲットビューを生成
			GraphicsCore::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &GraphicsCore::pBackBuffer);
		}
		pBackBuffer->Release();
		pBackBuffer = NULL;

		Texture2DDescription depthTexDesc;
		depthTexDesc.width = width;
		depthTexDesc.height = height;
		depthTexDesc.textureFormat = DXGI_FORMAT_R32_TYPELESS;
		depthTexDesc.resourceFormat = DXGI_FORMAT_R32_FLOAT;
		depthTexDesc.sample = 4;
		depthTexDesc.bindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		GraphicsCore::pDepthStencilTexture = Texture2D::Create(depthTexDesc);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
		ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
		if (depthTexDesc.sample == 0)
		{
			depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthDesc.Texture2D.MipSlice = 0;
		}
		else
		{
			depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}

		GraphicsCore::pDevice->CreateDepthStencilView(GraphicsCore::pDepthStencilTexture->texture, &depthDesc, &GraphicsCore::pDepthStencilView);

		// 出力マネージャにレンダーターゲットビューを設定
		GraphicsCore::pDeviceContext->OMSetRenderTargets(1, &GraphicsCore::pBackBuffer, GraphicsCore::pDepthStencilView);


		// ビューポートの設定
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		GraphicsCore::pDeviceContext->RSSetViewports(1, &vp);

		GraphicsCore::viewWidth = width;
		GraphicsCore::viewHeight = height;

		resizing = false;

		return result;
	}

	DLL_API void GraphicsCore_Finalize()
	{
		finalize = true;
	}

	DLL_API GraphicsObject* TEST(GraphicsObjectDescription* desc)
	{
		//return GraphicsObject::Create(desc);
		return nullptr;
	}
}
