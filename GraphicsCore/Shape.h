#pragma once

#include "ReleasableObject.h"
#include "DLL.h"
#include "DataTypesDefinition.h"
#include <d3d11_4.h>

struct Shape : public ReleasableObject
{
private:
	//static std::vector<Shape*> *shapeList;
public:
	//static void ReleaseAll();
	Shape(VertexType type);
	ID3D11Buffer* vertexBuffer = nullptr;
	VertexType vertexType;
	ID3D11Buffer* indexBuffer = nullptr;
	UINT numVertices;
	UINT numIndices;
	HRESULT Set(void* vertex, UINT numVertex, void* index, UINT numIndex);
	void Update(void* vertex, UINT numVertex, void* index, UINT numIndex);
	HRESULT SetVertices(void* data, UINT length);
	HRESULT SetIndices(void* data, UINT length);
	void Release();
};

DLL_API Shape* Shape_Create(VertexType type);
DLL_API HRESULT Shape_Set(Shape* shape, void* vertex, UINT numVertex, void* index, UINT numIndex);
DLL_API void Shape_Update(Shape* shape, void* vertex, UINT numVertex, void* index, UINT numIndex);