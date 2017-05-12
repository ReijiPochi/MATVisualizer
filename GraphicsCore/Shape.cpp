#include "Shape.h"
#include "GraphicsCore.h"

Shape::Shape(VertexType type)
{
	vertexType = type;
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	numVertices = 0;
	numIndices = 0;
}

HRESULT Shape::Set(VertexType vertexType, void* vertex, UINT numVertex, void* index, UINT numIndex)
{
	HRESULT hr = SetVertices(vertex, numVertex);

	if (FAILED(hr))
	{
		return hr;
	}

	this->vertexType = vertexType;


	if (numIndex == 0)
	{
		return hr;
	}

	hr = SetIndices(index, numIndex);

	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void Shape::Update(void* vertex, UINT numVertex, void* index, UINT numIndex)
{
	if (numVertex != 0 && vertexBuffer != nullptr)
	{
		numVertices = numVertex;
		GraphicsCore::pDeviceContext->UpdateSubresource(vertexBuffer, 0, NULL, vertex, numVertex, 0);
	}
	
	if (numIndex != 0 && indexBuffer != nullptr)
	{
		numIndices = numIndex;
		GraphicsCore::pDeviceContext->UpdateSubresource(indexBuffer, 0, NULL, index, numIndex, 0);
	}
}

HRESULT Shape::SetVertices(void* data, UINT length)
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
	if (vertexBuffer != nullptr)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
	return GraphicsCore::pDevice->CreateBuffer(&bd, &initData, &vertexBuffer);

	return S_OK;
}

HRESULT Shape::SetIndices(void* data, UINT length)
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
	if (indexBuffer != nullptr)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
	return GraphicsCore::pDevice->CreateBuffer(&bd, &initData, &indexBuffer);
}

void Shape::Release()
{
	ReleasableObject::Release();

	ReleaseIUnknown((IUnknown**)vertexBuffer);
	ReleaseIUnknown((IUnknown**)indexBuffer);
}

DLL_API Shape* Shape_Create(VertexType type)
{
	return new Shape(type);
}

DLL_API HRESULT Shape_Set(Shape* shape, VertexType vertexType, void* vertex, UINT numVertex, void* index, UINT numIndex)
{
	return shape->Set(vertexType, vertex, numVertex, index, numIndex);
}

DLL_API void Shape_Update(Shape* shape, void* vertex, UINT numVertex, void* index, UINT numIndex)
{
	shape->Update(vertex, numVertex, index, numIndex);
}