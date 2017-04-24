#pragma once

#include <d3d11_4.h>
#include "ReleasableObject.h"

struct ConstantBufferDescription
{
	int byteWidth;
};

class ConstantBuffer : public ReleasableObject
{
public:
	static ConstantBuffer* Create(ConstantBufferDescription desc);
	void Update(void* data);
	void Set();
	void Release();
	ID3D11Buffer* buffer;
};