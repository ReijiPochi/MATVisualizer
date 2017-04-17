#pragma once

#include "DLL.h"
#include <windows.h>
#include <cstdio>
#include <d3d11_4.h>
#include <d3dcompiler.h>

struct GraphicsCore
{
	HINSTANCE				hInst;					// �C���X�^���X�n���h��
	HWND					hWnd;					// �E�B���h�E�n���h��
	D3D_FEATURE_LEVEL		featureLevel;			// 
	ID3D11Device*			pDevice;				// D3D11�f�o�C�X�C���^�t�F�[�X
	ID3D11DeviceContext*	pDeviceContext;			// D3D11�f�o�C�X�R���e�L�X�g�C���^�t�F�[�X
	IDXGISwapChain*			pSwapChain;				// �X���b�v�`�F�C��
	ID3D11RenderTargetView* pBackBuffer;			// �o�b�N�o�b�t�@
													//ID3D11VertexShader*		pVS;					// ���_�V�F�[�_
													//ID3D11GeometryShader*   pGS;					// �W�I���g���V�F�[�_
													//ID3D11PixelShader*		pPS;					// �s�N�Z���V�F�[�_
													//ID3D11InputLayout*		pVertexLayout;			// ���̓��C�A�E�g
													//ID3D11Buffer*			pVertexBuffer;			// ���_�o�b�t�@
};

extern "C"
{
	static GraphicsCore CurrentGraphicsCore;

	DLL_API int Initialize(HWND handle);
}