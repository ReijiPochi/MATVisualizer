#pragma once

#include "DLL.h"
#include "GraphicsObject.h"
#include <d3d11_4.h>


DLL_API HRESULT Shader_GenerateVertexShaderAndInputLayout(LPCSTR fileName, LPCSTR entryPoint, GraphicsObjectDescription* desc, ID3D11VertexShader** ppVS, ID3D11InputLayout** ppinputLayout);
DLL_API HRESULT Shader_GenerateGeometryShader(LPCSTR fileName, LPCSTR entryPoint, ID3D11GeometryShader** ppGS);
DLL_API HRESULT Shader_GeneratePixelShader(LPCSTR fileName, LPCSTR entryPoint, ID3D11PixelShader** ppPS);
HRESULT Shader_CompileShaderFromFile(LPCSTR pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3DBlob** ppBlob);