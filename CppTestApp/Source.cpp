#include <windows.h>
#include <cstdio>
#include <future>

#include <GraphicsCore.h>
#include <GraphicsObject.h>
#include <DataTypesDefinition.h>

using namespace std;

HRESULT InitWnd(HINSTANCE hInst, int nCmdShow, HWND* hWnd);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);

void main()
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	HWND hWnd;
	InitWnd(hInst, SW_SHOWNORMAL, &hWnd);

	//printf("ModuleHandle : %x \n", GraphicsCore::hInst);
	//printf("WindowHandle : %x \n", GraphicsCore::hWnd);

	auto graphicsThread = thread(Initialize, hWnd);
	//Initialize(CurrentGraphicsCore.hWnd);

	printf("Initialized GraphicsCore \n");

	GraphicsObjectDescription goDesc;
	ZeroMemory(&goDesc, sizeof(goDesc));
	goDesc.vertexType = VertexType_ShapeAndValue;
	goDesc.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	int id = GraphicsObject_Create(goDesc);
	printf("A Object was generated. ID : %d \n", id);

	VertexData_ShapeAndValue vertices[]
	{
		{Vector3(0.0,0.0,0.0), 0},
		{Vector3(0.0,1.0,0.0), 1},
		{Vector3(1.0,0.0,0.0), 3}
	};

	GraphicsObject_SetVertices(id, vertices, 3);

	id = GraphicsObject_Create(goDesc);
	printf("A Object was generated. ID : %d \n", id);

	VertexData_ShapeAndValue vertices2[]
	{
		{ Vector3(0.0,0.0,0.0), 0 },
		{ Vector3(0.0,-1.0,0.0), 1 },
		{ Vector3(-1.0,0.0,0.0), 5 }
	};

	GraphicsObject_SetVertices(id, vertices2, 3);

	MSG msg = { 0 };
	bool debuging = true;
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (debuging)
			{
				//char command[1];
				//scanf("%1c", command);

				//if (command[0] == 'e')
				//{
				//	debuging = false;
				//	printf("デバッグモードを終了しました。");
				//}
			}
		}
	}

	graphicsThread.join();

	return;
}

HRESULT InitWnd(HINSTANCE hInst, int nCmdShow, HWND* hWnd)
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

	// ウィンドウサイズの設定
	RECT rc = { 0, 0, 800, 600 }; // 4 : 3
								  //RECT rc = { 0, 0, 800, 450 }; // 16 : 9
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウの生成
	*hWnd = CreateWindow(
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
	ShowWindow(*hWnd, nCmdShow);

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