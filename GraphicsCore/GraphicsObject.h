#pragma once

#include "DLL.h"
#include "DataTypesDefinition.h"
#include "Texture.h"
#include "ReleasableObject.h"
#include "Buffer.h"
#include "Shape.h"
#include <d3d11_4.h>

static D3D11_INPUT_ELEMENT_DESC INPUT_ELEMENTS_SHAPE[] = {
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static D3D11_INPUT_ELEMENT_DESC INPUT_ELEMENTS_SHAPE_AND_VALUE[] = {
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "GC_DataIndex", 0, DXGI_FORMAT_R16_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

//struct VertexAndIndex
//{
//	ID3D11Buffer* vertexBuffer;
//	ID3D11Buffer* indexBuffer;
//	UINT numVertices;
//	UINT numIndices;
//};

#define GRAPHICSOBJECT_SHAPE_MAX (8)
#define GRAPHICSOBJECT_TEXTURE_MAX (8)
#define GRAPHICSOBJECT_BUFFER_MAX (8)
#define BYTES_INPUT_ELEMENTS_SHAPE (12)
#define BYTES_INPUT_ELEMENTS_SHAPE_AND_VALUE (16)

UINT GetSizeOfVertexType(VertexType type);
UINT GetElementCountOfVertexType(VertexType type);

struct GraphicsObjectDescription
{
	VertexType vertexType;
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
	ID3D11VertexShader* vs;
	ID3D11GeometryShader* gs;
	ID3D11PixelShader* ps;
	ID3D11InputLayout* inputLayout;
};

class GraphicsObject : public ReleasableObject
{
public:
	static GraphicsObject* Create(GraphicsObjectDescription desc);
	void DownloadBuffers();
	void Release();
	bool						isLocking = true;
	GraphicsObjectDescription	description;
	VertexType					vertexType;
	D3D11_PRIMITIVE_TOPOLOGY	primitiveTopology;
	Shape*						shapes[GRAPHICSOBJECT_SHAPE_MAX];
	Texture*					textures[GRAPHICSOBJECT_TEXTURE_MAX];
	Buffer*						buffers[GRAPHICSOBJECT_BUFFER_MAX];
};

DLL_API GraphicsObject* GraphicsObject_Create(GraphicsObjectDescription desc);
//DLL_API HRESULT GraphicsObject_SetShape(GraphicsObject* object, int slot, void* vertex, UINT numVertex, void* index, UINT numIndex);
//DLL_API void GraphicsObject_UpdateShape(GraphicsObject* object, int slot, void* vertex, UINT numVertex, void* index, UINT numIndex);
DLL_API void GraphicsObject_SetTexture(GraphicsObject* object, int slot, Texture* texture);
DLL_API void GraphicsObject_SetBuffer(GraphicsObject* object, int slot, Buffer* buffer);
DLL_API void GraphicsObject_SetShape(GraphicsObject* object, int slot, Shape* shape);
DLL_API void GraphicsObject_SetLock(GraphicsObject* object, bool lock);
DLL_API Shape* GrapgicsObject_GetShape(GraphicsObject* object, int slot);
DLL_API Buffer* GrapgicsObject_GetBuffer(GraphicsObject* object, int slot);