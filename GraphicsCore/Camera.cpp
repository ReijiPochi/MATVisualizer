#include "Camera.h"
#include "GraphicsCore.h"


Camera* Camera::Create()
{
	Camera* result = new Camera;

	return result;
}

void Camera::SetMatrix()
{
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(eye, target, up);

	float aspectRatio = GraphicsCore::viewWidth / GraphicsCore::viewHeight;

	cameraMatrix = view * DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.01f, 1000.0f);
}

DLL_API Camera* Camera_Create()
{
	return Camera::Create();
}

DLL_API void Camera_Update(Camera* camera, DirectX::XMMATRIX* matrix)
{
	camera->cameraMatrix = *matrix;
}