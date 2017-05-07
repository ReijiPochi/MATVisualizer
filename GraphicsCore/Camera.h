#pragma once

#include "DLL.h"
#include <DirectXMath.h>

class Camera
{
public:
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	float fov = 90.0f;
	DirectX::XMMATRIX cameraMatrix;
	void SetMatrix();
};

DLL_API Camera* Camera_Create();
DLL_API void Camera_Update(Camera* camera, DirectX::XMMATRIX* matrix);