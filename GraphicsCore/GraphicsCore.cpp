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
	for (std::vector<GraphicsObject*>::iterator itr = GraphicsCore::pRenderingList.begin(); itr != GraphicsCore::pRenderingList.end(); ++itr)
	{
		if ((*itr)->isLocking)
			continue;

		// ���̓A�Z���u���ɓ��̓��C�A�E�g��ݒ�
		GraphicsCore::pDeviceContext->IASetInputLayout((*itr)->pInputLayout);

		// ���̓A�Z���u���ɒ��_�o�b�t�@��ݒ�
		UINT stride = GetSizeOfVertexType((*itr)->vertexType);
		UINT offset = 0;
		GraphicsCore::pDeviceContext->IASetVertexBuffers(0, 1, &(*itr)->pVertexBuffer, &stride, &offset);


		// �v���~�e�B�u�̎�ނ�ݒ�
		GraphicsCore::pDeviceContext->IASetPrimitiveTopology((*itr)->primitiveTopology);

		// �V�F�[�_��ݒ�
		GraphicsCore::pDeviceContext->VSSetShader((*itr)->pVertexShader, NULL, 0);
		GraphicsCore::pDeviceContext->GSSetShader((*itr)->pGeometryShader, NULL, 0);
		GraphicsCore::pDeviceContext->PSSetShader((*itr)->pPixelShader, NULL, 0);

		GraphicsCore::pGlobalCBuffer->Set();
		(*itr)->DownloadBuffers();

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
	ReleaseIUnknown(GraphicsCore::pDepthStencilView);
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

	// �E�B���h�E�T�C�Y���擾
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

	// �@�\���x��
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
	};
	UINT numFeatureLevels = sizeof(featureLevels) / sizeof(featureLevels[0]);

	// �X���b�v�`�F�C���̐ݒ�
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


	// �o�b�N�o�b�t�@�[���擾
	ID3D11Texture2D* pBackBuffer = NULL;
	{
		GraphicsCore::pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// �����_�[�^�[�Q�b�g�r���[�𐶐�
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

	// �o�̓}�l�[�W���Ƀ����_�[�^�[�Q�b�g�r���[��ݒ�
	GraphicsCore::pDeviceContext->OMSetRenderTargets(1, &GraphicsCore::pBackBuffer, GraphicsCore::pDepthStencilView);

	D3D11_RASTERIZER_DESC rDesc;
	ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.DepthClipEnable = true;
	rDesc.CullMode = D3D11_CULL_NONE;

	ID3D11RasterizerState *rasterizerState;
	{
		GraphicsCore::pDevice->CreateRasterizerState(&rDesc, &rasterizerState);
		GraphicsCore::pDeviceContext->RSSetState(rasterizerState);
	}
	rasterizerState->Release();
	rasterizerState = nullptr;

	// �r���[�|�[�g�̐ݒ�
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

	GraphicsCore::pGlobalCBuffer = ConstantBuffer::Create(cbufferDesc);

	GraphicsCore::Ready = true;
}

extern "C"
{

	float r = 0.0;

	// �f�o�C�X�������������AGraphicsCore�̋@�\���g�p�ł���悤�ɂ��܂��B
	// HWND handle : �`�挋�ʂ��o�͂���E�C���h�E�̃n���h��
	DLL_API int GraphicsCore_Initialize(HWND handle)
	{
		GraphicsCore::hWnd = handle;
		
		InitializeDevice();
		
		// ���C�����[�v
		while (!finalize)
		{
			GraphicsCore::pCamera->eye = DirectX::XMVectorSet(2.0f * sin(r * 5.0f), 1.0f, -2.0f * cos(r * 5.0f), 0.0f);
			GraphicsCore::pCamera->target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			GraphicsCore::pCamera->up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

			GraphicsCore::pCamera->SetMatrix();
			GraphicsCore::globalCBufferData.camera = GraphicsCore::pCamera->cameraMatrix;

			GraphicsCore::pGlobalCBuffer->Update(&GraphicsCore::globalCBufferData);

			// �����_�[�^�[�Q�b�g�r���[���N���A
			float clearColor[4] = { r, 0.2f, 0.2f, 1.0f };
			GraphicsCore::pDeviceContext->ClearRenderTargetView(GraphicsCore::pBackBuffer, clearColor);

			GraphicsCore::pDeviceContext->ClearDepthStencilView(GraphicsCore::pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			GraphicsCore::Render();

			// �t���b�v����
			GraphicsCore::pSwapChain->Present(1, 0);

			r += 0.001;
		}

		GraphicsCore::Release();

		return 1;
	}

	DLL_API void GraphicsCore_AddToRenderingList(GraphicsObject* object)
	{
		GraphicsCore::pRenderingList.push_back(object);
	}

	DLL_API void GraphicsCore_Finalize()
	{
		finalize = true;
	}
}
