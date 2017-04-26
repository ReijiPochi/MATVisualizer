#include "ConstantBuffer.h"
#include "GraphicsCore.h"

ConstantBuffer* ConstantBuffer::Create(ConstantBufferDescription desc)
{
	ConstantBuffer* result = new ConstantBuffer;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.ByteWidth = desc.byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	GraphicsCore::pDevice->CreateBuffer(&bufferDesc, NULL, &result->buffer);

	return result;
}

void ConstantBuffer::Update(void* data)
{
	GraphicsCore::pDeviceContext->UpdateSubresource(buffer, 0, NULL, data, 0, 0);
}

void ConstantBuffer::Set()
{
	GraphicsCore::pDeviceContext->VSSetConstantBuffers(0, 1, &buffer);
	GraphicsCore::pDeviceContext->GSSetConstantBuffers(0, 1, &buffer);
	GraphicsCore::pDeviceContext->PSSetConstantBuffers(0, 1, &buffer);
}

void ConstantBuffer::Release()
{
	ReleasableObject::Release();
	ReleaseIUnknown((IUnknown**)&buffer);
}