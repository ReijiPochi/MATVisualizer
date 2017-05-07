#include "Camera.h"
#include "GraphicsCore.h"


void Camera::SetMatrix()
{
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(eye, target, up);

	float aspectRatio = GraphicsCore::viewWidth / GraphicsCore::viewHeight;

	cameraMatrix = view * DirectX::XMMatrixPerspectiveFovLH(3.141593f * fov / 180.0f, aspectRatio, 0.01f, 1000.0f);
}

DLL_API Camera* Camera_Create()
{
	return new Camera();
}

DLL_API void Camera_Update(Camera* camera, DirectX::XMMATRIX* matrix)
{
	camera->cameraMatrix = *matrix;
}