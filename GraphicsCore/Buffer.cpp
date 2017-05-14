#include "Buffer.h"
#include "GraphicsCore.h"

Buffer* Buffer::Create(BufferDescription* desc, void* data)
{
	if (desc->numElements == 0)
		return nullptr;

	Buffer* result = new Buffer;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.ByteWidth = desc->elementSize * desc->numElements;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = desc->elementSize;

	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem = data;

	GraphicsCore::pDevice->CreateBuffer(&bufferDesc, &subresourceData, &result->buffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.NumElements = desc->numElements;

	GraphicsCore::pDevice->CreateShaderResourceView(result->buffer, &srvDesc, &result->shaderResource);

	return result;
}

void Buffer::Release()
{
	ReleasableObject::Release();

	ReleaseIUnknown((IUnknown**)&shaderResource);
	ReleaseIUnknown((IUnknown**)&buffer);
}

DLL_API Buffer* Buffer_Create(BufferDescription* desc, void* data)
{
	return Buffer::Create(desc, data);
}