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

	return go;
}

HRESULT GraphicsObject::SetShape(int slot, void* vertex, UINT numVertex, void* index, UINT numIndex)
{
	VertexAndIndex* shape = new VertexAndIndex;
	ZeroMemory(shape, sizeof(VertexAndIndex));

	HRESULT hr = SetVertices(shape, vertex, numVertex);

	if (FAILED(hr))
	{
		return hr;
	}


	if (numIndex == 0)
	{
		shapes[slot] = shape;
		return hr;
	}

	hr = SetIndices(shape, index, numIndex);

	if (FAILED(hr))
	{
		return hr;
	}

	shapes[slot] = shape;

	return hr;
}

void GraphicsObject::UpdateShape(int slot, void* vertex, UINT numVertex, void* index, UINT numIndex)
{
	if (shapes[slot] == nullptr)
		return;

	if (numVertex != 0 && shapes[slot]->vertexBuffer != nullptr)
	{
		GraphicsCore::pDeviceContext->UpdateSubresource(shapes[slot]->vertexBuffer, 0, NULL, vertex, numVertex, 0);
	}

	if (numIndex != 0 && shapes[slot]->indexBuffer != nullptr)
	{
		GraphicsCore::pDeviceContext->UpdateSubresource(shapes[slot]->indexBuffer, 0, NULL, index, numIndex, 0);
	}
}

HRESULT GraphicsObject::SetVertices(VertexAndIndex* shape, void* data, UINT length)
{
	shape->numVertices = length;

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
	if (shape->vertexBuffer != NULL)
	{
		shape->vertexBuffer->Release();
		shape->vertexBuffer = NULL;
	}
	return GraphicsCore::pDevice->CreateBuffer(&bd, &initData, &shape->vertexBuffer);
}

HRESULT GraphicsObject::SetIndices(VertexAndIndex* shape, void* data, UINT length)
{
	shape->numIndices = length;

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
	if (shape->indexBuffer != NULL)
	{
		shape->indexBuffer->Release();
		shape->indexBuffer = NULL;
	}
	return GraphicsCore::pDevice->CreateBuffer(&bd, &initData, &shape->indexBuffer);
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

	for (int i = 0; i < GRAPHICSOBJECT_SHAPE_MAX; i++)
	{
		if (shapes[i] != nullptr)
		{
			ReleaseIUnknown((IUnknown**)&shapes[i]->vertexBuffer);
			ReleaseIUnknown((IUnknown**)&shapes[i]->indexBuffer);
		}
	}


	ReleaseIUnknown((IUnknown**)&description.vs);
	ReleaseIUnknown((IUnknown**)&description.gs);
	ReleaseIUnknown((IUnknown**)&description.ps);
	ReleaseIUnknown((IUnknown**)&description.inputLayout);
}

DLL_API GraphicsObject* GraphicsObject_Create(GraphicsObjectDescription desc)
{
	return GraphicsObject::Create(desc);
}

DLL_API HRESULT GraphicsObject_SetShape(GraphicsObject* object, int slot, void* vertex, UINT numVertex, void* index, UINT numIndex)
{
	return object->SetShape(slot, vertex, numVertex, index, numIndex);
}

DLL_API void GraphicsObject_UpdateShape(GraphicsObject* object, int slot, void* vertex, UINT numVertex, void* index, UINT numIndex)
{
	object->UpdateShape(slot, vertex, numVertex, index, numIndex);
}

DLL_API void GraphicsObject_SetTexture(GraphicsObject* object, int slot, Texture* texture)
{
	object->textures[slot] = texture;
}

DLL_API void GraphicsObject_SetBuffer(GraphicsObject* object, int slot, Buffer* buffer)
{
	object->buffers[slot] = buffer;
}

DLL_API void GraphicsObject_SetLock(GraphicsObject* object, bool lock)
{
	object->isLocking = lock;
}