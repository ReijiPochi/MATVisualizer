#include "Shape.h"
#include "GraphicsCore.h"

struct SetUpdateCallbackArgs
{
	Shape* shape;
	void* vertex;
	UINT numVertex;
	void* index;
	UINT numIndex;
};

//std::vector<Shape*> *Shape::shapeList = new std::vector<Shape*>;
//
//void Shape::ReleaseAll()
//{
//	for (std::vector<Shape*>::iterator itr = shapeList->begin(); itr != shapeList->end(); ++itr)
//	{
//		if ((*itr) != nullptr)
//		{
//			(*itr)->Release();
//			delete *itr;
//		}
//	}
//}

Shape::Shape(VertexType type)
{
	vertexType = type;
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	numVertices = 0;
	numIndices = 0;

	//shapeList->push_back(this);
}

HRESULT Shape::Set(void* vertex, UINT numVertex, void* index, UINT numIndex)
{
	SetVertices(vertex, numVertex);
	SetIndices(index, numIndex);

	return S_OK;
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

	if (length == 0)
		return S_OK;

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

	if (length == 0)
		return S_OK;

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
	if (vertexBuffer)vertexBuffer->Release();
	if (indexBuffer)indexBuffer->Release();

	vertexBuffer = nullptr;
	indexBuffer = nullptr;

	// なんでかReleaseIUnknownを使うとアクセス違反が
	//ReleaseIUnknown((IUnknown**)vertexBuffer);
	//ReleaseIUnknown((IUnknown**)indexBuffer);
}

void SetCallback(void* data)
{
	SetUpdateCallbackArgs args = *(SetUpdateCallbackArgs*)data;

	args.shape->Set(args.vertex, args.numVertex, args.index, args.numIndex);
}

void UpdateCallback(void* data)
{
	SetUpdateCallbackArgs args = *(SetUpdateCallbackArgs*)data;

	args.shape->Update(args.vertex, args.numVertex, args.index, args.numIndex);
}

DLL_API Shape* Shape_Create(VertexType type)
{
	return new Shape(type);
}

DLL_API HRESULT Shape_Set(Shape* shape, void* vertex, UINT numVertex, void* index, UINT numIndex)
{
	//SetUpdateCallbackArgs* args = new SetUpdateCallbackArgs;
	//args->shape = shape;
	//args->vertex = vertex;
	//args->numVertex = numVertex;
	//args->index = index;
	//args->numIndex = numIndex;

	//CallbackData* callback = new CallbackData;
	//callback->function = SetCallback;
	//callback->data = args;

	//AddToQueue(*callback);

	shape->Set(vertex, numVertex, index, numIndex);

	return S_OK;
}

DLL_API void Shape_Update(Shape* shape, void* vertex, UINT numVertex, void* index, UINT numIndex)
{
	//SetUpdateCallbackArgs* args = new SetUpdateCallbackArgs;
	//args->shape = shape;
	//args->vertex = vertex;
	//args->numVertex = numVertex;
	//args->index = index;
	//args->numIndex = numIndex;

	//CallbackData* callback = new CallbackData;
	//callback->function = UpdateCallback;
	//callback->data = args;

	//AddToQueue(*callback);

	shape->Update(vertex, numVertex, index,numIndex);
}

DLL_API void Shape_Release(Shape* shape)
{
	shape->Release();
}