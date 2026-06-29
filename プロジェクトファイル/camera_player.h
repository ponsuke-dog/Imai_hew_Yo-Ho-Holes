//********************************************************************************
//
// プレイヤーのカメラクラス[camera_player.h]
//
//															Author : Riugo Honda
//															Date   :2025/02/16
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/16
//********************************************************************************
#pragma once
 

#ifndef _CAMERA_PLAYER_H_
#define _CAMERA_PLAYER_H_
#include "configuration.h"
#include "camera.h"

class Ship;
class Player;

typedef enum
{
	CAMERA_STATE_NORMAL,
	CAMERA_STATE_ACTION_GATHER,
	CAMERA_STATE_ACTION_CANNON,
	CAMERA_STATE_RESET,
	CAMERA_STATE_EVENT,
	CAMERA_STATE_BLOWOFF,
	CAMERA_STATE_MAX
}CameraState;



class PlayerCamera:public Camera
{
private:

	XMFLOAT3 m_TargetSlow;
	XMFLOAT3 m_PositionSlow;


	float m_Lenge;
	float m_Zoom;

	//ポインター
	Ship* m_pShip = nullptr;
	Player* m_pPlayer = nullptr;

	//カメラのステート
	CameraState m_State;
	CameraState m_OldState;

	//カメラのシェイク用
	float m_Shake = 0.0f;
	float m_ShakeTime = 0.0f;

	//プレイヤーの動き出しを設定
	int m_Framecount;
	bool m_PlayerMoveFlag;

	GAMEMODE m_GameMode;

public:

	PlayerCamera(XMFLOAT3 pos, XMFLOAT3 target);
	PlayerCamera(XMFLOAT3 pos, XMFLOAT3 target, Ship* p_ship, Player* p_player, GAMEMODE mode);
	~PlayerCamera();

	//void Update(int team);
	void Update() override;
	void Draw()override;

	//カメラのステート変更
	void SetCameraState(CameraState state);

	void SetTarget(XMFLOAT3 target);

	void SetCameraShake(float shake);

	XMFLOAT3 GetFrontDirection();
	XMFLOAT3 GetRightDirection();

private:
	//カメラの動きが終了してるかのチェック処理
	void IsCameraMoving();

	//カメラの回転
	void CameraRotation();

};
#endif // !_CAMERA_PLAYER_H_
