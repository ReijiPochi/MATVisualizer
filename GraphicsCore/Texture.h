#pragma once

#include <d3d11_4.h>
#include <dxgi1_5.h>

struct Texture2DDescription
{
	int width = 1;
	int height = 1;
	DXGI_FORMAT textureFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT resourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	int sample = 1;
	UINT bindFlags = D3D11_BIND_SHADER_RESOURCE;
};

struct Texture1DDescription
{
	int length = 1;
	DXGI_FORMAT textureFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT resourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	UINT bindFlags = D3D11_BIND_SHADER_RESOURCE;
};

class Texture2D
{
public:
	static Texture2D* Create(Texture2DDescription desc);
	void Release();
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView *shaderResource;
};

class Texture1D
{
public:
	static Texture1D* Create(Texture1DDescription desc, void* data);
	ID3D11Texture1D* texture;
	ID3D11ShaderResourceView* shaderResource;
};