//********************************************************************************
//
// 修理アクションのクラス[action_repair.cpp] 
// 
//															Author : Riugo Honda
//															Date   :2024/12/29
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/29
//********************************************************************************
#include "directx.h"
#include "cri.h"
#include "mode.h"
#include "player.h"
#include "deckblock.h"
#include "enhance.h"
#include "effect_3d.h"
#include "manager_effect.h"
#include "ship.h"
#include "client.h"
#include "action_repair.h"

RepairAction::RepairAction(Player* p_player, Enhance* p_enhance)
	:m_pPlayer(p_player) , m_pEnhance(p_enhance)
{
	//m_Effect = nullptr;
}

RepairAction::~RepairAction()
{
	
}

void RepairAction::Update()
{
	if (m_FrameCount > 60)
	{
		//60フレ（１秒後）のアクション終了
		m_pPlayer->SetAction(ACTIONSTATE_IDLE);

		//修復の回数が０以下になったらアイテムの保持状況をなしにする
 		if (m_RepairCount <= 0)
		{
			m_pPlayer->SetItemState(ITEMSTATE_NONE);
		}

		/*if (m_Effect) {
			m_Effect->SetDestroyFlag(true);
			m_Effect = nullptr;
		}*/

		m_FrameCount = 0;

		return;
	}

	m_FrameCount++;
}

void RepairAction::SetRepairBlock(DeckBlock* p_DeckBlock)
{
	//ここで修復行現在はブロックHPをいじってない。

	if (GetGameMode() == GAMEMODE_ONLINE)
	{
		/*BlockData data;
		data.blocknumber = p_DeckBlock->GetId();
		data.teamid = p_DeckBlock->GetShip()->GetTeamId();
		data.state = BLOCKSTATE_REPAIR;

		GetClient()->SendBlockData(data);*/


		CreateSource(GetApp3dSE(), p_DeckBlock->GetPosition(), CRI_SE3D_修復);
	}
	else
	{
		p_DeckBlock->Repair();
		for (int i = 0; i < 4; i++)
		{
			CreateSource(GetApp3dSE(i), p_DeckBlock->GetPosition(), CRI_SE3D_修復);
		}
		
		EffectManagerRegister(new Effect3D(EFFECTTYPE_REPAIR, { p_DeckBlock->GetPosition().x, p_DeckBlock->GetPosition().y + 1.5f, p_DeckBlock->GetPosition().z }, { 2.5f, 2.5f, 2.5f }, GetEffectFrameMaxNum(EFFECTTYPE_REPAIR), EFFECT3DTYPE_NORMAL));
		EffectManagerRegister(new Effect3D(EFFECTTYPE_CLUNK_MOJI, { p_DeckBlock->GetPosition().x, p_DeckBlock->GetPosition().y + 1.5f, p_DeckBlock->GetPosition().z }, { 3.0f, 3.0f, 3.0f }, GetEffectFrameMaxNum(EFFECTTYPE_CLUNK_MOJI), EFFECT3DTYPE_NORMAL));
	}
	m_FrameCount = 0;
	m_RepairCount--;
}

void RepairAction::SetRepairCount()
{
	//ここで強化段階によって修復回数のセット
	switch (m_pEnhance->GetRepairLevel())
	{
	case 0:
		m_RepairCount = 1;
		break;
	case 1:
		m_RepairCount = 2;
		break;
	case 2:
		m_RepairCount = 2;
		break;
	case 3:
		m_RepairCount = 3;
		break;
	case 4:
		m_RepairCount = 3;
		break;
	case 5:
		m_RepairCount = 4;
		break;
	}
}

void RepairAction::ResetRepairCount()
{
	//一応の修復回数の初期化
	m_RepairCount = 0;
}

int RepairAction::GetRepairCount()
{
	return m_RepairCount;
}

Enhance* RepairAction::GetEnhance()
{
	return m_pEnhance;
}
