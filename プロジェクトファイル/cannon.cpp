/*===============================================================


		大砲クラス [cannon.cpp]

										Author : shusuke katori
										Date   : 2024/11/19

==================================================================*/


#include "cannonball.h"
#include "ship.h"
#include "deckblock.h"
#include "manager_model.h"
#include "player.h"
#include "controller.h"
#include "camera_player.h"
#include "object_billboard.h"
#include "manager_billboard.h"
#include "collision.h"
#include "mode.h"
#include "client.h"
#include "cri.h"
#include "cannon.h"

Cannon::Cannon(XMFLOAT3 pos, Ship* p_ship , int id):Object3D(p_ship,OBJECTTYPE_CANNON,pos), m_Id(id)
{
	if (p_ship->GetTeamId() == 0) {
		SetRotation(XMFLOAT3(0.0f, -90.0f * (3.1415f / 180.0f), 0.0f));
		SetPosition({ GetPosition().x + 1.5f,GetPosition().y + 1.0f,GetPosition().z });
	}
	else if (p_ship->GetTeamId() == 1) {
		SetRotation(XMFLOAT3(0.0f, 90.0f * (3.1415f / 180.0f), 0.0f));
		SetPosition({ GetPosition().x - 1.5f,GetPosition().y + 1.0f,GetPosition().z });
	}

	//アクション中フラグ
	//m_ActionFlag = false;


	m_Endpoint = XMFLOAT3(0.0f, 0.0f, 0.0f);

	if (GetShip()->GetTeamId() == 0)
		m_BillboardObject = new BillboardObject(BILLBOARDTYPE_CANNON, { GetPosition().x - 1.0f, GetPosition().y + 2.0f, GetPosition().z }, { 0.4f, 0.4f, 0.4f });
	else
		m_BillboardObject = new BillboardObject(BILLBOARDTYPE_CANNON, { GetPosition().x + 1.0f, GetPosition().y + 2.0f, GetPosition().z }, { 0.4f, 0.4f, 0.4f });

	BillBoradManagerRegister(m_BillboardObject);

}

Cannon::~Cannon()
{
	
}


//大砲の処理


//大砲のアップデート（砲弾自体の情報）
void Cannon::Update(void)
{
	float ObjectRadius = 4.0f;

	if (m_BillboardObject)
	{
		if(m_ActionFlag)
		{
			m_BillboardObject->SetDrawFlag(false);	//ItemDropを使用した時でもUI描画消す用

			for (auto object : GetShip()->GetPlayers())
			{
				if(object->GetItemState() == ITEMSTATE_CANNON)
				{
					if (CheckCircleCollider({ GetPosition().x, GetPosition().z }, { object->GetPosition().x, object->GetPosition().z }, ObjectRadius, 0.0f)) {
						if (GetGameMode() == GAMEMODE_ONLINE && object->GetId() != GetClient()->GetMyPlayerID()) { continue; }
						m_BillboardObject->SetDrawFlag(true);
						break;
					}
					else {
						m_BillboardObject->SetDrawFlag(false);
					}
				}

			}
		}
		else
		{
			m_BillboardObject->SetDrawFlag(false);
		}
	}

}


//大砲の描画
void Cannon::Draw(void) const
{

	ModelDraw(MODELNAME_CANNON, GetPosition(), GetRotation());
}

void Cannon::CreateCannonBall(int id , Player* p_player)
{
	DeckBlock* p_deckblock = GetShip()->GetEnemyShip()->GetDeckBlocks()[id];


	GetShip()->RegisterCannonBalls(new CannonBall(p_deckblock->GetPosition(), this, GetShip() , p_player));
	if (GetGameMode() == GAMEMODE_ONLINE)
	{
		CreateSource(GetApp3dSE(), GetPosition(), CRI_SE3D_銃火器・大砲);
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			CreateSource(GetApp3dSE(i), GetPosition(), CRI_SE3D_銃火器・大砲);
		}
	}
	
	if (p_player) {
		p_player->GetCamera()->SetCameraShake(0.9f);
		p_player->GetController()->Controller_SetVibration(8000, 8000, 500);
	}

	m_ActionFlag = true;

	if (GetGameMode() == GAMEMODE_ONLINE) {
		CannonObject data;
		data.cannonid = m_Id;
		data.teamid = GetShip()->GetTeamId();
		data.use = USEFLAG_TRUE;
		GetClient()->SendCannonObjectData(data);
	}

}




//セッター

void Cannon::SetCannonActionFlag(const bool flag)
{
	m_ActionFlag = flag;
}

//ゲッター

bool Cannon::GetCannonActionFlag(void)
{
	return m_ActionFlag;
}

int Cannon::GetId()
{
	return m_Id;
}

void Cannon::SetId(int id)
{
	m_Id = id;
}

void Cannon::Reset()
{
	m_ActionFlag = true;
}



