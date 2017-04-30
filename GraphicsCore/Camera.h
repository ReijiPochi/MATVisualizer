#pragma once

#include "DLL.h"
#include <DirectXMath.h>

class Camera
{
public:
	static Camera* Create();
	void SetMatrix();
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	float fov;
	DirectX::XMMATRIX cameraMatrix;
};

DLL_API Camera* Camera_Create();
DLL_API void Camera_Update(Camera* camera, DirectX::XMMATRIX* matrix);