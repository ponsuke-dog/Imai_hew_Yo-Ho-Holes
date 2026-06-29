//	camera.cpp
//	
//
//	2024.10.29			‘åŽR—½‘¾˜N
#include "directx.h"
#include "camera.h"

XMMATRIX Camera::GetCameraView()
{
	return m_View;;
}

XMMATRIX Camera::GetCameraProjection()
{
	return m_Projection;
}