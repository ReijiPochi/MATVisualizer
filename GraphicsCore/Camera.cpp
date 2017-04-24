#include "Camera.h"
#include "GraphicsCore.h"


void Camera::SetMatrix()
{
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(eye, target, up);

	float aspectRatio = GraphicsCore::viewWidth / GraphicsCore::viewHeight;

	cameraMatrix = view * DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.01f, 1000.0f);
}