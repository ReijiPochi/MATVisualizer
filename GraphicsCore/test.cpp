//----------------------------------------------------------------------
// File : main.cpp
// Desc : Application Main Entry Point
// Copyright(c) Project Asura. All right reserved.
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// History
//----------------------------------------------------------------------
// <---> 2010/07/16 [Pocol] �V�K�쐬
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
	XMFLOAT3 position;	// ���_���W
};

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
//! @brief �A�v���P�[�V�������C���G���g���[�|�C���g
//! @param [in] argc �����̐�
//! @param [in] argv �����̒l
//! @return �V�X�e���֒l��ԋp����
//----------------------------------------------------------------------
int main( int argc, char** argv )
{
	HINSTANCE hInst = GetModuleHandle( NULL );

	// �E�B���h�E������
	if ( FAILED( InitWnd( hInst, SW_SHOWNORMAL ) ) )
	{
		printf( "InitWnd() Failed.\n" );
		return 0;
	}

	// �f�o�C�X������
	if ( FAILED( InitDevice() ) )
	{
		printf( "InitDevice() Failed.\n" );
		return 0;
	}

	// ���C�����[�v
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
			// �`��
			Render();
		}
	}

	// �f�o�C�X�̃N���[���A�b�v
	CleanupDevice();

	return (int)msg.wParam;
}

//----------------------------------------------------------------------
//! @brief �E�B���h�E�̐���
//! @param [in] hInst �C���X�^���X�n���h��
//! @param [in] nCmdShow 
//----------------------------------------------------------------------
HRESULT InitWnd( HINSTANCE hInst, int nCmdShow )
{
	// �g���E�B���h�E�N���X�̓o�^
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

	// �C���X�^���X�n���h����ݒ�
	gDemoApp.hInst = hInst;

	// �E�B���h�E�T�C�Y�̐ݒ�
	RECT rc = { 0, 0, 800, 600 }; // 4 : 3
	//RECT rc = { 0, 0, 800, 450 }; // 16 : 9
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	// �E�B���h�E�̐���
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

	// �E�B���h�E�̕\���ݒ�
	ShowWindow( gDemoApp.hWnd, nCmdShow );

	return S_OK;
}

//----------------------------------------------------------------------
//! @brief �E�B���h�E�v���V�[�W��
//! @param [in] hWnd �E�B���h�E�n���h��
//! @param [in] uMsg ���b�Z�[�W
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
//! @brief �t�@�C������V�F�[�_���R���p�C��
//! @param [in] szFileName �t�@�C����
//! @param [in] szEntryPoint �G���g���[�|�C���g��
//! @param [in] szShaderMode �V�F�[�_�v���t�@�C��
//! @param [out] ppBlobOut �R���p�C�����ꂽ�o�C�g�R�[�h
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

	// �V�F�[�_���t�@�C������R���p�C��
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
			// �G���[���b�Z�[�W�o��
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
//! @brief �f�o�C�X�̏�����
//-----------------------------------------------------------------------
HRESULT InitDevice()
{
	HRESULT hr = S_OK;

	// �E�B���h�E�T�C�Y���擾
	RECT rc;
	GetClientRect( gDemoApp.hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// �h���C�o�[�^�C�v
	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

	// �@�\���x��
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = sizeof( featureLevels ) / sizeof( featureLevels[0] );

	// �X���b�v�`�F�C���̐ݒ�
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

	// �f�o�C�X�ƃX���b�v�`�F�C�����쐬����
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
			// ���������烋�[�v�E�o
			break;
		}
	}
	if ( FAILED( hr ) )
	{
		// �쐬�Ɏ��s������I��
		return hr;
	}

	// �o�b�N�o�b�t�@�擾
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = gDemoApp.pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer );
	if ( FAILED( hr ) )
	{
		return hr;
	}

	// �����_�[�^�[�Q�b�g�r���[�𐶐�
	hr = gDemoApp.pDevice->CreateRenderTargetView( pBackBuffer, NULL, &gDemoApp.pRenderTargetView );
	pBackBuffer->Release();
	pBackBuffer = NULL;
	if ( FAILED( hr ) )
	{
		return hr;
	}

	// �o�̓}�l�[�W���Ƀ����_�[�^�[�Q�b�g�r���[��ݒ�
	gDemoApp.pDeviceContext->OMSetRenderTargets( 1, &gDemoApp.pRenderTargetView, NULL ); 

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gDemoApp.pDeviceContext->RSSetViewports( 1, &vp );

	// ���_�V�F�[�_���R���p�C��
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

	// ���_�V�F�[�_����
	hr = gDemoApp.pDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &gDemoApp.pVS );
	if ( FAILED( hr ) )
	{
		pVSBlob->Release();
		pVSBlob = NULL;
		return hr;
	}

	// ���̓��C�A�E�g�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	// ���̓��C�A�E�g�𐶐�
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

	// ���̓A�Z���u���ɓ��̓��C�A�E�g��ݒ�
	gDemoApp.pDeviceContext->IASetInputLayout( gDemoApp.pVertexLayout );

	// �W�I���g���V�F�[�_�R���p�C��
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

	// �W�I���g���V�F�[�_�𐶐�
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

	// �s�N�Z���V�F�[�_�R���p�C��
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

	// �s�N�Z���V�F�[�_����
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

	// ���_�̒�`
	CustomVertex vertices[] = {
		XMFLOAT3( 0.0f, 0.5f, 0.5f ),
		XMFLOAT3( 0.5f, -0.5f, 0.5f ),
		XMFLOAT3( -0.5f, -0.5f, 0.5f ),
	};

	// ���_�o�b�t�@�̐ݒ�
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( CustomVertex ) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	// �T�u���\�[�X�̐ݒ�
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory( &initData, sizeof(initData ) );
	initData.pSysMem = vertices;

	// ���_�o�b�t�@����
	hr = gDemoApp.pDevice->CreateBuffer( &bd, &initData, &gDemoApp.pVertexBuffer );
	if ( FAILED( hr ) )
	{
		return hr;
	}

	// ���̓A�Z���u���ɒ��_�o�b�t�@��ݒ�
	UINT stride = sizeof( CustomVertex );
	UINT offset = 0;
	gDemoApp.pDeviceContext->IASetVertexBuffers( 0, 1, &gDemoApp.pVertexBuffer, &stride, &offset );

	// �v���~�e�B�u�̎�ނ�ݒ�
	gDemoApp.pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	return S_OK;
}

//-----------------------------------------------------------------------
//! @brief �f�o�C�X�̃N���[���A�b�v
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
//! @brief �t���[���`��
//-----------------------------------------------------------------------
void Render()
{
	// �����_�[�^�[�Q�b�g�r���[���N���A
	float clearColor[ 4 ] = { 0.3f, 0.3f, 1.0f, 1.0f };
	gDemoApp.pDeviceContext->ClearRenderTargetView( gDemoApp.pRenderTargetView, clearColor );

	// �V�F�[�_��ݒ肵�ĕ`��
	gDemoApp.pDeviceContext->VSSetShader( gDemoApp.pVS, NULL, 0 );
	gDemoApp.pDeviceContext->GSSetShader( gDemoApp.pGS, NULL, 0 );
	gDemoApp.pDeviceContext->PSSetShader( gDemoApp.pPS, NULL, 0 );
	gDemoApp.pDeviceContext->Draw( 3, 0 );

	// �t���b�v����
	gDemoApp.pSwapChain->Present( 0, 0 );		
}