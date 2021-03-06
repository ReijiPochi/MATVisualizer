#pragma once

#include "ReleasableObject.h"
#include "DLL.h"
#include <d3d11_4.h>

struct BufferDescription
{
	int elementSize;
	int numElements;
};

struct Buffer
{
public:
	static Buffer* Create(BufferDescription* desc, void* data);
	void Release();
	ID3D11Buffer* buffer;
	ID3D11ShaderResourceView* shaderResource;
};


DLL_API Buffer* Buffer_Create(BufferDescription* desc, void* data);
DLL_API void Buffer_Release(Buffer* buffer);