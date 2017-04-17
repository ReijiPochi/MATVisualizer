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

	// デバイス等を初期化し、GraphicsCoreの機能を使用できるようにします。
	// HWND handle : 描画結果を出力するウインドウのハンドル
	DLL_API int Initialize(HWND handle)
	{
		HINSTANCE hInst = GetModuleHandle(NULL);

		CurrentGraphicsCore.hWnd = handle;

		InitializeDevice();
		E_FAIL
		// メインループ
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
				// レンダーターゲットビューをクリア
				float clearColor[4] = { 0.5f, 0.2f, 0.2f, 1.0f };
				CurrentGraphicsCore.pDeviceContext->ClearRenderTargetView(CurrentGraphicsCore.pBackBuffer, clearColor);

				// フリップ処理
				CurrentGraphicsCore.pSwapChain->Present(1, 0);
			}
		}

		return 1;
	}

	
	void InitializeDevice()
	{
		// ウィンドウサイズを取得
		RECT rc;
		GetClientRect(CurrentGraphicsCore.hWnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

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


		// バックバッファーを取得
		ID3D11Texture2D* pBackBuffer = NULL;
		CurrentGraphicsCore.pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// レンダーターゲットビューを生成
		CurrentGraphicsCore.pDevice->CreateRenderTargetView(pBackBuffer, NULL, &CurrentGraphicsCore.pBackBuffer);
		pBackBuffer -> Release();
		pBackBuffer = NULL;
		
		// 出力マネージャにレンダーターゲットビューを設定
		CurrentGraphicsCore.pDeviceContext->OMSetRenderTargets(1, &CurrentGraphicsCore.pBackBuffer, NULL);

		// ビューポートの設定
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
	//	// 拡張ウィンドウクラスの登録
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

	//	// インスタンスハンドルを設定
	//	graphicsCore.hInst = hInst;

	//	// ウィンドウサイズの設定
	//	RECT rc = { 0, 0, 800, 600 }; // 4 : 3
	//								  //RECT rc = { 0, 0, 800, 450 }; // 16 : 9
	//	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	//	// ウィンドウの生成
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

	//	// ウィンドウの表示設定
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
