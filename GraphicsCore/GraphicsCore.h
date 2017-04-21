#pragma once

#include "DLL.h"
#include "GraphicsObject.h"
#include <windows.h>
#include <cstdio>
#include <d3d11_4.h>
#include <d3dcompiler.h>


#define GRAPHICS_OBJECT_MAX_COUNT (128)

class GraphicsCore
{
public:
	static bool Ready;
	static HWND						hWnd;					// ウィンドウハンドル
	static GraphicsObject *objects[];
	static D3D_FEATURE_LEVEL		featureLevel;			// 
	static ID3D11Device*			pDevice;				// D3D11デバイスインタフェース
	static ID3D11DeviceContext*		pDeviceContext;			// D3D11デバイスコンテキストインタフェース
	static IDXGISwapChain*			pSwapChain;				// スワップチェイン
	static ID3D11RenderTargetView*	pBackBuffer;			// バックバッファ
	static void Render();
	static void Release();
};

void ReleaseIUnknown(IUnknown* target);

extern "C"
{
	DLL_API int Initialize(HWND handle);
	DLL_API void Finalize();
}