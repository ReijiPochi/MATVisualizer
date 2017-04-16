#include <windows.h>
#include <cstdio>
#include <GraphicsCore.h>

HRESULT InitWnd(HINSTANCE hInst, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);

void main()
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	InitWnd(hInst, SW_SHOWNORMAL);

	printf("ModuleHandle : %x \n", CurrentGraphicsCore.hInst);
	printf("WindowHandle : %x \n", CurrentGraphicsCore.hWnd);

	Initialize(CurrentGraphicsCore.hWnd);

	return;
}

HRESULT InitWnd(HINSTANCE hInst, int nCmdShow)
{
	// 拡張ウィンドウクラスの登録
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("TestApp");
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wc))
	{
		return E_FAIL;
	}

	// インスタンスハンドルを設定
	CurrentGraphicsCore.hInst = hInst;

	// ウィンドウサイズの設定
	RECT rc = { 0, 0, 800, 600 }; // 4 : 3
								  //RECT rc = { 0, 0, 800, 450 }; // 16 : 9
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウの生成
	CurrentGraphicsCore.hWnd = CreateWindow(
		TEXT("TestApp"),
		TEXT("Graphics Window"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInst,
		NULL);

	// ウィンドウの表示設定
	ShowWindow(CurrentGraphicsCore.hWnd, nCmdShow);

	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg)
	{
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;

	default:
	{
		/* DO_NOTHING */
	}
	break;
	}

	return DefWindowProc(hWnd, uMsg, wp, lp);
}