//********************************************************************************
//
// ドローンのカメラクラス[camera_drone.h]
//
//															Author : Riugo Honda
//															Date   :2025/02/16
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/18
//********************************************************************************
#include "directx.h"
#include "configuration.h"
#include "scene.h"
#include "shader.h"
#include "keyboard.h"
#include "ship.h"
#include "player.h"
#include "controller.h"
#include "easing.h"
#include "mode.h"

#include "camera_drone.h"

/*------------------------------------------------------------------------------
   定数宣言
------------------------------------------------------------------------------*/

#ifndef PI
#define PI (3.1415926545f)
#endif

//カメラの回転関係の定数
#define RADIAN (PI / 180.0f)
#define DRONE_ROTATION_RADIUS (60.0f)
#define DRONE_ROTATION_HEIGHT (25.0f)
#define ANGLE_INCREASERATE (1.0f)

//カメラの上昇関係の定数
#define RISING_RATE_Y (0.05f)
#define RISING_RATE_Z (0.15f)
#define RISE_Y_ENDPOINT (18.0f)
#define RISE_Z_ENDPOINT (60.0f)

//カメラの下降追跡の定積
#define DESCENDING_RATE_Y (0.1f)
#define DESCENDING_RATE_Z (0.25f)
#define DESCEND_Y_ENDPOINT (1.0f)
#define DESCEND_Z_ENDPOINT (6.0f)
#define DESCEND_Y_STARTPOINT (18.0f)
#define DESCEND_Z_STARTPOINT (60.0f)


/*------------------------------------------------------------------------------
   関数宣言
------------------------------------------------------------------------------*/

DroneCamera::DroneCamera(XMFLOAT3 target, DroneView type)
{
	m_Position = {0.0f , 0.0f , 0.0f};
	m_Target = target;
	m_DroneType = type;
	

	m_RotatingAngle = 0.0f;

	m_Radian = m_RotatingAngle * RADIAN;

	m_Up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Lenge = 5.0f;
	m_Zoom = 1.0f;

	m_OneLap = false;

	
	//他のドローンを使わないからとりあえずこれで固定
	m_Projection = XMMatrixPerspectiveFovLH(m_Zoom, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);// 後で要調整
	

}

void DroneCamera::Update()
{
	switch (m_DroneType)
	{
	case DRONE_TYPE_RISEVIEW:
		//ドローンの垂直上昇
		DronaRise();
		break;
	case DRONE_TYPE_CHASEVIEW:
		//ドローンの水平移動
		DroneChaseHorizontaly();
		break;
	case DRONE_TYPE_CIRCLEVIEW:
		//ドローンの空中旋回
		DroneRotation();
		break;
	case DRONE_TYPE_CURVE_RISEVIEW:
		DroneCurveRising();
		break;
	case DRONE_TYPE_CURVE_DESCENDVIEW:
		DroneTowardsPlayer();
		break;
	}

	
}

void DroneCamera::Draw()
{
	m_View = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_Target), XMLoadFloat3(&m_Up));
}

DroneView DroneCamera::GetDroneView()
{
	return m_DroneType;
}

bool DroneCamera::IsOneLapFinished()
{
	return m_OneLap;
}

//空中旋回
void DroneCamera::DroneRotation()
{
	m_Position.x =  sinf(m_Radian) * DRONE_ROTATION_RADIUS;
	m_Position.y = m_Target.y + DRONE_ROTATION_HEIGHT;
	m_Position.z = - cosf(m_Radian) * DRONE_ROTATION_RADIUS;

	m_RotatingAngle += ANGLE_INCREASERATE;

	m_Radian = m_RotatingAngle * RADIAN;

	if (m_Radian >= (2 * PI) + 1.0f)
	{
		m_OneLap = true;
	}
}

//ドローンのターゲットは固定ドローンは上昇
void DroneCamera::DronaRise()
{
	//ドローンの位置を初期化・・・ここでターゲットに視点も併せてるためこの後に位置補正を行ってる
	m_Position.x = m_Target.x + sinf(m_Rotation.y) * cosf(m_Rotation.x) * m_Lenge;
	m_Position.y = m_Target.y + m_Lenge;
	m_Position.z = m_Target.z - cosf(m_Rotation.y) * cosf(m_Rotation.x) * (m_Lenge);//m_Lenge

	static float raisecamera = 0.0f;

	m_Position.y += raisecamera;

	raisecamera += RISING_RATE_Y;
}


//ドローンのターゲット及び本体の水平移動
void DroneCamera::DroneChaseHorizontaly()
{

	//ドローンの位置を初期化・・・ここでターゲットに視点も併せてるためこの後に位置補正を行ってる
	m_Position.x = m_Target.x - sinf(m_Rotation.y) * cosf(m_Rotation.x) * m_Lenge;
	m_Position.y = m_Target.y + 1.5;
	m_Position.z = m_Target.z + cosf(m_Rotation.y) * cosf(m_Rotation.x) * (m_Lenge);

	//ポジションはターゲットに補正されるためここではターゲットを移動させてる
	static float walkingcamera = 0.0f;

	m_Target.z -= walkingcamera;

	walkingcamera += 0.001;

	
}

//ドローンのターゲット固定、本体は後退しながら上昇
void DroneCamera::DroneCurveRising()
{
	//ドローンの位置を初期化・・・ここでターゲットに視点も併せてるためこの後に位置補正を行ってる
	m_Position.x = m_Target.x + sinf(m_Rotation.y) * cosf(m_Rotation.x) * m_Lenge;
	m_Position.y = m_Target.y + m_Lenge;
	m_Position.z = m_Target.z + cosf(m_Rotation.y) * cosf(m_Rotation.x) * (m_Lenge);

	static float raisecamera = 0;
	static float movecamera = 0;

	m_Position.y += raisecamera;
	m_Position.z += movecamera;

	if (raisecamera <= RISE_Y_ENDPOINT)
	{
		raisecamera += RISING_RATE_Y;
	}
	
	if (movecamera <= RISE_Z_ENDPOINT)
	{
		movecamera += RISING_RATE_Z;
	}
	
}

//ドローンのターゲット固定、本体は下降しながらターゲットへ向かって移動
void DroneCamera::DroneTowardsPlayer()
{
	static float descendcamera = DESCEND_Y_STARTPOINT;
	static float movecamera = DESCEND_Z_STARTPOINT;

	//ドローンの位置を初期化・・・ここでターゲットに視点も併せてるためこの後に位置補正を行ってる
	m_Position.x = m_Target.x + sinf(m_Rotation.y) * cosf(m_Rotation.x) * m_Lenge;
	m_Position.y = m_Target.y + (descendcamera);
	m_Position.z = m_Target.z + cosf(m_Rotation.y) * cosf(m_Rotation.x) * (movecamera);

	if (descendcamera >= DESCEND_Y_ENDPOINT)
	{
		descendcamera -= DESCENDING_RATE_Y;
	}

	if (movecamera >= DESCEND_Z_ENDPOINT)
	{
		movecamera -= DESCENDING_RATE_Z;
	}
}
