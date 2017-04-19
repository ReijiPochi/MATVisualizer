#pragma once

#include "DLL.h"
#include <windows.h>
#include <cstdio>
#include <d3d11_4.h>
#include <d3dcompiler.h>



class GraphicsCore
{
public:
	static bool Ready;
	//static HINSTANCE				hInst;					// インスタンスハンドル
	static HWND						hWnd;					// ウィンドウハンドル
	static D3D_FEATURE_LEVEL		featureLevel;			// 
	static ID3D11Device*			pDevice;				// D3D11デバイスインタフェース
	static ID3D11DeviceContext*		pDeviceContext;			// D3D11デバイスコンテキストインタフェース
	static IDXGISwapChain*			pSwapChain;				// スワップチェイン
	static ID3D11RenderTargetView*	pBackBuffer;			// バックバッファ
};

extern "C"
{
	DLL_API int Initialize(HWND handle);
}