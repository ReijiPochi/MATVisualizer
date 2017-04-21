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
	static HWND						hWnd;					// �E�B���h�E�n���h��
	static GraphicsObject *objects[];
	static D3D_FEATURE_LEVEL		featureLevel;			// 
	static ID3D11Device*			pDevice;				// D3D11�f�o�C�X�C���^�t�F�[�X
	static ID3D11DeviceContext*		pDeviceContext;			// D3D11�f�o�C�X�R���e�L�X�g�C���^�t�F�[�X
	static IDXGISwapChain*			pSwapChain;				// �X���b�v�`�F�C��
	static ID3D11RenderTargetView*	pBackBuffer;			// �o�b�N�o�b�t�@
	static void Render();
	static void Release();
};

void ReleaseIUnknown(IUnknown* target);

extern "C"
{
	DLL_API int Initialize(HWND handle);
	DLL_API void Finalize();
}