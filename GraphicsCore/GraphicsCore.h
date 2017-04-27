#pragma once

#include "DLL.h"
#include "GraphicsObject.h"
#include "Texture.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include <windows.h>
#include <cstdio>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>


#define GRAPHICS_OBJECT_MAX_COUNT (128)


struct GlobalCBufferData
{
	DirectX::XMMATRIX camera;
};

class GraphicsCore
{
public:
	static bool						Ready;
	static HWND						hWnd;					// ウィンドウハンドル
	static float					viewWidth;
	static float					viewHeight;
	static Camera*					pCamera;
	static GlobalCBufferData		globalCBufferData;
	static ConstantBuffer*			pGlobalCBuffer;
	static std::vector<GraphicsObject*> pRenderingList;
	static D3D_FEATURE_LEVEL		featureLevel;			// 
	static ID3D11Device*			pDevice;				// D3D11デバイスインタフェース
	static ID3D11DeviceContext*		pDeviceContext;			// D3D11デバイスコンテキストインタフェース
	static IDXGISwapChain*			pSwapChain;				// スワップチェイン
	static ID3D11RenderTargetView*	pBackBuffer;			// バックバッファ
	static Texture2D*				pDepthStencilTexture;
	static ID3D11DepthStencilView*	pDepthStencilView;
	static void Render();
	static void Release();
};

void ReleaseIUnknown(IUnknown** target);

extern "C"
{
	DLL_API int GraphicsCore_Initialize(HWND handle);
	DLL_API ID3D11Device* GraphicsCore_GetDevice();
	DLL_API void GraphicsCore_AddToRenderingList(GraphicsObject* object);
	DLL_API void GraphicsCore_Finalize();
	DLL_API GraphicsObject* TEST(GraphicsObjectDescription* desc);
}