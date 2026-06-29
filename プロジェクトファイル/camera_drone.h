//********************************************************************************
//
// ドローンのカメラクラス[camera_drone.h]
//
//															Author : Riugo Honda
//															Date   :2025/02/16
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/16
//********************************************************************************
#pragma once


#ifndef _CAMERA_DRONE_H_
#define _CAMERA_DRONE_H_
#include "configuration.h"
#include "camera.h"

class Ship;
class Player;

typedef enum
{
	DRONE_TYPE_NONE = -1,
	DRONE_TYPE_RISEVIEW,
	DRONE_TYPE_CHASEVIEW,
	DRONE_TYPE_CIRCLEVIEW,
	DRONE_TYPE_CURVE_RISEVIEW,
	DRONE_TYPE_CURVE_DESCENDVIEW
}DroneView;



class DroneCamera :public Camera
{
private:
	float m_Lenge;
	float m_Zoom;

	//カメラのステート
	DroneView m_DroneType;

	//カメラのシェイク用
	float m_Shake = 0.0f;
	float m_ShakeTime = 0.0f;

	//ドローンの回転制御
	float m_RotatingAngle;
	float m_Radian;
	bool m_OneLap;
public:
	DroneCamera(XMFLOAT3 target,  DroneView type);
	~DroneCamera() {};

	//void Update(int team);
	void Update() override;
	void Draw()override;

	DroneView GetDroneView();

	bool IsOneLapFinished();
private:
	void DroneRotation();
	void DronaRise();
	void DroneChaseHorizontaly();
	void DroneCurveRising();
	void DroneTowardsPlayer();
};
#endif // !_CAMERA_DRONE_H_

