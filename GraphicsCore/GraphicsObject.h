#pragma once

#include "DLL.h"
#include <d3d11_4.h>

#define GRAPHICS_OBJECT_MAX_COUNT (128)

struct GraphicsObjectDescription
{
	int dummy;
};

class GraphicsObject
{
public:
	static int Create(GraphicsObjectDescription desc);
	int objectID;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
	ID3D11VertexShader* pVertexShader;
	ID3D11GeometryShader* pGeometryShader;
	ID3D11PixelShader* pPixelShader;
	ID3D11InputLayout* pInputLayout;

private:
	static int GetNewObjectID();
	static HRESULT GenerateVertexShaderAndInputLayout(ID3D11VertexShader** ppVS, ID3D11InputLayout** ppinputLayout);
	static HRESULT CompileShaderFromFile(char* pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3DBlob** ppBlob);
};

extern "C"
{
	static GraphicsObject* objects[GRAPHICS_OBJECT_MAX_COUNT];
	DLL_API int GraphicsObject_Create(GraphicsObjectDescription desc);
}