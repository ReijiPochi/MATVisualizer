#include "Shader.h"
#include "GraphicsCore.h"

DLL_API HRESULT Shader_GenerateVertexShaderAndInputLayout(LPCSTR fileName, LPCSTR entryPoint, GraphicsObjectDescription* desc, ID3D11VertexShader** ppVS, ID3D11InputLayout** ppinputLayout)
{
	while (!GraphicsCore::Ready);

	HRESULT result = S_OK;

	ID3DBlob* pVSBlob = NULL;
	result = Shader_CompileShaderFromFile(fileName, entryPoint, "vs_5_0", &pVSBlob);
	if (FAILED(result))
	{
		if (pVSBlob != NULL)
		{
			pVSBlob->Release();
			pVSBlob = NULL;
		}
		return result;
	}

	result = GraphicsCore::pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, ppVS);
	if (FAILED(result) && pVSBlob != NULL)
	{
		if (pVSBlob != NULL)
		{
			pVSBlob->Release();
			pVSBlob = NULL;
		}
		return result;
	}

	D3D11_INPUT_ELEMENT_DESC *layout;

	switch ((*desc).vertexType)
	{
	case VertexType_Shape:
		layout = INPUT_ELEMENTS_SHAPE;
		break;

	case VertexType_ShapeAndValue:
		layout = INPUT_ELEMENTS_SHAPE_AND_VALUE;
		break;

	default:
		break;
	}

	UINT numElements = GetElementCountOfVertexType((*desc).vertexType);

	// 入力レイアウトを生成
	result = GraphicsCore::pDevice->CreateInputLayout(
		layout,
		numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		ppinputLayout);

	pVSBlob->Release();
	pVSBlob = NULL;

	return result;
}

DLL_API HRESULT Shader_GenerateGeometryShader(LPCSTR fileName, LPCSTR entryPoint, ID3D11GeometryShader** ppGS)
{
	while (!GraphicsCore::Ready);

	HRESULT result;

	ID3DBlob* pGSBlob = NULL;
	result = Shader_CompileShaderFromFile(fileName, entryPoint, "gs_5_0", &pGSBlob);
	if (FAILED(result))
	{
		if (pGSBlob != NULL)
		{
			pGSBlob->Release();
			pGSBlob = NULL;
		}
		return result;
	}

	result = GraphicsCore::pDevice->CreateGeometryShader(pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), NULL, ppGS);

	pGSBlob->Release();
	pGSBlob = NULL;

	return result;
}

DLL_API HRESULT Shader_GeneratePixelShader(LPCSTR fileName, LPCSTR entryPoint, ID3D11PixelShader** ppPS)
{
	HRESULT result;

	ID3DBlob* pPSBlob = NULL;
	result = Shader_CompileShaderFromFile(fileName, entryPoint, "ps_5_0", &pPSBlob);
	if (FAILED(result))
	{
		if (pPSBlob != NULL)
		{
			pPSBlob->Release();
			pPSBlob = NULL;
		}
		return result;
	}

	result = GraphicsCore::pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, ppPS);

	pPSBlob->Release();
	pPSBlob = NULL;

	return result;
}

HRESULT Shader_CompileShaderFromFile(LPCSTR pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3DBlob** ppBlob)
{
	while (!GraphicsCore::Ready);

	HRESULT hr = S_OK;

	UINT ShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	ShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;

	FILE* pfile = fopen(pFileName, "r");
	if (pfile == NULL)
	{
		return E_FAIL;
	}

	fseek(pfile, 0, SEEK_END);
	long fileLength = ftell(pfile);
	fseek(pfile, 0, SEEK_SET);

	char* shaderText = new char[fileLength];

	for (int i = 0; i < fileLength; i++)
	{
		int c = fgetc(pfile);

		if (c >= 0 && c <= 127)
			shaderText[i] = c;
		else
			shaderText[i] = 0;
	}

	hr = D3DCompile(
		shaderText,
		fileLength,
		NULL,
		NULL,
		NULL,
		pEntryPoint,
		pShaderModel,
		ShaderFlags,
		0,
		ppBlob,
		&pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
		{
			// エラーメッセージ出力
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		}
	}

	if (pErrorBlob)
	{
		pErrorBlob->Release();
		pErrorBlob = NULL;
	}

	return hr;
}