//********************************************************************************
//
// プレイヤーのカメラクラス[camera_player.h]
//
//															Author : Riugo Honda
//															Date   :2025/02/16
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/16
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


#include "camera_player.h"

PlayerCamera::PlayerCamera(XMFLOAT3 pos, XMFLOAT3 target)
{
	m_Position = pos;
	m_Target = target;

	m_Up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Lenge = 5.0f;
	m_Zoom = 1.0f;

	m_State = CAMERA_STATE_NORMAL;
}

PlayerCamera::PlayerCamera(XMFLOAT3 pos, XMFLOAT3 target, Ship* p_ship, Player* p_player, GAMEMODE mode) :m_pShip(p_ship), m_pPlayer(p_player)
{
	m_Position = pos;
	m_Target = target;

	m_Up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Lenge = 5.0f;
	m_Zoom = 1.0f;

	m_State = CAMERA_STATE_NORMAL;

	m_Target = m_pPlayer->GetPosition();

	m_Position.x = m_Target.x + sinf(m_Rotation.y) * cosf(m_Rotation.x) * m_Lenge;
	m_Position.y = m_Target.y + m_Lenge;
	m_Position.z = m_Target.z - cosf(m_Rotation.y) * cosf(m_Rotation.x) * (m_Lenge);

	m_Framecount = 0;
	m_PlayerMoveFlag = false;

	m_GameMode = mode;

	/*m_Interpolation = 1.0f;

	m_OldPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);*/

	if (mode == GAMEMODE_SOLO)
	{
		m_Projection = XMMatrixPerspectiveFovLH(m_Zoom, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);// 後で要調整
	}
	if (mode == GAMEMODE_LOCAL_SPLITSCREEN)
	{
		m_Projection = XMMatrixPerspectiveFovLH(m_Zoom, (float)SCREEN_WIDTH * 0.5f / SCREEN_HEIGHT, 1.0f, 1000.0f);// 後で要調整
	}
	if (mode == GAMEMODE_LOCAL_MULTIWINDOW)
	{
		m_Projection = XMMatrixPerspectiveFovLH(m_Zoom, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);// 後で要調整
	}
	if (mode == GAMEMODE_SEGMENTATION)
	{
		m_Projection = XMMatrixPerspectiveFovLH(m_Zoom, (float)SCREEN_WIDTH * 0.5f / SCREEN_HEIGHT, 1.0f, 1000.0f);// 後で要調整
	}
	if (mode == GAMEMODE_MULTIWINDOW)
	{
		m_Projection = XMMatrixPerspectiveFovLH(m_Zoom, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);// 後で要調整
	}

}

PlayerCamera::~PlayerCamera()
{
}

//void PlayerCamera::Update(int team)
void PlayerCamera::Update()
{

	//カメラのステートによってカメラのアプデ変更
	if (m_State == CAMERA_STATE_NORMAL || m_State == CAMERA_STATE_BLOWOFF)
	{
		m_TargetSlow = m_pPlayer->GetPosition();


		//メモ：カメラががくんと動く原因はm_positionSlowからm_Positionに切り替えてるためであり
		//この書き方に修正すると治る。
		m_PositionSlow.x = m_TargetSlow.x + sinf(m_Rotation.y) * cosf(m_Rotation.x) * m_Lenge;
		m_PositionSlow.y = m_TargetSlow.y + m_Lenge;
		m_PositionSlow.z = m_TargetSlow.z - cosf(m_Rotation.y) * cosf(m_Rotation.x) * (m_Lenge);//m_Lenge
	}

	if (m_State == CAMERA_STATE_RESET)
	{
		m_Target.x += (m_TargetSlow.x - m_Target.x) * 0.07f;
		m_Target.y += (m_TargetSlow.y - m_Target.y) * 0.07f;
		m_Target.z += (m_TargetSlow.z - m_Target.z) * 0.07f;
	}
	else
	{
		m_Target.x += (m_TargetSlow.x - m_Target.x) * 0.05f;
		m_Target.y += (m_TargetSlow.y - m_Target.y) * 0.05f;
		m_Target.z += (m_TargetSlow.z - m_Target.z) * 0.05f;
	}

	//カメラのシェイク・・・2025/1/5 (仮）
	m_Target.y += m_Shake * cosf(m_ShakeTime);
	m_ShakeTime += XM_2PI / 3.0f; //（シェイクの速さ決め）
	m_Shake *= 0.9f;			  //シェイクの長さ決め

	m_Position.x += (m_PositionSlow.x - m_Position.x) * 0.05f;
	m_Position.y += (m_PositionSlow.y - m_Position.y) * 0.05f;
	m_Position.z += (m_PositionSlow.z - m_Position.z) * 0.05f;

	if (m_State == CAMERA_STATE_RESET)
	{
		IsCameraMoving();
	}

	if (m_PlayerMoveFlag)
	{
		if (m_Framecount >= 35)
		{
			m_PlayerMoveFlag = false;

			m_State = CAMERA_STATE_NORMAL;

			if (m_pPlayer->GetAction() != ACTIONSTATE_BLOWOFF)
				m_pPlayer->SetAction(ACTIONSTATE_IDLE);
		}

		m_Framecount++;
	}

}

void PlayerCamera::Draw()
{
	m_View = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_Target), XMLoadFloat3(&m_Up));
}



void PlayerCamera::SetCameraState(CameraState state)
{
	m_OldState = m_State;

	m_State = state;

	switch (m_State)
	{
	case CAMERA_STATE_NORMAL:
		m_Rotation.y = 0.0f;

		SetTarget(m_pPlayer->GetPosition());
		break;
	case CAMERA_STATE_ACTION_GATHER:
		if (m_pShip->GetTeamId() == 0)
		{
			/*m_OldPosition = m_Position;
			m_Interpolation = 0.0f;*/
			if (m_GameMode == GAMEMODE_SOLO || m_GameMode == GAMEMODE_LOCAL_MULTIWINDOW || m_GameMode == GAMEMODE_MULTIWINDOW)
			{
				//真ん中のバリスタを中心に漂流全体を見渡すカメラ配置
				m_TargetSlow.x = -33.0f; // m_pPlayer->GetPosition().x
				m_TargetSlow.y = 18.0f; //1.0f m_pPlayer->GetPosition().y
				m_TargetSlow.z = 0.0f;	//0.00 m_pPlayer->GetPosition().z

				m_PositionSlow.x = m_TargetSlow.x + 3.0f;
				m_PositionSlow.y = m_TargetSlow.y + 5.0f;
				m_PositionSlow.z = m_TargetSlow.z;
			}
			else
			{
				//スプリットウィンドウのため自分の収集範囲を表示する
				m_TargetSlow.x = m_pPlayer->GetPosition().x;	// 
				m_TargetSlow.y = m_pPlayer->GetPosition().y + 18.0f;	//
				m_TargetSlow.z = m_pPlayer->GetPosition().z;	//

				m_PositionSlow.x = m_pPlayer->GetPosition().x + 2.0f;
				m_PositionSlow.y = m_pPlayer->GetPosition().y + 23.0f;
				m_PositionSlow.z = m_pPlayer->GetPosition().z;
			}

		}
		if (m_pShip->GetTeamId() == 1)
		{
			/*m_OldPosition = m_Position;
			m_Interpolation = 0.0f;*/
			if (m_GameMode == GAMEMODE_SOLO || m_GameMode == GAMEMODE_LOCAL_MULTIWINDOW || m_GameMode == GAMEMODE_MULTIWINDOW)
			{
				//真ん中のバリスタを中心に漂流全体を見渡すカメラ配置
				m_TargetSlow.x = 33.0f;
				m_TargetSlow.y = 18.0f;
				m_TargetSlow.z = 0.0f;

				m_PositionSlow.x = m_TargetSlow.x - 3.0f;
				m_PositionSlow.y = m_TargetSlow.y + 5.0f;
				m_PositionSlow.z = m_TargetSlow.z;
			}
			else
			{
				//スプリットウィンドウのため自分の収集範囲を表示する
				m_TargetSlow.x = m_pPlayer->GetPosition().x;	// 
				m_TargetSlow.y = m_pPlayer->GetPosition().y + 18.0f;	//
				m_TargetSlow.z = m_pPlayer->GetPosition().z;	//

				m_PositionSlow.x = m_pPlayer->GetPosition().x - 2.0f;
				m_PositionSlow.y = m_pPlayer->GetPosition().y + 23.0f;
				m_PositionSlow.z = m_pPlayer->GetPosition().z;
			}

		}

		break;
	case CAMERA_STATE_ACTION_CANNON:
		if (m_pShip->GetTeamId() == 0)
		{
			/*m_OldPosition = m_Position;
			m_Interpolation = 0.0f;*/

			m_TargetSlow.x = m_pPlayer->GetPosition().x + 5.0f;
			m_TargetSlow.y = m_pPlayer->GetPosition().y;
			m_TargetSlow.z = m_pPlayer->GetPosition().z;

			m_PositionSlow.x = m_pPlayer->GetPosition().x - 4.0f;
			m_PositionSlow.y = m_pPlayer->GetPosition().y + 2.0f;
			m_PositionSlow.z = (m_pPlayer->GetPosition().z) - 3.5f;
		}
		if (m_pShip->GetTeamId() == 1)
		{
			/*m_OldPosition = m_Position;
			m_Interpolation = 0.0f;*/

			m_TargetSlow.x = m_pPlayer->GetPosition().x - 5.0f;
			m_TargetSlow.y = m_pPlayer->GetPosition().y;
			m_TargetSlow.z = m_pPlayer->GetPosition().z;

			m_PositionSlow.x = m_pPlayer->GetPosition().x + 4.0f;
			m_PositionSlow.y = m_pPlayer->GetPosition().y + 2.0f;
			m_PositionSlow.z = (m_pPlayer->GetPosition().z) - 3.5f;
		}

		break;
	case CAMERA_STATE_RESET:
		/*m_OldPosition = m_Position;
		m_Interpolation = 0.0f;*/
		//m_pPlayer->SetAction(ACTIONSTATE_IDOL);
		m_Framecount = 0;
		m_PlayerMoveFlag = true;

		m_TargetSlow.x = m_pPlayer->GetPosition().x;
		m_TargetSlow.y = m_pPlayer->GetPosition().y;
		m_TargetSlow.z = m_pPlayer->GetPosition().z;

		m_PositionSlow.x = m_TargetSlow.x + sinf(m_Rotation.y) * cosf(m_Rotation.x) * m_Lenge;
		m_PositionSlow.y = m_TargetSlow.y + m_Lenge;
		m_PositionSlow.z = m_TargetSlow.z - cosf(m_Rotation.y) * cosf(m_Rotation.x) * (m_Lenge);//m_Lenge
		break;
	case CAMERA_STATE_EVENT:
		if (m_pShip->GetTeamId() == 0)
		{

		}
		if (m_pShip->GetTeamId() == 1)
		{

		}
		break;
	case CAMERA_STATE_BLOWOFF:
	{
		m_TargetSlow.x = m_pPlayer->GetPosition().x;
		m_TargetSlow.y = m_pPlayer->GetPosition().y;
		m_TargetSlow.z = m_pPlayer->GetPosition().z;

		m_PositionSlow.x = m_TargetSlow.x + sinf(m_Rotation.y) * cosf(m_Rotation.x) * m_Lenge;
		m_PositionSlow.y = m_TargetSlow.y + m_Lenge;
		m_PositionSlow.z = m_TargetSlow.z - cosf(m_Rotation.y) * cosf(m_Rotation.x) * (m_Lenge);//m_Lenge
	}
	default:
		break;
	}

}

void PlayerCamera::SetTarget(XMFLOAT3 target)
{
	m_Target = target;
}

void PlayerCamera::SetCameraShake(float shake)
{
	m_Shake = shake;

	m_ShakeTime = 0.0f;
}

XMFLOAT3 PlayerCamera::GetFrontDirection()
{
	XMFLOAT3 forward_vector;
	forward_vector.x = m_Target.x - m_Position.x;
	forward_vector.y = m_Target.y - m_Position.y;
	forward_vector.z = m_Target.z - m_Position.z;

	float x = sqrtf(forward_vector.x * forward_vector.x + forward_vector.y * forward_vector.y + forward_vector.z * forward_vector.z);

	forward_vector.x /= x;
	forward_vector.y /= x;
	forward_vector.z /= x;

	return forward_vector;
}

XMFLOAT3 PlayerCamera::GetRightDirection()
{
	XMFLOAT3 cameraRight;
	cameraRight.x = m_Up.y * GetFrontDirection().z - m_Up.z * GetFrontDirection().y;
	cameraRight.y = m_Up.z * GetFrontDirection().x - m_Up.x * GetFrontDirection().z;
	cameraRight.z = m_Up.x * GetFrontDirection().y - m_Up.y * GetFrontDirection().x;

	float x = sqrtf(cameraRight.x * cameraRight.x + cameraRight.y * cameraRight.y + cameraRight.z * cameraRight.z);

	cameraRight.x /= x;
	cameraRight.y /= x;
	cameraRight.z /= x;


	return cameraRight;
}

void PlayerCamera::IsCameraMoving()
{

	if (m_OldState == CAMERA_STATE_ACTION_GATHER)
	{
		float Target_checker_x = m_TargetSlow.x - m_Target.x;
		float Target_checker_y = m_TargetSlow.y - m_Target.y;
		float Target_checker_z = m_TargetSlow.z - m_Target.z;

		//値要調整 xzパターン管理
		if ((0 <= Target_checker_x && Target_checker_x < 0.1f) || (-0.1f < Target_checker_x && Target_checker_x <= 0))
		{
			if ((0 <= Target_checker_y && Target_checker_y < 0.1f) || (-0.1f < Target_checker_y && Target_checker_y <= 0))
			{
				if ((0 <= Target_checker_z && Target_checker_z < 0.1f) || (-0.1f < Target_checker_z && Target_checker_z <= 0))
				{
					m_State = CAMERA_STATE_NORMAL;

					//５８フレでここに入る
					m_pPlayer->SetAction(ACTIONSTATE_IDLE);
				}
			}
		}
	}

	if (m_OldState == CAMERA_STATE_ACTION_CANNON)
	{
		float position_checker_x = m_PositionSlow.x - m_Position.x;
		float position_checker_y = m_PositionSlow.y - m_Position.y;
		float position_checker_z = m_PositionSlow.z - m_Position.z;

		if ((0.0f <= position_checker_x && position_checker_x < 0.5f) || (-0.5f < position_checker_x && position_checker_x <= 0.0f))
		{
			if ((0.0f <= position_checker_y && position_checker_y < 0.5f) || (-0.5f < position_checker_y && position_checker_y <= 0.0f))
			{
				if ((0.0f < position_checker_z && position_checker_z < 0.5f) || (-0.5f < position_checker_z && position_checker_z <= 0.0f))
				{
					m_State = CAMERA_STATE_NORMAL;

					m_pPlayer->SetAction(ACTIONSTATE_IDLE);
				}
			}
		}
	}
}

void PlayerCamera::CameraRotation()
{
	if (Keyboard_IsKeyDown(KK_LEFT) || (m_pPlayer->GetController()->IsConnected() && m_pPlayer->GetController()->Controller_RightStickIsLeft())) {
		m_Rotation.y -= 0.1f;
	}
	if (Keyboard_IsKeyDown(KK_RIGHT) || (m_pPlayer->GetController()->IsConnected() && m_pPlayer->GetController()->Controller_RightStickIsRight())) {
		m_Rotation.y += 0.1f;
	}

	//カメラのローテーションリセット
	if (Keyboard_IsKeyTrigger(KK_UP) || (m_pPlayer->GetController()->IsConnected() && m_pPlayer->GetController()->Controller_IsJustPressed(m_pPlayer->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_LEFT_SHOULDER))))
	{
		m_Rotation.y = 0.0f;
	}
}
