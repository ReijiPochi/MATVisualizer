#pragma once

#include "DLL.h"
#include "DataTypesDefinition.h"
#include "Texture.h"
#include "ReleasableObject.h"
#include "Buffer.h"
#include <d3d11_4.h>

enum VertexType
{
	VertexType_Shape,
	VertexType_ShapeAndValue,
};

static D3D11_INPUT_ELEMENT_DESC INPUT_ELEMENTS_SHAPE[] = {
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static D3D11_INPUT_ELEMENT_DESC INPUT_ELEMENTS_SHAPE_AND_VALUE[] = {
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "GC_DataIndex", 0, DXGI_FORMAT_R16_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

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
};

class GraphicsObject : public ReleasableObject
{
public:
	static GraphicsObject* Create(GraphicsObjectDescription desc);
	HRESULT SetVertices(void* data, UINT length);
	void DownloadBuffers();
	void Release();
	bool isLocking = true;
	VertexType					vertexType;
	D3D11_PRIMITIVE_TOPOLOGY	primitiveTopology;
	ID3D11Buffer*				pVertexBuffer;
	ID3D11Buffer*				pIndexBuffer;
	ID3D11VertexShader*			pVertexShader;
	ID3D11GeometryShader*		pGeometryShader;
	ID3D11PixelShader*			pPixelShader;
	ID3D11InputLayout*			pInputLayout;
	Texture*					textures[GRAPHICSOBJECT_TEXTURE_MAX];
	Buffer*						buffers[GRAPHICSOBJECT_BUFFER_MAX];

private:
	static HRESULT GenerateVertexShaderAndInputLayout(GraphicsObjectDescription desc, ID3D11VertexShader** ppVS, ID3D11InputLayout** ppinputLayout);
	static HRESULT GenerateGeometryShader(ID3D11GeometryShader** ppGS);
	static HRESULT GeneratePixelShader(ID3D11PixelShader** ppPS);
	static HRESULT CompileShaderFromFile(char* pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3DBlob** ppBlob);
};

extern "C"
{
	DLL_API GraphicsObject* GraphicsObject_Create(GraphicsObjectDescription desc);
	DLL_API HRESULT GraphicsObject_SetVertices(GraphicsObject* object, void* data, UINT length);
	DLL_API void GraphicsObject_SetTexture(GraphicsObject* object, int slot, Texture* texture);
	DLL_API void GraphicsObject_SetBuffer(GraphicsObject* object, int slot, Buffer* buffer);
}