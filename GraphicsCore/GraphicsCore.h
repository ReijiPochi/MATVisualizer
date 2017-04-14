#pragma once

#include <windows.h>
#include <cstdio>
#include <d3d11_4.h>
#include <d3dcompiler.h>

extern "C"
{
#define CPPDLL_API __declspec(dllexport)

	struct GraphicsCore
	{
		HINSTANCE				hInst;					// インスタンスハンドル
		HWND					hWnd;					// ウィンドウハンドル
		D3D_DRIVER_TYPE			driverType;				// ドライバータイプ
		D3D_FEATURE_LEVEL		featureLevel;			// 
		ID3D11Device*			pDevice;				// D3D11デバイスインタフェース
		ID3D11DeviceContext*	pDeviceContext;			// D3D11デバイスコンテキストインタフェース
		IDXGISwapChain*			pSwapChain;				// スワップチェイン
		ID3D11RenderTargetView* pRenderTargetView;		// レンダーターゲットビュー
		ID3D11VertexShader*		pVS;					// 頂点シェーダ
		ID3D11GeometryShader*   pGS;					// ジオメトリシェーダ
		ID3D11PixelShader*		pPS;					// ピクセルシェーダ
		ID3D11InputLayout*		pVertexLayout;			// 入力レイアウト
		ID3D11Buffer*			pVertexBuffer;			// 頂点バッファ
	};

	CPPDLL_API int Initialize(HWND handle);
}