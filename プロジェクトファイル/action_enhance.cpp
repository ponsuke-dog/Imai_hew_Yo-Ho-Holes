//********************************************************************************
//
// 強化アクション[action_enhance.cpp] 
// 
//															Author : Hiroki Nakajima
//															Date   : 2024/12/12
// -------------------------------------------------------------------------------
//															Last Edited:2025/01/17
//********************************************************************************
#include "sprite.h"
#include "texture.h"
#include "configuration.h"
#include "keyboard.h"
#include "controller.h"
#include "enhance.h"
#include "ui_enhance.h"
#include "manager_ui.h"
#include "ship.h"
#include "player.h"
#include "cri.h"
#include "mode.h"
#include "client.h"
#include "action_enhance.h"

//===================================================
// コンストラクタ
//===================================================
EnhanceAction::EnhanceAction(Player* player) : m_Player(player)
{
	m_EnhanceUI = m_Player->GetManagerUI()->GetEnhanceUI();
}

//===================================================
// デストラクタ
//===================================================
EnhanceAction::~EnhanceAction()
{
}

//===================================================
// 更新
//===================================================
void EnhanceAction::Update()
{
	static constexpr int LEVEL_MAX = 5; //最大レベル

	//Tabで強化画面を開く、閉じる
	if ((!m_Player->GetController()->IsConnected() &&Keyboard_IsKeyTrigger(KK_TAB)) || (m_Player->GetController()->IsConnected() && m_Player->GetController()->Controller_IsJustPressed(m_Player->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_START))))
	{
		m_EnhanceUI->SwitchDrawFlag();
		m_Player->SetAction(ACTIONSTATE_IDLE);
		m_Player->GetManagerUI()->SetUIState(UI_STATE_NORMAL);
		m_EnhanceUI->GetEnhance()->SetUsingEnhance(true);

		if (GetGameMode() == GAMEMODE_ONLINE) {
			EnhanceObject data;
			data.teamid = m_Player->GetShip()->GetTeamId();
			data.use = USEFLAG_TRUE;
			data.stock = m_Player->GetShip()->GetEnhance()->GetStock();
			data.cannonlevel = m_Player->GetShip()->GetEnhance()->GetCannonLevel();
			data.gatherlevel = m_Player->GetShip()->GetEnhance()->GetGatherLevel();
			data.repairlevel = m_Player->GetShip()->GetEnhance()->GetRepairLevel();
			GetClient()->SendEnhanceObjectData(data);
		}

		SEStart(GetAppSE(), CRI_SE_キャンセルボタン);
	}

	if (m_EnhanceUI->GetInterface() == 0)
	{	//強化画面1
		if ((!m_Player->GetController()->IsConnected() && Keyboard_IsKeyTrigger(KK_A)) || (m_Player->GetController()->IsConnected() && m_Player->GetController()->Controller_IsJustPressed(m_Player->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_DPAD_LEFT)))) //1で選択カーソル左移動
			m_EnhanceUI->MoveSelect(0);
		if ((!m_Player->GetController()->IsConnected() && Keyboard_IsKeyTrigger(KK_D)) || (m_Player->GetController()->IsConnected() && m_Player->GetController()->Controller_IsJustPressed(m_Player->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_DPAD_RIGHT)))) //2で右移動
			m_EnhanceUI->MoveSelect(1);
		if ((!m_Player->GetController()->IsConnected() && Keyboard_IsKeyTrigger(KK_ENTER)) || (m_Player->GetController()->IsConnected() && m_Player->GetController()->Controller_IsJustPressed(m_Player->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_B)))) //Enterで決定、強化画面2へ
		{
			m_EnhanceUI->SetInterface(1);
			SEStart(GetAppSE(), CRI_SE_クリック音);
		}
		if ((!m_Player->GetController()->IsConnected() && Keyboard_IsKeyTrigger(KK_Q)) || (m_Player->GetController()->IsConnected() && m_Player->GetController()->Controller_IsJustPressed(m_Player->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_A)))) //Deleteで強化画面1に戻る
		{
			m_EnhanceUI->SwitchDrawFlag();
			m_Player->SetAction(ACTIONSTATE_IDLE);
			m_Player->GetManagerUI()->SetUIState(UI_STATE_NORMAL);
			m_EnhanceUI->GetEnhance()->SetUsingEnhance(true);

			if (GetGameMode() == GAMEMODE_ONLINE) {
				EnhanceObject data;
				data.teamid = m_Player->GetShip()->GetTeamId();
				data.use = USEFLAG_TRUE;
				data.stock = m_Player->GetShip()->GetEnhance()->GetStock();
				data.cannonlevel = m_Player->GetShip()->GetEnhance()->GetCannonLevel();
				data.gatherlevel = m_Player->GetShip()->GetEnhance()->GetGatherLevel();
				data.repairlevel = m_Player->GetShip()->GetEnhance()->GetRepairLevel();
				GetClient()->SendEnhanceObjectData(data);
			}

			SEStart(GetAppSE(), CRI_SE_キャンセルボタン);

		}
	}
	else if (m_EnhanceUI->GetInterface() == 1)
	{	//強化画面2
		if ((!m_Player->GetController()->IsConnected() && Keyboard_IsKeyTrigger(KK_Q)) || (m_Player->GetController()->IsConnected() && m_Player->GetController()->Controller_IsJustPressed(m_Player->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_A)))) //Deleteで強化画面1に戻る
		{
			m_EnhanceUI->SetInterface(0);
			SEStart(GetAppSE(), CRI_SE_キャンセルボタン);
		}

		//Enterで強化
		if ((!m_Player->GetController()->IsConnected() && Keyboard_IsKeyTrigger(KK_ENTER)) || (m_Player->GetController()->IsConnected() && m_Player->GetController()->Controller_IsJustPressed(m_Player->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_B))))
		{
			switch (m_EnhanceUI->GetSelect())
			{
			case ENHANCETYPE_GATHER: //回収強化
				if (CheckLevel(m_EnhanceUI->GetEnhance()->GetGatherLevel()) && m_EnhanceUI->GetEnhance()->GetGatherLevel() < LEVEL_MAX) {
					m_EnhanceUI->GetEnhance()->SetGatherLevel();

					if (GetGameMode() == GAMEMODE_ONLINE) {
						EnhanceObject data;
						data.teamid = m_Player->GetShip()->GetTeamId();
						data.use = USEFLAG_FALSE;
						data.stock = m_Player->GetShip()->GetEnhance()->GetStock();
						data.cannonlevel = m_Player->GetShip()->GetEnhance()->GetCannonLevel();
						data.gatherlevel = m_Player->GetShip()->GetEnhance()->GetGatherLevel();
						data.repairlevel = m_Player->GetShip()->GetEnhance()->GetRepairLevel();
						GetClient()->SendEnhanceObjectData(data);
					}

					SEStart(GetAppSE(), CRI_SE_パワーアップ);
				}
				break;
			case ENHANCETYPE_CANNON:  //大砲強化
				if (CheckLevel(m_EnhanceUI->GetEnhance()->GetCannonLevel()) && m_EnhanceUI->GetEnhance()->GetCannonLevel() < LEVEL_MAX){
					m_EnhanceUI->GetEnhance()->SetCannonLevel();

					if (GetGameMode() == GAMEMODE_ONLINE) {
						EnhanceObject data;
						data.teamid = m_Player->GetShip()->GetTeamId();
						data.use = USEFLAG_FALSE;
						data.stock = m_Player->GetShip()->GetEnhance()->GetStock();
						data.cannonlevel = m_Player->GetShip()->GetEnhance()->GetCannonLevel();
						data.gatherlevel = m_Player->GetShip()->GetEnhance()->GetGatherLevel();
						data.repairlevel = m_Player->GetShip()->GetEnhance()->GetRepairLevel();
						GetClient()->SendEnhanceObjectData(data);
					}

					SEStart(GetAppSE(), CRI_SE_パワーアップ);
				}
				break;
			case ENHANCETYPE_REPAIR:  //修復強化
				if (CheckLevel(m_EnhanceUI->GetEnhance()->GetRepairLevel()) && m_EnhanceUI->GetEnhance()->GetRepairLevel() < LEVEL_MAX) {
					m_EnhanceUI->GetEnhance()->SetRepairLevel();

					if (GetGameMode() == GAMEMODE_ONLINE) {
						EnhanceObject data;
						data.teamid = m_Player->GetShip()->GetTeamId();
						data.use = USEFLAG_FALSE;
						data.stock = m_Player->GetShip()->GetEnhance()->GetStock();
						data.cannonlevel = m_Player->GetShip()->GetEnhance()->GetCannonLevel();
						data.gatherlevel = m_Player->GetShip()->GetEnhance()->GetGatherLevel();
						data.repairlevel = m_Player->GetShip()->GetEnhance()->GetRepairLevel();
						GetClient()->SendEnhanceObjectData(data);
					}

					SEStart(GetAppSE(), CRI_SE_パワーアップ);
				}
				break;
			default:
				break;
			}

			
		}
	}
}

//===================================================
// 描画
//===================================================
void EnhanceAction::Draw()
{
}

//===================================================
// レベルのチェック
//===================================================
bool EnhanceAction::CheckLevel(int level)
{
	static constexpr int FUEL_QUANTITY[5] = { 2, 3, 5, 7, 10 }; //レベルごとの強化素材必要量

 	if (m_EnhanceUI->GetEnhance()->GetStock() >= FUEL_QUANTITY[level])
	{	//強化素材を必要量以上所持していたら強化
		m_EnhanceUI->GetEnhance()->DecreaseStock(FUEL_QUANTITY[level]); //強化素材をレベル毎の必要量分減らす
		return true;
	}

	return false;
}
