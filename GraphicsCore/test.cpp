//----------------------------------------------------------------------
// File : main.cpp
// Desc : Application Main Entry Point
// Copyright(c) Project Asura. All right reserved.
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// History
//----------------------------------------------------------------------
// <---> 2010/07/16 [Pocol] 新規作成
// <End>


//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <cstdio>


//----------------------------------------------------------------------
// Linker
//----------------------------------------------------------------------
#pragma comment( lib, "d3d11.lib" )

#if defined(DEBUG) || defined(_DEBUG)
	#pragma comment( lib, "d3dx11d.lib" )
#else
	#pragma comment( lib, "d3dx11.lib" )
#endif


//----------------------------------------------------------------------
// Data Definitions
//----------------------------------------------------------------------
struct CustomVertex
{
	XMFLOAT3 position;	// 頂点座標
};

struct GraphicsCore
{
	HINSTANCE				hInst;					// インスタンスハンドル
	HWND					hWnd;					// ウィンドウハンドル
	D3D_DRIVER_TYPE			driverType;				// ドライバータイプ
	D3D_FEATURE_LEVEL		featureLevel;			// 
	ID3D11Device*			pDevice;				// D3D11デバイスインタフェース
	ID3D11DeviceContext*	pDeviceContext;			// D3D11デバイスコンテキストインタフェース
	IDXGISwapChain*			pSwapChain;				// スワップチェイン
	ID3D11RenderTargetView* pRenderTargetView;		// レンダーターゲットビュー
	ID3D11VertexShader*		pVS;					// 頂点シェーダ
	ID3D11GeometryShader*   pGS;					// ジオメトリシェーダ
	ID3D11PixelShader*		pPS;					// ピクセルシェーダ
	ID3D11InputLayout*		pVertexLayout;			// 入力レイアウト
	ID3D11Buffer*			pVertexBuffer;			// 頂点バッファ
};

//----------------------------------------------------------------------
// Forward Declarations
//----------------------------------------------------------------------
HRESULT InitWnd( HINSTANCE hInst, int nCmdShow );
HRESULT InitDevice();
void	CleanupDevice();
HRESULT CompileShaderFromFile( TCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp );
void	Render();


//----------------------------------------------------------------------
// Global Variables
//----------------------------------------------------------------------
GraphicsCore gDemoApp;


//----------------------------------------------------------------------
//! @brief アプリケーションメインエントリーポイント
//! @param [in] argc 引数の数
//! @param [in] argv 引数の値
//! @return システムへ値を返却する
//----------------------------------------------------------------------
int main( int argc, char** argv )
{
	HINSTANCE hInst = GetModuleHandle( NULL );

	// ウィンドウ初期化
	if ( FAILED( InitWnd( hInst, SW_SHOWNORMAL ) ) )
	{
		printf( "InitWnd() Failed.\n" );
		return 0;
	}

	// デバイス初期化
	if ( FAILED( InitDevice() ) )
	{
		printf( "InitDevice() Failed.\n" );
		return 0;
	}

	// メインループ
	MSG msg = { 0 };
	while( WM_QUIT != msg.message )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			// 描画
			Render();
		}
	}

	// デバイスのクリーンアップ
	CleanupDevice();

	return (int)msg.wParam;
}

//----------------------------------------------------------------------
//! @brief ウィンドウの生成
//! @param [in] hInst インスタンスハンドル
//! @param [in] nCmdShow 
//----------------------------------------------------------------------
HRESULT InitWnd( HINSTANCE hInst, int nCmdShow )
{
	// 拡張ウィンドウクラスの登録
	WNDCLASSEX wc;
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT( "AsDemoAppDx11" );
	wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
	if ( !RegisterClassEx( &wc ) )
	{
		return E_FAIL;
	}

	// インスタンスハンドルを設定
	gDemoApp.hInst = hInst;

	// ウィンドウサイズの設定
	RECT rc = { 0, 0, 800, 600 }; // 4 : 3
	//RECT rc = { 0, 0, 800, 450 }; // 16 : 9
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	// ウィンドウの生成
	gDemoApp.hWnd = CreateWindow(
		TEXT( "AsDemoAppDx11" ),
		TEXT( "DirectX11 Sample Program" ),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInst,
		NULL );
	if ( !gDemoApp.hWnd )
	{
		return E_FAIL;
	}

	// ウィンドウの表示設定
	ShowWindow( gDemoApp.hWnd, nCmdShow );

	return S_OK;
}

//----------------------------------------------------------------------
//! @brief ウィンドウプロシージャ
//! @param [in] hWnd ウィンドウハンドル
//! @param [in] uMsg メッセージ
//! @param [in] wp
//! @param [in] lp
//----------------------------------------------------------------------
LRESULT CALLBACK WndProc
(
	HWND	hWnd,
	UINT	uMsg,
	WPARAM	wp,
	LPARAM	lp
)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch( uMsg )
	{
		case WM_PAINT :
			{
				hdc = BeginPaint( hWnd, &ps );
				EndPaint( hWnd, &ps );
			}
			break;

		case WM_DESTROY :
			{
				PostQuitMessage( 0 );
			}
			break;

		default :
			{
				/* DO_NOTHING */
			}
			break;
	}

	return DefWindowProc( hWnd, uMsg, wp, lp );
}

//-----------------------------------------------------------------------
//! @brief ファイルからシェーダをコンパイル
//! @param [in] szFileName ファイル名
//! @param [in] szEntryPoint エントリーポイント名
//! @param [in] szShaderMode シェーダプロファイル
//! @param [out] ppBlobOut コンパイルされたバイトコード
//-----------------------------------------------------------------------
HRESULT CompileShaderFromFile
(
	TCHAR* szFileName,
	LPCSTR szEntryPoint,
	LPCSTR szShaderModel,
	ID3DBlob** ppBlobOut
)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;

	// シェーダをファイルからコンパイル
	hr = D3DX11CompileFromFile( 
		szFileName,
		NULL,
		NULL,
		szEntryPoint,
		szShaderModel,
		dwShaderFlags,
		0,
		NULL,
		ppBlobOut,
		&pErrorBlob,
		NULL );
	if ( FAILED( hr ) )
	{
		if ( pErrorBlob != NULL )
		{
			// エラーメッセージ出力
			OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
		}
	}

	if ( pErrorBlob )
	{
		pErrorBlob->Release();
		pErrorBlob = NULL;
	}

	return hr;
}

//-----------------------------------------------------------------------
//! @brief デバイスの初期化
//-----------------------------------------------------------------------
HRESULT InitDevice()
{
	HRESULT hr = S_OK;

	// ウィンドウサイズを取得
	RECT rc;
	GetClientRect( gDemoApp.hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// ドライバータイプ
	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

	// 機能レベル
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = sizeof( featureLevels ) / sizeof( featureLevels[0] );

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = gDemoApp.hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	// デバイスとスワップチェインを作成する
	for( UINT idx = 0; idx < numDriverTypes; idx++ )
	{
		gDemoApp.driverType = driverTypes[ idx ];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			gDemoApp.driverType,
			NULL,
			createDeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&sd,
			&gDemoApp.pSwapChain,
			&gDemoApp.pDevice,
			&gDemoApp.featureLevel,
			&gDemoApp.pDeviceContext );

		if ( SUCCEEDED( hr ) )
		{
			// 成功したらループ脱出
			break;
		}
	}
	if ( FAILED( hr ) )
	{
		// 作成に失敗したら終了
		return hr;
	}

	// バックバッファ取得
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = gDemoApp.pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer );
	if ( FAILED( hr ) )
	{
		return hr;
	}

	// レンダーターゲットビューを生成
	hr = gDemoApp.pDevice->CreateRenderTargetView( pBackBuffer, NULL, &gDemoApp.pRenderTargetView );
	pBackBuffer->Release();
	pBackBuffer = NULL;
	if ( FAILED( hr ) )
	{
		return hr;
	}

	// 出力マネージャにレンダーターゲットビューを設定
	gDemoApp.pDeviceContext->OMSetRenderTargets( 1, &gDemoApp.pRenderTargetView, NULL ); 

	// ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gDemoApp.pDeviceContext->RSSetViewports( 1, &vp );

	// 頂点シェーダをコンパイル
	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile( TEXT("Resources/Effects/Sample.fx"), "VSFunc", "vs_4_0", &pVSBlob );
	if ( FAILED( hr ) )
	{
		MessageBox( NULL,
			TEXT( "he FX file cannot be compiled.  Please run this executable from the directory that contains the FX file." ),
			TEXT( "Shader Compile Error" ),
			MB_OK | MB_ICONERROR );
		return hr;
	}

	// 頂点シェーダ生成
	hr = gDemoApp.pDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &gDemoApp.pVS );
	if ( FAILED( hr ) )
	{
		pVSBlob->Release();
		pVSBlob = NULL;
		return hr;
	}

	// 入力レイアウトの定義
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	// 入力レイアウトを生成
	hr = gDemoApp.pDevice->CreateInputLayout(
		layout,
		numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&gDemoApp.pVertexLayout );
	pVSBlob->Release();
	pVSBlob = NULL;
	if ( FAILED( hr ) )
	{
		return hr;
	}

	// 入力アセンブラに入力レイアウトを設定
	gDemoApp.pDeviceContext->IASetInputLayout( gDemoApp.pVertexLayout );

	// ジオメトリシェーダコンパイル
	ID3DBlob* pGSBlob = NULL;
	hr = CompileShaderFromFile( TEXT("Resources/Effects/Sample.fx"), "GSFunc", "gs_4_0", &pGSBlob );
	if ( FAILED( hr ) )
	{
		MessageBox(
			NULL,
			TEXT( "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file." ),
			TEXT( "Shader Compile Error" ),
			MB_OK | MB_ICONERROR );
		return hr;
	}

	// ジオメトリシェーダを生成
	hr = gDemoApp.pDevice->CreateGeometryShader(
		pGSBlob->GetBufferPointer(),
		pGSBlob->GetBufferSize(),
		NULL,
		&gDemoApp.pGS );
	pGSBlob->Release();
	pGSBlob = NULL;
	if ( FAILED( hr ) )
	{
		return hr;
	}

	// ピクセルシェーダコンパイル
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile( TEXT("Resources/Effects/Sample.fx"), "PSFunc", "ps_4_0", &pPSBlob );
	if ( FAILED( hr ) )
	{
		MessageBox(
			NULL,
			TEXT( "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file." ),
			TEXT( "Shader Compile Error" ),
			MB_OK | MB_ICONERROR );
		return hr;
	}

	// ピクセルシェーダ生成
	hr = gDemoApp.pDevice->CreatePixelShader(
		pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(),
		NULL,
		&gDemoApp.pPS );
	pPSBlob->Release();
	pPSBlob = NULL;
	if ( FAILED( hr ) )
	{
		return hr;
	}

	// 頂点の定義
	CustomVertex vertices[] = {
		XMFLOAT3( 0.0f, 0.5f, 0.5f ),
		XMFLOAT3( 0.5f, -0.5f, 0.5f ),
		XMFLOAT3( -0.5f, -0.5f, 0.5f ),
	};

	// 頂点バッファの設定
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( CustomVertex ) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	// サブリソースの設定
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory( &initData, sizeof(initData ) );
	initData.pSysMem = vertices;

	// 頂点バッファ生成
	hr = gDemoApp.pDevice->CreateBuffer( &bd, &initData, &gDemoApp.pVertexBuffer );
	if ( FAILED( hr ) )
	{
		return hr;
	}

	// 入力アセンブラに頂点バッファを設定
	UINT stride = sizeof( CustomVertex );
	UINT offset = 0;
	gDemoApp.pDeviceContext->IASetVertexBuffers( 0, 1, &gDemoApp.pVertexBuffer, &stride, &offset );

	// プリミティブの種類を設定
	gDemoApp.pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	return S_OK;
}

//-----------------------------------------------------------------------
//! @brief デバイスのクリーンアップ
//-----------------------------------------------------------------------
void CleanupDevice()
{
	if ( gDemoApp.pDeviceContext )
	{
		gDemoApp.pDeviceContext->ClearState();
	}
	if ( gDemoApp.pVertexBuffer )
	{
		gDemoApp.pVertexBuffer->Release();
		gDemoApp.pVertexBuffer = NULL;
	}
	if ( gDemoApp.pVertexLayout )
	{
		gDemoApp.pVertexLayout->Release();
		gDemoApp.pVertexLayout = NULL;
	}
	if ( gDemoApp.pVS )
	{
		gDemoApp.pVS->Release();
		gDemoApp.pVS = NULL;
	}
	if ( gDemoApp.pPS )
	{
		gDemoApp.pPS->Release();
		gDemoApp.pPS = NULL;
	}
	if ( gDemoApp.pRenderTargetView )
	{
		gDemoApp.pRenderTargetView->Release();
		gDemoApp.pRenderTargetView = NULL;
	}
	if ( gDemoApp.pSwapChain )
	{
		gDemoApp.pSwapChain->Release();
		gDemoApp.pSwapChain = NULL;
	}
	if ( gDemoApp.pDeviceContext )
	{
		gDemoApp.pDeviceContext->Release();
		gDemoApp.pDeviceContext = NULL;
	}
	if ( gDemoApp.pDevice )
	{
		gDemoApp.pDevice->Release();
		gDemoApp.pDevice = NULL;
	}
}

//-----------------------------------------------------------------------
//! @brief フレーム描画
//-----------------------------------------------------------------------
void Render()
{
	// レンダーターゲットビューをクリア
	float clearColor[ 4 ] = { 0.3f, 0.3f, 1.0f, 1.0f };
	gDemoApp.pDeviceContext->ClearRenderTargetView( gDemoApp.pRenderTargetView, clearColor );

	// シェーダを設定して描画
	gDemoApp.pDeviceContext->VSSetShader( gDemoApp.pVS, NULL, 0 );
	gDemoApp.pDeviceContext->GSSetShader( gDemoApp.pGS, NULL, 0 );
	gDemoApp.pDeviceContext->PSSetShader( gDemoApp.pPS, NULL, 0 );
	gDemoApp.pDeviceContext->Draw( 3, 0 );

	// フリップ処理
	gDemoApp.pSwapChain->Present( 0, 0 );		
}