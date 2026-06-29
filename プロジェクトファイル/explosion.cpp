/*===============================================================


		爆発処理 [explosion.h]

										Author : shusuke katori
										Date   : 2024/12/1

==================================================================*/

#include "collision.h"
#include "ship.h"
#include "deckblock.h"
#include "enhance.h"
#include "player.h"
#include "camera_player.h"
#include "controller.h"
#include "manager_animation.h"
#include "mode.h"
#include "client.h"
#include "explosion.h"

Explosion::Explosion(XMFLOAT3 position, Ship* pShip , Player* p_player) : Object3D(pShip, OBJECTTYPE_EXPLOSION, position) , m_pPlayer(p_player)
{

	//ここで爆発のエフェクト生成
	m_FrameCount = 0;
	m_EndCount = 16;

	switch (GetShip()->GetEnhance()->GetCannonLevel())
	{
	case 0:
		m_Radius = 0.5f;
		m_damage = 50.0f * 10.0f;
		//m_damage = 0.0f;
		break;
	case 1:
		m_Radius = 0.6f;
		m_damage = 60.0f * 10.0f;
		//m_damage = 0.0f;
		break;
	case 2:
		m_Radius = 0.6f;
		//m_damage = 70.0f * 10.0f;
		m_damage = 0.0f;
		break;
	case 3:
		m_Radius = 1.5f;
		m_damage = 80.0f * 10.0f;
		//m_damage = 0.0f;
		break;
	case 4:
		m_Radius = 1.5f;
		m_damage = 90.0f * 10.0f;
		//m_damage = 0.0f;
		break;
	case 5:
		m_Radius = 1.6f;
		m_damage = 100.0f * 10.0f;
		//m_damage = 0.0f;
		break;
	default:
		break;
	}

}

Explosion::~Explosion()
{
}


void Explosion::Update()
{

	if (m_FrameCount >= m_EndCount)
	{
		if (m_pPlayer) {//爆発の終了とともにカメラのリセット
			m_pPlayer->GetCamera()->SetCameraState(CAMERA_STATE_RESET);
		}

		SetDestroyFlag(true);
	}

	//爆発の処理
	if (m_FrameCount == 0)
	{

		for (auto deckblocks : GetShip()->GetEnemyShip()->GetDeckBlocks())
		{

			if (CheckCircleCollider(XMFLOAT2(GetPosition().x, GetPosition().z), XMFLOAT2(deckblocks->GetPosition().x, deckblocks->GetPosition().z), m_Radius, 0.5f))
			{
				if (deckblocks->GetDeckType() == DECKTYPE_WOOD)
				{
					float length = sqrtf(
						((deckblocks->GetPosition().x - GetPosition().x) * (deckblocks->GetPosition().x - GetPosition().x)) +
						((deckblocks->GetPosition().z - GetPosition().z) * (deckblocks->GetPosition().z - GetPosition().z))
					);

					//与ダメージ = 基礎ダメージ - ( 着弾点とブロックの中心点の距離 * ( 基礎ダメージの4分の1 ) )
					if (GetGameMode() != GAMEMODE_ONLINE) {
						deckblocks->Damage(m_damage - (length * (m_damage / 4.0f)));
					}

					//if (!deckblocks->GetUse()) {
						for (auto players : GetShip()->GetEnemyShip()->GetPlayers()) {
							if (CheckBoxCollider(XMFLOAT2(players->GetPosition().x, players->GetPosition().z), XMFLOAT2(deckblocks->GetPosition().x, deckblocks->GetPosition().z), XMFLOAT2(PLAYER_SIZE, PLAYER_SIZE), XMFLOAT2(1.0f, 1.0f))) {
								if (GetGameMode() == GAMEMODE_ONLINE && players->GetId() != GetClient()->GetMyPlayerID()) { continue; }
								players->SetBlowOff();
								if (GetGameMode() != GAMEMODE_ONLINE) {
									players->GetCamera()->SetCameraShake(3.0f);
									players->GetController()->Controller_SetVibration(16000, 16000, 2000);
								}
							}
						}
					//}
					

				}
				
			}
		}

		
	}

	/*if (m_FrameCount == 5)
	{
		if (m_pPlayer) {
			m_pPlayer->GetCamera()->SetCameraState(CAMERA_STATE_RESET);
		}
	}*/

	m_FrameCount++;

	

}

void Explosion::Draw() const
{

}


