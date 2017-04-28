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

GraphicsObject* GraphicsObject::Create(GraphicsObjectDescription desc)
{
	while (!GraphicsCore::Ready);

	GraphicsObject *go = new GraphicsObject();

	go->description = desc;

	if (desc.vertexType == VertexType_ShapeAndValue)
	{
		
	}

	go->primitiveTopology = desc.primitiveTopology;
	go->vertexType = desc.vertexType;

	go->isLocking = false;

	return go;
}

HRESULT GraphicsObject::SetVertices(void* data, UINT length)
{
	numVertices = length;

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

HRESULT GraphicsObject::SetIndices(void* data, UINT length)
{
	numIndices = length;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.ByteWidth = 4 * length;

	// サブリソースの設定
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = data;

	// 頂点バッファ生成
	if (pIndexBuffer != NULL)
	{
		pIndexBuffer->Release();
		pIndexBuffer = NULL;
	}
	return GraphicsCore::pDevice->CreateBuffer(&bd, &initData, &pIndexBuffer);
}

void GraphicsObject::DownloadBuffers()
{
	for (int i = 0; i < GRAPHICSOBJECT_BUFFER_MAX; i++)
	{
		if (buffers[i] == nullptr)
			continue;

		if (buffers[i]->buffer == nullptr)
			continue;

		GraphicsCore::pDeviceContext->VSSetShaderResources(i, 1, &buffers[i]->shaderResource);
	}
}

void GraphicsObject::Release()
{
	isLocking = true;

	ReleasableObject::Release();

	ReleaseIUnknown((IUnknown**)&pVertexBuffer);
	ReleaseIUnknown((IUnknown**)&pIndexBuffer);
	ReleaseIUnknown((IUnknown**)&description.vs);
	ReleaseIUnknown((IUnknown**)&description.gs);
	ReleaseIUnknown((IUnknown**)&description.ps);
	ReleaseIUnknown((IUnknown**)&description.inputLayout);
}

DLL_API GraphicsObject* GraphicsObject_Create(GraphicsObjectDescription desc)
{
	return GraphicsObject::Create(desc);
}

DLL_API HRESULT GraphicsObject_SetVertices(GraphicsObject* object, void* data, UINT length)
{
	return object->SetVertices(data, length);
}

DLL_API HRESULT GraphicsObject_SetIndices(GraphicsObject* object, void* data, UINT length)
{
	return object->SetIndices(data, length);
}

DLL_API void GraphicsObject_SetTexture(GraphicsObject* object, int slot, Texture* texture)
{
	object->textures[slot] = texture;
}

DLL_API void GraphicsObject_SetBuffer(GraphicsObject* object, int slot, Buffer* buffer)
{
	object->buffers[slot] = buffer;
}