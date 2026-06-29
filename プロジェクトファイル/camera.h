//	camera.h
//	
//
//	2024.10.29			‘åŽR—½‘¾˜N

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
protected:
	XMMATRIX m_View;
	XMMATRIX m_Projection;

	XMFLOAT3 m_Position;
	XMFLOAT3 m_Target;
	XMFLOAT3 m_Up;
	XMFLOAT3 m_Rotation;

public:
	Camera() {};
	virtual ~Camera() {};
	
	virtual void Update() {};
	virtual void Draw() {};

	XMMATRIX GetCameraView();
	XMMATRIX GetCameraProjection();
};

#endif // CAMERA_H
