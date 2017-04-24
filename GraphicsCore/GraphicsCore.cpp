#include "GraphicsCore.h"
#include "GraphicsObject.h"
#include "ConstantBuffer.h"
#include "ReleasableObject.h"

#include <windows.h>
#include <cstdio>
#include <d3d11_4.h>
#include <d3dcompiler.h>

#pragma comment( lib, "d3d11.lib" )

bool GraphicsCore::Ready = false;
HWND GraphicsCore::hWnd = NULL;
float GraphicsCore::viewHeight = 1.0f;
float GraphicsCore::viewWidth = 1.0f;
Camera* GraphicsCore::camera = new Camera;
GlobalCBufferData GraphicsCore::globalCBufferData = { };
ConstantBuffer* GraphicsCore::globalCBuffer = NULL;
GraphicsObject* GraphicsCore::objects[GRAPHICS_OBJECT_MAX_COUNT];
D3D_FEATURE_LEVEL GraphicsCore::featureLevel = (D3D_FEATURE_LEVEL)0;
ID3D11Device* GraphicsCore::pDevice = NULL;
ID3D11DeviceContext* GraphicsCore::pDeviceContext = NULL;
IDXGISwapChain* GraphicsCore::pSwapChain = NULL;
ID3D11RenderTargetView* GraphicsCore::pBackBuffer = NULL;
Texture2D* GraphicsCore::depthStencil = NULL;
ID3D11DepthStencilView* GraphicsCore::depthStencilView = NULL;

bool finalize = false;

void InitializeDevice();

void ReleaseIUnknown(IUnknown* target)
{
	if (target != nullptr)
	{
		target->Release();
		target = nullptr;
	}
}

void GraphicsCore::Render()
{
	for (int index = 0; index < GRAPHICS_OBJECT_MAX_COUNT; index++)
	{
		if (GraphicsCore::objects[index] == NULL)
			continue;

		if (GraphicsCore::objects[index]->isLocking)
			continue;

		// 入力アセンブラに入力レイアウトを設定
		GraphicsCore::pDeviceContext->IASetInputLayout(GraphicsCore::objects[index]->pInputLayout);

		// 入力アセンブラに頂点バッファを設定
		UINT stride = GetSizeOfVertexType(GraphicsCore::objects[index]->vertexType);
		UINT offset = 0;
		GraphicsCore::pDeviceContext->IASetVertexBuffers(0, 1, &GraphicsCore::objects[index]->pVertexBuffer, &stride, &offset);

		// プリミティブの種類を設定
		GraphicsCore::pDeviceContext->IASetPrimitiveTopology(GraphicsCore::objects[index]->primitiveTopology);

		// シェーダを設定
		GraphicsCore::pDeviceContext->VSSetShader(GraphicsCore::objects[index]->pVertexShader, NULL, 0);
		GraphicsCore::pDeviceContext->GSSetShader(GraphicsCore::objects[index]->pGeometryShader, NULL, 0);
		GraphicsCore::pDeviceContext->PSSetShader(GraphicsCore::objects[index]->pPixelShader, NULL, 0);

		GraphicsCore::globalCBuffer->Set();

		GraphicsCore::pDeviceContext->Draw(3, 0);
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

	ReleaseIUnknown(GraphicsCore::pBackBuffer);
	ReleaseIUnknown(GraphicsCore::depthStencilView);
	ReleaseIUnknown(GraphicsCore::pSwapChain);
	ReleaseIUnknown(GraphicsCore::pDeviceContext);
	ReleaseIUnknown(GraphicsCore::pDevice);
}

void InitializeDevice()
{
	if (!DirectX::XMVerifyCPUSupport()) {
		MessageBoxA(NULL, "DirectX::XMVerifyCPUSupport() returns FALSE!", NULL, MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// ウィンドウサイズを取得
	RECT rc;
	GetClientRect(GraphicsCore::hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	GraphicsCore::viewWidth = width;
	GraphicsCore::viewHeight = height;

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
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
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
	GraphicsCore::pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// レンダーターゲットビューを生成
	GraphicsCore::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &GraphicsCore::pBackBuffer);
	pBackBuffer->Release();
	pBackBuffer = NULL;

	Texture2DDescription depthTexDesc;
	depthTexDesc.width = width;
	depthTexDesc.height = height;
	depthTexDesc.textureFormat = DXGI_FORMAT_R32_TYPELESS;
	depthTexDesc.resourceFormat = DXGI_FORMAT_R32_FLOAT;
	depthTexDesc.sample = 4;
	depthTexDesc.bindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	GraphicsCore::depthStencil = Texture2D::Create(depthTexDesc);

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

	GraphicsCore::pDevice->CreateDepthStencilView(GraphicsCore::depthStencil->texture, &depthDesc, &GraphicsCore::depthStencilView);

	// 出力マネージャにレンダーターゲットビューを設定
	GraphicsCore::pDeviceContext->OMSetRenderTargets(1, &GraphicsCore::pBackBuffer, GraphicsCore::depthStencilView);

	D3D11_RASTERIZER_DESC rDesc;
	ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.DepthClipEnable = true;
	rDesc.CullMode = D3D11_CULL_NONE;

	ID3D11RasterizerState *rasterizerState;

	GraphicsCore::pDevice->CreateRasterizerState(&rDesc, &rasterizerState);
	GraphicsCore::pDeviceContext->RSSetState(rasterizerState);

	// ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	GraphicsCore::pDeviceContext->RSSetViewports(1, &vp);

	ConstantBufferDescription cbufferDesc;
	cbufferDesc.byteWidth = sizeof(GlobalCBufferData);

	GraphicsCore::globalCBuffer = ConstantBuffer::Create(cbufferDesc);

	GraphicsCore::Ready = true;
}

extern "C"
{

	float r = 0.0;

	// デバイス等を初期化し、GraphicsCoreの機能を使用できるようにします。
	// HWND handle : 描画結果を出力するウインドウのハンドル
	DLL_API int Initialize(HWND handle)
	{
		GraphicsCore::hWnd = handle;
		
		InitializeDevice();
		
		// メインループ
		while (!finalize)
		{
			GraphicsCore::camera->eye = DirectX::XMVectorSet(2.0f * sin(r * 5.0f), 0.0f, -2.0f * cos(r * 5.0f), 0.0f);
			GraphicsCore::camera->SetMatrix();
			GraphicsCore::globalCBufferData.camera = GraphicsCore::camera->cameraMatrix;

			GraphicsCore::globalCBuffer->Update(&GraphicsCore::globalCBufferData);

			// レンダーターゲットビューをクリア
			float clearColor[4] = { r, 0.2f, 0.2f, 1.0f };
			GraphicsCore::pDeviceContext->ClearRenderTargetView(GraphicsCore::pBackBuffer, clearColor);

			GraphicsCore::pDeviceContext->ClearDepthStencilView(GraphicsCore::depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			GraphicsCore::Render();

			// フリップ処理
			GraphicsCore::pSwapChain->Present(1, 0);

			r += 0.001;
		}

		GraphicsCore::Release();

		return 1;
	}

	DLL_API void Finalize()
	{
		finalize = true;
	}
}
