#include "GraphicsObject.h"
#include "GraphicsCore.h"
#include <windows.h>
#include <stdio.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

UINT GetSizeOfVertexType(VertexType type)
{
	switch (type)
	{
	case VertexType_Shape:
		return BYTES_INPUT_ELEMENTS_SHAPE;

	case VertexType_ShapeAndValue:
		return BYTES_INPUT_ELEMENTS_SHAPE_AND_VALUE;

	default:
		return 0;
	}
}

UINT GetElementCountOfVertexType(VertexType type)
{
	switch (type)
	{
	case VertexType_Shape:
		return 1;

	case VertexType_ShapeAndValue:
		return 2;

	default:
		return 0;
	}
}

int GraphicsObject::Create(GraphicsObjectDescription desc)
{
	while (!GraphicsCore::Ready);

	int id = GetNewObjectID();

	if (id < 0)
		return -1;

	GraphicsObject *go = new GraphicsObject();

	GenerateVertexShaderAndInputLayout(desc, &(go->pVertexShader), &(go->pInputLayout));
	GenerateGeometryShader(&go->pGeometryShader);
	GeneratePixelShader(&go->pPixelShader);

	if (desc.vertexType == VertexType_ShapeAndValue)
	{
		
	}

	go->primitiveTopology = desc.primitiveTopology;
	go->vertexType = desc.vertexType;

	GraphicsCore::objects[id] = go;

	GraphicsCore::objects[id]->isLocking = false;

	return id;
}

HRESULT GraphicsObject::SetVertices(void* data, UINT length)
{
	// 頂点バッファの設定
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	switch (vertexType)
	{
	case VertexType_Shape:
		bd.ByteWidth = sizeof(VertexData_Shape) * length;
		break;

	case VertexType_ShapeAndValue:
		bd.ByteWidth = sizeof(VertexData_ShapeAndValue) * length;
		break;

	default:
		break;
	}

	// サブリソースの設定
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = data;

	// 頂点バッファ生成
	if (pVertexBuffer != NULL)
	{
		pVertexBuffer->Release();
		pVertexBuffer = NULL;
	}
	return GraphicsCore::pDevice->CreateBuffer(&bd, &initData, &pVertexBuffer);
}

void GraphicsObject::Release()
{
	isLocking = true;

	ReleaseIUnknown(pVertexBuffer);
	ReleaseIUnknown(pIndexBuffer);
	ReleaseIUnknown(pVertexShader);
	ReleaseIUnknown(pGeometryShader);
	ReleaseIUnknown(pPixelShader);
	ReleaseIUnknown(pInputLayout);

	GraphicsCore::objects[objectID] = nullptr;
}

// 使用できるオブジェクトスロットを検索します。
int GraphicsObject::GetNewObjectID()
{
	for (int index = 0; index < GRAPHICS_OBJECT_MAX_COUNT; index++)
	{
		if (GraphicsCore::objects[index] == nullptr)
		{
			return index;
		}
	}

	return -1;
}

HRESULT GraphicsObject::GenerateVertexShaderAndInputLayout(GraphicsObjectDescription desc, ID3D11VertexShader** ppVS, ID3D11InputLayout** ppinputLayout)
{
	HRESULT result = S_OK;

	ID3DBlob* pVSBlob = NULL;
	result = CompileShaderFromFile("Resources/Effects/Sample.fx", "VSFunc", "vs_5_0", &pVSBlob);
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

	switch (desc.vertexType)
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

	UINT numElements = GetElementCountOfVertexType(desc.vertexType);

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

HRESULT GraphicsObject::GenerateGeometryShader(ID3D11GeometryShader** ppGS)
{
	HRESULT result;

	ID3DBlob* pGSBlob = NULL;
	result = CompileShaderFromFile("Resources/Effects/Sample.fx", "GSFunc", "gs_5_0", &pGSBlob);
	if(FAILED(result))
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

HRESULT GraphicsObject::GeneratePixelShader(ID3D11PixelShader** ppPS)
{
	HRESULT result;

	ID3DBlob* pPSBlob = NULL;
	result = CompileShaderFromFile("Resources/Effects/Sample.fx", "PSFunc", "ps_5_0", &pPSBlob);
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

DLL_API HRESULT GraphicsObject_SetVertices(int objectID, void* data, UINT length)
{
	return GraphicsCore::objects[objectID]->SetVertices(data, length);
}

DLL_API void SetTexture(int objectID, int slot, Texture* texture)
{
	GraphicsCore::objects[objectID]->textures[slot] = texture;
}