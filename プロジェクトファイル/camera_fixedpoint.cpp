//********************************************************************************
//
// 定点カメラのカメラクラス[camera_fixedpoint.h]
//
//															Author : Riugo Honda
//															Date   :2025/02/19
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/19
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

#include "camera_fixedpoint.h"

/*------------------------------------------------------------------------------
   定数宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
   関数宣言
------------------------------------------------------------------------------*/

FixedpointCamera::FixedpointCamera(XMFLOAT3 target, XMFLOAT3  distance, FixedpointView type, GAMEMODE mode)
{
	m_Position = { 0.0f , 0.0f , 0.0f };
	m_Target = target;
	m_FixedpointType = type;
	m_GameMode = mode;

	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Distance = distance;
	m_Zoom = 1.0f;

	if (type == FIXEDPOINT_TOPVIEW)
	{
		m_Up = XMFLOAT3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		m_Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	if (mode == GAMEMODE_LOCAL_SPLITSCREEN || mode == GAMEMODE_SEGMENTATION)
	{
		m_Projection = XMMatrixPerspectiveFovLH(m_Zoom, (float)SCREEN_WIDTH * 0.5f / SCREEN_HEIGHT, 1.0f, 1000.0f);// 後で要調整
	}
	else
	{
		m_Projection = XMMatrixPerspectiveFovLH(m_Zoom, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);// 後で要調整
	}
}

void FixedpointCamera::Update()
{
	switch (m_FixedpointType)
	{
	case FIXEDPOINT_TOPVIEW:
		TopUpdate();
		break;
	case FIXEDPOINT_REARVIEW:
		RearUpdate();
		break;
	case FIXEDPOINT_SIDEVIEW_RIGHT:
		SideRightUpdate();
		break;
	case FIXEDPOINT_SIDEVIEW_LEFT:
		SideLeftUpdate();
		break;
	default:
		break;
	}
}

void FixedpointCamera::Draw()
{
	m_View = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_Target), XMLoadFloat3(&m_Up));
}

FixedpointView FixedpointCamera::GetFixedpointView()
{
	return m_FixedpointType;
}

void FixedpointCamera::TopUpdate()
{
	//ドローンの位置を初期化・・・ここでターゲットに視点も併せてるためこの後に位置補正を行ってる
	m_Position.x = m_Target.x + sinf(m_Rotation.y) * cosf(m_Rotation.x) * m_Distance.x;
	m_Position.y = m_Target.y + m_Distance.y;
	m_Position.z = m_Target.z - cosf(m_Rotation.y) * cosf(m_Rotation.x) * (m_Distance.z);//m_Lenge
}

void FixedpointCamera::RearUpdate()
{
	//ドローンの位置を初期化・・・ここでターゲットに視点も併せてるためこの後に位置補正を行ってる
	m_Position.x = m_Target.x + sinf(m_Rotation.y) * cosf(m_Rotation.x) * m_Distance.x;
	m_Position.y = m_Target.y + m_Distance.y;
	m_Position.z = m_Target.z + cosf(m_Rotation.y) * cosf(m_Rotation.x) * (m_Distance.z);//m_Lenge
}

void FixedpointCamera::SideRightUpdate()
{
	//ドローンの位置を初期化・・・ここでターゲットに視点も併せてるためこの後に位置補正を行ってる
	m_Position.x = m_Target.x - cosf(m_Rotation.y) * cosf(m_Rotation.x) * (m_Distance.x);
	m_Position.y = m_Target.y + m_Distance.y;
	m_Position.z = m_Target.z + sinf(m_Rotation.y) * cosf(m_Rotation.x) * (m_Distance.z);//m_Lenge
}

void FixedpointCamera::SideLeftUpdate()
{
	//ドローンの位置を初期化・・・ここでターゲットに視点も併せてるためこの後に位置補正を行ってる
	m_Position.x = m_Target.x + cosf(m_Rotation.y) * cosf(m_Rotation.x) * (m_Distance.x);
	m_Position.y = m_Target.y + m_Distance.y;
	m_Position.z = m_Target.z + sinf(m_Rotation.y) * cosf(m_Rotation.x) * (m_Distance.z);//m_Lenge
}
