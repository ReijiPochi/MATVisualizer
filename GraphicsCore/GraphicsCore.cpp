#include "GraphicsCore.h"

#include <windows.h>
#include <cstdio>
#include <d3d11_4.h>
#include <d3dcompiler.h>

#pragma comment( lib, "d3d11.lib" )


extern "C"
{
	GraphicsCore gDemoApp;

	//HRESULT InitWnd(HINSTANCE hInst, int nCmdShow);
	//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);

	CPPDLL_API int Initialize(HWND handle)
	{
		HINSTANCE hInst = GetModuleHandle(NULL);

		if (handle)
		{
			return 114;
		}
		else
		{
			return 514;
		}

		// �E�B���h�E������
		//InitWnd(hInst, SW_SHOWNORMAL);
	}

	//HRESULT InitWnd(HINSTANCE hInst, int nCmdShow)
	//{
	//	// �g���E�B���h�E�N���X�̓o�^
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

	//	// �C���X�^���X�n���h����ݒ�
	//	gDemoApp.hInst = hInst;

	//	// �E�B���h�E�T�C�Y�̐ݒ�
	//	RECT rc = { 0, 0, 800, 600 }; // 4 : 3
	//								  //RECT rc = { 0, 0, 800, 450 }; // 16 : 9
	//	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	//	// �E�B���h�E�̐���
	//	gDemoApp.hWnd = CreateWindow(
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
	//	if (!gDemoApp.hWnd)
	//	{
	//		return E_FAIL;
	//	}

	//	// �E�B���h�E�̕\���ݒ�
	//	ShowWindow(gDemoApp.hWnd, nCmdShow);

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
