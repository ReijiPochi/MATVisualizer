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
		HINSTANCE				hInst;					// �C���X�^���X�n���h��
		HWND					hWnd;					// �E�B���h�E�n���h��
		D3D_DRIVER_TYPE			driverType;				// �h���C�o�[�^�C�v
		D3D_FEATURE_LEVEL		featureLevel;			// 
		ID3D11Device*			pDevice;				// D3D11�f�o�C�X�C���^�t�F�[�X
		ID3D11DeviceContext*	pDeviceContext;			// D3D11�f�o�C�X�R���e�L�X�g�C���^�t�F�[�X
		IDXGISwapChain*			pSwapChain;				// �X���b�v�`�F�C��
		ID3D11RenderTargetView* pRenderTargetView;		// �����_�[�^�[�Q�b�g�r���[
		ID3D11VertexShader*		pVS;					// ���_�V�F�[�_
		ID3D11GeometryShader*   pGS;					// �W�I���g���V�F�[�_
		ID3D11PixelShader*		pPS;					// �s�N�Z���V�F�[�_
		ID3D11InputLayout*		pVertexLayout;			// ���̓��C�A�E�g
		ID3D11Buffer*			pVertexBuffer;			// ���_�o�b�t�@
	};

	CPPDLL_API int Initialize(HWND handle);
}