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
	//static HINSTANCE				hInst;					// �C���X�^���X�n���h��
	static HWND						hWnd;					// �E�B���h�E�n���h��
	static D3D_FEATURE_LEVEL		featureLevel;			// 
	static ID3D11Device*			pDevice;				// D3D11�f�o�C�X�C���^�t�F�[�X
	static ID3D11DeviceContext*		pDeviceContext;			// D3D11�f�o�C�X�R���e�L�X�g�C���^�t�F�[�X
	static IDXGISwapChain*			pSwapChain;				// �X���b�v�`�F�C��
	static ID3D11RenderTargetView*	pBackBuffer;			// �o�b�N�o�b�t�@
};

extern "C"
{
	DLL_API int Initialize(HWND handle);
}