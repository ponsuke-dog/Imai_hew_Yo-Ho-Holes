//********************************************************************************
//
// 定点カメラのカメラクラス[camera_fixedpoint.h]
//
//															Author : Riugo Honda
//															Date   :2025/02/19
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/19
//********************************************************************************
#pragma once


#ifndef _CAMERA_FIXEDPOINT_H_
#define _CAMERA_FIXEDPOINT_H_
#include "configuration.h"
#include "camera.h"

class Ship;
class Player;

typedef enum
{
	FIXEDPOINT_NONEVIEW = -1,

	FIXEDPOINT_TOPVIEW,
	FIXEDPOINT_FRONTVIEW,
	FIXEDPOINT_REARVIEW,
	FIXEDPOINT_SIDEVIEW_RIGHT,
	FIXEDPOINT_SIDEVIEW_LEFT,

	FIXEDPOINT_MAXVIEW,
}FixedpointView;



class FixedpointCamera :public Camera
{
private:
	XMFLOAT3 m_Distance;
	float m_Zoom;

	//カメラのステート
	FixedpointView m_FixedpointType;

	//カメラのシェイク用
	float m_Shake = 0.0f;
	float m_ShakeTime = 0.0f;

	GAMEMODE m_GameMode;

	float m_RotatingAngle;
	float m_Radian;
public:
	FixedpointCamera(XMFLOAT3 target, XMFLOAT3  distance,FixedpointView type, GAMEMODE mode);
	~FixedpointCamera() {};

	//void Update(int team);
	void Update() override;
	void Draw()override;

	FixedpointView GetFixedpointView();

private:
	void TopUpdate();
	void RearUpdate();
	void SideRightUpdate();
	void SideLeftUpdate();
};
#endif // !_CAMERA_FIXEDPOINT_H_
