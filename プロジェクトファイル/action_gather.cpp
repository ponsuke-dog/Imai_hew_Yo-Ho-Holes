//********************************************************************************
//
// 収集アクションのクラス[action_gather.h] 
// 
//															Author : Riugo Honda
//															Date   :2024/12/12
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/12
//********************************************************************************
#include "directx.h"
#include "mode.h"
#include "cri.h"
#include "gather.h"
#include "aim.h"
#include "player.h"
#include "controller.h"
#include "keyboard.h"
#include "camera_player.h"
#include "math.h"
#include "manager_ui.h"
#include "ui_gather.h"
#include "mode.h"
#include "ship.h"
#include "client.h"
#include "action_gather.h"

GatherAction::GatherAction(Player* p_player)
	:m_pPlayer(p_player)
{
	
}

GatherAction::~GatherAction()
{
}

void GatherAction::Update()
{
	AimAction();

}

void GatherAction::Draw() const
{

	if (m_pAim != nullptr)
	{
		m_pAim->Draw();
	}

}

void GatherAction::AimAction()
{
	XMFLOAT3 movevector = { 0.0f, 0.0f, 0.0f };

	float lx = 0.0f;
	float ly = 0.0f;

	if (m_pPlayer->GetController()->IsConnected())
	{
		lx = m_pPlayer->GetController()->GetLeftStickX();
		ly = m_pPlayer->GetController()->GetLeftStickY();
	}
	if (!m_pPlayer->GetController()->IsConnected()) {
		//加速
		if (Keyboard_IsKeyDown(KK_A))
		{
			lx += -1.0f;
		}
		if (Keyboard_IsKeyDown(KK_D))
		{
			lx += 1.0f;
		}
		if (Keyboard_IsKeyDown(KK_W))
		{
			ly += 1.0f;
		}
		if (Keyboard_IsKeyDown(KK_S))
		{
			ly += -1.0f;
		}
	}

	if (lx != 0 || ly != 0)
	{
		XMFLOAT3 forwardDir = m_pPlayer->GetCamera()->GetFrontDirection();
		XMFLOAT3 RightDir = m_pPlayer->GetCamera()->GetRightDirection();

		movevector.x = forwardDir.x * ly + RightDir.x * lx;
		movevector.y = 0;
		movevector.z = forwardDir.z * ly + RightDir.z * lx;

		float x = sqrtf(movevector.x * movevector.x + movevector.z * movevector.z);

		movevector.x /= x;
		movevector.z /= x;
	}

	if ((!m_pPlayer->GetController()->IsConnected() && Keyboard_IsKeyTrigger(KK_ENTER)) || (m_pPlayer->GetController()->IsConnected() && m_pPlayer->GetController()->Controller_IsJustPressed(m_pPlayer->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_RIGHT_THUMB))))
	{
		if (m_pAim == nullptr) { return; }

		SetNet(m_pPlayer);

		if (GetGameMode() == GAMEMODE_ONLINE)
		{
			CreateSource(GetApp3dSE(), m_pGather->GetPosition(), CRI_SE3D_ネット発射);
		}
		else
		{

			for (int i = 0; i < 4; i++)
			{
				CreateSource(GetApp3dSE(i), m_pGather->GetPosition(), CRI_SE3D_ネット発射);
			}
		}
		
		m_pPlayer->GetCamera()->SetCameraShake(0.5);
		m_pPlayer->GetController()->Controller_SetVibration(4000, 4000, 500);
	}

	if (m_pAim != nullptr)
	{
		m_pAim->AimMove(movevector);
	}
}

void GatherAction::UseGatherObject(Gather* pgather)
{
	m_pGather = pgather;
	m_pAim = new Aim(XMFLOAT3(m_pGather->GetPosition().x, 0.0f, m_pGather->GetPosition().z), m_pGather, m_pGather->GetShip());
	m_pGather->SetUsingGather(false);

	if (GetGameMode() == GAMEMODE_ONLINE) {
		/*GatherObject data;
		data.teamid = m_pGather->GetShip()->GetTeamId();
		data.gatherid = m_pGather->GetId();
		data.use = USEFLAG_FALSE;
		GetClient()->SendGatherObjectData(data);*/
	}

	m_pPlayer->GetManagerUI()->SetUIState(UI_STATE_GATHER_ACTION);
	/*if (m_pGather->GetUsingGather())
	{
		m_pAim = new Aim(XMFLOAT3(m_pGather->GetPosition().x, 0.0f, m_pGather->GetPosition().z), m_pGather, m_pGather->GetShip());

	}
	else
	{
		m_pPlayer->SetAction(ACTIONSTATE_IDLE);

		m_pPlayer->GetCamera()->SetCameraState(CAMERA_STATE_RESET);
		
	}*/

}



void GatherAction::SetNet(Player* p_player)
{
	if (m_pAim == nullptr) { return; }

	if (m_pGather == nullptr) { return; }

	m_pGather->SetNet(m_pAim->GetPosition(), m_pAim->GetSize() , p_player);
	m_pPlayer->GetManagerUI()->SetUIState(UI_STATE_NORMAL);

	delete m_pAim;
	m_pAim = nullptr;
}

void GatherAction::AimCancel()
{
	if (m_pAim == nullptr) { return; }

	delete m_pAim;
	m_pAim = nullptr;

}

