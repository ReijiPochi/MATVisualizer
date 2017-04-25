#pragma once

#include <DirectXMath.h>

class Camera
{
public:
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// FOV AngleY
	float fov = 1.0;
	DirectX::XMMATRIX cameraMatrix;
	void SetMatrix();
};