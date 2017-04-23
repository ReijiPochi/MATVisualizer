#include "Texture.h"
#include "GraphicsCore.h"
#include <d3d11_4.h>

Texture2D* Texture2D::Create(Texture2DDescription desc)
{
	Texture2D* result = new Texture2D;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = desc.width;
	textureDesc.Height = desc.height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = desc.textureFormat;
	textureDesc.SampleDesc.Count = desc.sample;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = desc.bindFlags;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	GraphicsCore::pDevice->CreateTexture2D(&textureDesc, NULL, &result->texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.resourceFormat;
	if (desc.sample == 1)
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}
	else
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	}

	GraphicsCore::pDevice->CreateShaderResourceView(result->texture, &srvDesc, &result->shaderResource);

	return result;
}

void Texture2D::Release()
{
	ReleaseIUnknown(texture);
	ReleaseIUnknown(shaderResource);
}

Texture1D* Texture1D::Create(Texture1DDescription desc, void* data)
{
	Texture1D* result = new Texture1D;

	D3D11_TEXTURE1D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE1D_DESC));
	textureDesc.Width = desc.length;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = desc.textureFormat;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = desc.bindFlags;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem = data;
	subresourceData.SysMemPitch = 0;
	subresourceData.SysMemSlicePitch = 0;

	GraphicsCore::pDevice->CreateTexture1D(&textureDesc, &subresourceData, &result->texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.resourceFormat;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;

	GraphicsCore::pDevice->CreateShaderResourceView(result->texture, &srvDesc, &result->shaderResource);

	return result;
}