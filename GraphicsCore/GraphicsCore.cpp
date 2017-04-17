#include "GraphicsCore.h"

#include <windows.h>
#include <cstdio>
#include <d3d11_4.h>
#include <d3dcompiler.h>

#pragma comment( lib, "d3d11.lib" )

extern "C"
{
	//HRESULT InitWnd(HINSTANCE hInst, int nCmdShow);
	//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);

	void InitializeDevice();

	// �f�o�C�X�������������AGraphicsCore�̋@�\���g�p�ł���悤�ɂ��܂��B
	// HWND handle : �`�挋�ʂ��o�͂���E�C���h�E�̃n���h��
	DLL_API int Initialize(HWND handle)
	{
		HINSTANCE hInst = GetModuleHandle(NULL);

		CurrentGraphicsCore.hWnd = handle;

		InitializeDevice();
		E_FAIL
		// ���C�����[�v
		MSG msg = { 0 };
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				// �����_�[�^�[�Q�b�g�r���[���N���A
				float clearColor[4] = { 0.5f, 0.2f, 0.2f, 1.0f };
				CurrentGraphicsCore.pDeviceContext->ClearRenderTargetView(CurrentGraphicsCore.pBackBuffer, clearColor);

				// �t���b�v����
				CurrentGraphicsCore.pSwapChain->Present(1, 0);
			}
		}

		return 1;
	}

	
	void InitializeDevice()
	{
		// �E�B���h�E�T�C�Y���擾
		RECT rc;
		GetClientRect(CurrentGraphicsCore.hWnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

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
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = CurrentGraphicsCore.hWnd;
		sd.SampleDesc.Count = 1;
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
			&CurrentGraphicsCore.pSwapChain,
			&CurrentGraphicsCore.pDevice,
			&CurrentGraphicsCore.featureLevel,
			&CurrentGraphicsCore.pDeviceContext);


		// �o�b�N�o�b�t�@�[���擾
		ID3D11Texture2D* pBackBuffer = NULL;
		CurrentGraphicsCore.pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// �����_�[�^�[�Q�b�g�r���[�𐶐�
		CurrentGraphicsCore.pDevice->CreateRenderTargetView(pBackBuffer, NULL, &CurrentGraphicsCore.pBackBuffer);
		pBackBuffer -> Release();
		pBackBuffer = NULL;
		
		// �o�̓}�l�[�W���Ƀ����_�[�^�[�Q�b�g�r���[��ݒ�
		CurrentGraphicsCore.pDeviceContext->OMSetRenderTargets(1, &CurrentGraphicsCore.pBackBuffer, NULL);

		// �r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		CurrentGraphicsCore.pDeviceContext->RSSetViewports(1, &vp);
	}

	//HRESULT InitWnd(HINSTANCE hInst, int nCmdShow)
	//{
	//	// �g���E�B���h�E�N���X�̓o�^
	//	WNDCLASSEX wc;
	//	wc.cbSize = sizeof(WNDCLASSEX);
	//	wc.style = CS_HREDRAW | CS_VREDRAW;
	//	wc.lpfnWndProc = WndProc;
	//	wc.cbClsExtra = 0;
	//	wc.cbWndExtra = 0;
	//	wc.hInstance = hInst;
	//	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//	wc.lpszMenuName = NULL;
	//	wc.lpszClassName = TEXT("AsDemoAppDx11");
	//	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	//	if (!RegisterClassEx(&wc))
	//	{
	//		return E_FAIL;
	//	}

	//	// �C���X�^���X�n���h����ݒ�
	//	graphicsCore.hInst = hInst;

	//	// �E�B���h�E�T�C�Y�̐ݒ�
	//	RECT rc = { 0, 0, 800, 600 }; // 4 : 3
	//								  //RECT rc = { 0, 0, 800, 450 }; // 16 : 9
	//	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	//	// �E�B���h�E�̐���
	//	graphicsCore.hWnd = CreateWindow(
	//		TEXT("AsDemoAppDx11"),
	//		TEXT("Graphics Window"),
	//		WS_OVERLAPPEDWINDOW,
	//		CW_USEDEFAULT,
	//		CW_USEDEFAULT,
	//		rc.right - rc.left,
	//		rc.bottom - rc.top,
	//		NULL,
	//		NULL,
	//		hInst,
	//		NULL);

	//	// �E�B���h�E�̕\���ݒ�
	//	ShowWindow(graphicsCore.hWnd, nCmdShow);

	//	return S_OK;
	//}

	//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp)
	//{
	//	PAINTSTRUCT ps;
	//	HDC hdc;

	//	switch (uMsg)
	//	{
	//	case WM_PAINT:
	//	{
	//		hdc = BeginPaint(hWnd, &ps);
	//		EndPaint(hWnd, &ps);
	//	}
	//	break;

	//	case WM_DESTROY:
	//	{
	//		PostQuitMessage(0);
	//	}
	//	break;

	//	default:
	//	{
	//		/* DO_NOTHING */
	//	}
	//	break;
	//	}

	//	return DefWindowProc(hWnd, uMsg, wp, lp);
	//}
}
