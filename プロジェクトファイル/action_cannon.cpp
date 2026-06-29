/*===============================================================


		大砲関係のアクション [action_cannon.cpp]

										Author : shusuke katori
										Date   : 2024/12/11

==================================================================*/

#include "keyboard.h"
#include "player.h"
#include "cannon.h"
#include "ship.h"
#include "manager_ui.h"
#include "ui_gauge_action.h"
#include "camera.h"
#include "controller.h"
#include "cri.h"
#include "manager_animation.h"
#include "action_cannon.h"
#include "client.h"
#include "mode.h"
#include <random>
#include <math.h>


CannonAction::CannonAction(Player* pPlayer) : m_Player(pPlayer)
{

	m_Cannon = nullptr;
	m_State = CANNON_ACTION_STATE_IDOL;

	m_FrameCount = 0.0f;
	m_GaugeCount = 0;


	
}

void CannonAction::Update()
{

	
	
	//状態ごとのアップデート処理
	switch (m_State)
	{
	case CANNON_ACTION_STATE_IDOL:
		ActionReset();
		break;
	case CANNON_ACTION_STATE_DIRECTION_GAUGE: //横軸の着弾点の設定
		DecideEndPoint();
		break;
	case CANNON_ACTION_STATE_SHOT_GAUGE: //縦軸の着弾点の設定
		GaugeAction();
		break;
	case CANNON_ACTION_STATE_SHOT:
		ShotAction();

		
		break;

	default:
		break;
	}

	m_FrameCount += 0.1f;
}

void CannonAction::Draw()
{
}

void CannonAction::UseCannonObject(Cannon* p_cannon)
{
	//if (p_cannon->GetCannonActionFlag() == false) { return; }

	m_Cannon = p_cannon;

	m_State = CANNON_ACTION_STATE_IDOL;

	m_Cannon->SetCannonActionFlag(false);

	/*if (GetGameMode() == GAMEMODE_ONLINE) {
		CannonObject data;
		data.teamid = m_Cannon->GetShip()->GetTeamId();
		data.cannonid = m_Cannon->GetId();
		data.use = USEFLAG_FALSE;
		GetClient()->SendCannonObjectData(data);
	}*/
}


void CannonAction::DecideEndPoint()
{
	m_EndPoint = 30 + (90 * m_Cannon->GetId());

	// 完全乱数
	std::random_device rd;
	m_EndPoint += (rd() % 9) * 15;

	m_State = CANNON_ACTION_STATE_SHOT_GAUGE;
	m_Player->GetManagerUI()->SetUIState(UI_STATE_GAUGE_ACTION);
	
}

void CannonAction::GaugeAction()
{
	
	//0~10で愚案愚案
	if(m_Player->GetShip()->GetTeamId() == 0)
	{
		m_GaugeCount = (1 - cosf(m_FrameCount)) * 5.0f;
	}
	else
	{
		m_GaugeCount = (1 + cosf(m_FrameCount)) * 5.0f;
	}

	
	m_Player->GetManagerUI()->GetGaugeActionUI()->MoveGaugeBar(m_GaugeCount, m_Player->GetShip()->GetTeamId());

	if ((!m_Player->GetController()->IsConnected() && Keyboard_IsKeyTrigger(KK_ENTER)) || (m_Player->GetController()->IsConnected() && m_Player->GetController()->Controller_IsJustPressed(m_Player->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_RIGHT_THUMB))))
	{
		m_State = CANNON_ACTION_STATE_SHOT;
	}
}

void CannonAction::ShotAction()
{
	m_EndPoint += ((int)(m_GaugeCount + 0.5f)) + 2;

	if (GetGameMode() != GAMEMODE_ONLINE) {
		m_Cannon->CreateCannonBall(m_EndPoint, m_Player);
	}
	else {
		CannonData data;
		data.teamid = m_Player->GetShip()->GetTeamId();
		data.playerid = m_Player->GetId();
		data.cannonid = m_Cannon->GetId();
		data.endpoint = m_EndPoint;
		GetClient()->SendCannonBallData(data);
	}

	m_State = CANNON_ACTION_STATE_NONE;
	
	m_Player->SetItemState(ITEMSTATE_NONE);
	m_Player->GetManagerUI()->SetUIState(UI_STATE_NORMAL);
	m_Player->GetAnimationManager()->SetAnimation(ANIMATIONSTATE_SHOT);

}

void CannonAction::ActionReset()
{
	m_State = CANNON_ACTION_STATE_DIRECTION_GAUGE;
	m_FrameCount = 0;
	m_GaugeCount = 0;
}