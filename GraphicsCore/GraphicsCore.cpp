#include "GraphicsCore.h"
#include "GraphicsObject.h"

#include <windows.h>
#include <cstdio>
#include <d3d11_4.h>
#include <d3dcompiler.h>

#pragma comment( lib, "d3d11.lib" )

bool GraphicsCore::Ready = false;
HWND GraphicsCore::hWnd = NULL;
GraphicsObject* GraphicsCore::objects[GRAPHICS_OBJECT_MAX_COUNT];
D3D_FEATURE_LEVEL GraphicsCore::featureLevel = (D3D_FEATURE_LEVEL)0;
ID3D11Device* GraphicsCore::pDevice = NULL;
ID3D11DeviceContext* GraphicsCore::pDeviceContext = NULL;
IDXGISwapChain* GraphicsCore::pSwapChain = NULL;
ID3D11RenderTargetView* GraphicsCore::pBackBuffer = NULL;

extern "C"
{
	void InitializeDevice();


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

			// シェーダを設定して描画
			GraphicsCore::pDeviceContext->VSSetShader(GraphicsCore::objects[index]->pVertexShader, NULL, 0);
			GraphicsCore::pDeviceContext->GSSetShader(GraphicsCore::objects[index]->pGeometryShader, NULL, 0);
			GraphicsCore::pDeviceContext->PSSetShader(GraphicsCore::objects[index]->pPixelShader, NULL, 0);

			GraphicsCore::pDeviceContext->Draw(3, 0);
		}
	}


	float r = 0.0;
	// デバイス等を初期化し、GraphicsCoreの機能を使用できるようにします。
	// HWND handle : 描画結果を出力するウインドウのハンドル
	DLL_API int Initialize(HWND handle)
	{
		GraphicsCore::hWnd = handle;
		
		InitializeDevice();
		
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
				float clearColor[4] = { r, 0.2f, 0.2f, 1.0f };
				GraphicsCore::pDeviceContext->ClearRenderTargetView(GraphicsCore::pBackBuffer, clearColor);

				GraphicsCore::Render();

				// フリップ処理
				GraphicsCore::pSwapChain->Present(1, 0);

				r += 0.001;
			}
		}

		return 1;
	}

	
	void InitializeDevice()
	{
		// ウィンドウサイズを取得
		RECT rc;
		GetClientRect(GraphicsCore::hWnd, &rc);
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
		sd.OutputWindow = GraphicsCore::hWnd;
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
			&GraphicsCore::pSwapChain,
			&GraphicsCore::pDevice,
			&GraphicsCore::featureLevel,
			&GraphicsCore::pDeviceContext);


		// バックバッファーを取得
		ID3D11Texture2D* pBackBuffer = NULL;
		GraphicsCore::pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// レンダーターゲットビューを生成
		GraphicsCore::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &GraphicsCore::pBackBuffer);
		pBackBuffer -> Release();
		pBackBuffer = NULL;
		
		// 出力マネージャにレンダーターゲットビューを設定
		GraphicsCore::pDeviceContext->OMSetRenderTargets(1, &GraphicsCore::pBackBuffer, NULL);

		// ビューポートの設定
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		GraphicsCore::pDeviceContext->RSSetViewports(1, &vp);

		GraphicsCore::Ready = true;
	}
}
