//********************************************************************************
//
// 収集のクラス[gather.h]
//
//															Author : Riugo Honda
//															Date   :2024/11/22
// -------------------------------------------------------------------------------
//															Last Edited:2024/11/24
//********************************************************************************

#include "net.h"
#include "manager_model.h"
#include "ship.h"
#include "object_billboard.h"
#include "manager_billboard.h"
#include "player.h"
#include "collision.h"
#include "mode.h"
#include "client.h"
#include "gather.h"


Gather::Gather(XMFLOAT3 startpos, Ship* p_ship, int id)
	:Object3D(p_ship, OBJECTTYPE_GATHER, startpos, XMFLOAT3(1.0f, 1.0f, 1.0f)),m_id(id)
{
	//回転の初期化
	if (p_ship->GetTeamId() == 0) {
		SetRotation(XMFLOAT3(0.0f, 90.0f * (3.1415f / 180.0f), 0.0f));
		SetPosition({ GetPosition().x - 1.5f,GetPosition().y + 1.0f,GetPosition().z });
	}
	else if (p_ship->GetTeamId() == 1) {
		SetRotation(XMFLOAT3(0.0f, -90.0f * (3.1415f / 180.0f), 0.0f));
		SetPosition({ GetPosition().x + 1.5f,GetPosition().y + 1.0f,GetPosition().z });
	}


	m_IsUsingGather = true;
	m_CoolTimeFlag = false;
	m_CoolTimeMax = 600;
	m_CoolTime = 0;

	if (GetShip()->GetTeamId() == 0)
		m_BillboardObject = new BillboardObject(BILLBOARDTYPE_GATHER, { GetPosition().x + 1.0f, GetPosition().y + 2.0f, GetPosition().z }, { 0.4f, 0.4f, 0.4f });
	else
		m_BillboardObject = new BillboardObject(BILLBOARDTYPE_GATHER, { GetPosition().x - 1.0f, GetPosition().y + 2.0f, GetPosition().z }, { 0.4f, 0.4f, 0.4f });

	BillBoradManagerRegister(m_BillboardObject);

};

Gather::~Gather()
{
	
}



//アップデート
void Gather::Update()
{

	float ObjectRadius = 4.0f;

	if (m_BillboardObject)
	{
		if (m_IsUsingGather)
		{
			for (auto object : GetShip()->GetPlayers())
			{
				if (object->GetItemState() == ITEMSTATE_NONE)
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

	if (GetGameMode() != GAMEMODE_ONLINE) {
		if (m_CoolTimeFlag)
		{
			m_CoolTime++;

			if (m_CoolTime >= m_CoolTimeMax)
			{
				m_CoolTimeFlag = false;

				m_IsUsingGather = true;

				m_CoolTime = 0;
			}
		}
	}
}

//ドロー
void Gather::Draw() const
{
	
	ModelDraw(MODELNAME_GATHER, GetPosition(), GetRotation());
	
}

void Gather::SetUsingGather(bool buse)
{
	m_IsUsingGather = buse;

	if (!buse)
	{
		m_CoolTime = 0;
	}
}

bool Gather::GetUsingGather()
{
	return m_IsUsingGather;
}

int Gather::GetId()
{
	return m_id;
}


void Gather::SetCoolTimeFlag(bool buse)
{
	m_CoolTimeFlag = buse;
}

void Gather::SetNet(XMFLOAT3 endposition, XMFLOAT3 size , Player* p_player)
{
	
	GetShip()->RegisterNets(new Net(XMFLOAT3(GetPosition().x, GetPosition().y, GetPosition().z), endposition, size, p_player, GetShip(), this));
}

void Gather::SetCoolTimeMax(int level)
{
	switch (level)
	{
	case 0:
		m_CoolTimeMax = 600;
		break;
	case 1:
		m_CoolTimeMax = 540;
		break;
	case 2:
		m_CoolTimeMax = 480;
		break;
	case 3:
		m_CoolTimeMax = 420;
		break;
	case 4:
		m_CoolTimeMax = 360;
		break;
	case 5:
		m_CoolTimeMax = 240;
		break;
	}
}

void Gather::Reset()
{
	m_IsUsingGather = true;
	m_CoolTime = m_CoolTimeMax;
}




