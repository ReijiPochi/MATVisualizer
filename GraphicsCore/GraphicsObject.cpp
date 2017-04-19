#include "GraphicsObject.h"
#include "GraphicsCore.h"
#include <windows.h>
#include <stdio.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

int GraphicsObject::Create(GraphicsObjectDescription desc)
{
	while (!GraphicsCore::Ready);

	int id = GetNewObjectID();

	if (id < 0)
		return -1;

	GraphicsObject* go = new GraphicsObject();

	GenerateVertexShaderAndInputLayout(&(go->pVertexShader), &(go->pInputLayout));

	objects[id] = go;
}

// 使用できるオブジェクトスロットを検索します。
int GraphicsObject::GetNewObjectID()
{
	for (int index = 0; index < GRAPHICS_OBJECT_MAX_COUNT; index++)
	{
		if (objects[index] == NULL)
		{
			return index;
		}
	}

	return -1;
}

HRESULT GraphicsObject::GenerateVertexShaderAndInputLayout(ID3D11VertexShader** ppVS, ID3D11InputLayout** ppinputLayout)
{
	HRESULT result = S_OK;

	ID3DBlob* pVSBlob = NULL;
	result = CompileShaderFromFile("Resources/Effects/Sample.fx", "VSFunc", "vs_5_0", &pVSBlob);

	result = GraphicsCore::pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, ppVS);
	if (FAILED(result))
	{
		pVSBlob->Release();
		pVSBlob = NULL;
		return result;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// 入力レイアウトを生成
	result = GraphicsCore::pDevice->CreateInputLayout(
		layout,
		numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		ppinputLayout);

	pVSBlob->Release();
	pVSBlob = NULL;

	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT GraphicsObject::CompileShaderFromFile(char* pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3DBlob** ppBlob)
{
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

DLL_API int GraphicsObject_Create(GraphicsObjectDescription desc)
{
	return GraphicsObject::Create(desc);
}