//********************************************************************************
//
// 強化[enhance.cpp] 
// 
//															Author : Hiroki Nakajima
//															Date   : 2024/??
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/25
//********************************************************************************
#include "manager_model.h"
#include "gather.h"
#include "ship.h"
#include "object_billboard.h"
#include "manager_billboard.h"
#include "player.h"
#include "collision.h"
#include "mode.h"
#include "client.h"
#include "enhance.h"

//===================================================
// コンストラクタ
//===================================================
Enhance::Enhance(XMFLOAT3 pos, Ship* pship) : Object3D(pship, OBJECTTYPE_ENHANCE, pos)
{
	//SetRotation(XMFLOAT3(0.0f, 180.0f * (3.1415f / 180.0f), 0.0f));
	SetPosition(XMFLOAT3(GetPosition().x, GetPosition().y + 1.0f, GetPosition().z + 1.4f));
	m_CannonLevel = 0;
	m_GatherLevel = 0;
	m_RepairLevel = 0;
	m_Stock = 0;
	m_IsUsingEnhance = true;

	if (GetGameMode() == GAMEMODE_SOLO)
	{
		m_CannonLevel = 5;
		m_GatherLevel = 5;
		m_RepairLevel = 5;
	}

	m_BillboardObject = new BillboardObject(BILLBOARDTYPE_ENHANCE, { GetPosition().x, GetPosition().y + 2.0f, GetPosition().z - 1.0f}, { 0.4f, 0.4f, 0.4f });
	BillBoradManagerRegister(m_BillboardObject);

}

//===================================================
// デストラクタ
//===================================================
Enhance::~Enhance()
{
}

//===================================================
// 更新 
//===================================================
void Enhance::Update()
{
	float ObjectRadius = 4.0f;

	if (m_BillboardObject)
	{
		if (m_IsUsingEnhance)
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

}

//===================================================
// 描画
//===================================================
void Enhance::Draw() const
{
	ModelDraw(MODELNAME_ENHANCE, GetPosition(), GetRotation());
}

//===================================================
// 大砲レベルの取得
//===================================================
int Enhance::GetCannonLevel()
{
	return m_CannonLevel;
}

//===================================================
// 大砲レベルの設定(上昇)
//===================================================
void Enhance::SetCannonLevel()
{
	m_CannonLevel++;
}

void Enhance::SetCannonLevel(int level)
{
	m_CannonLevel = level;
}

//===================================================
// 回収レベルの取得
//===================================================
int Enhance::GetGatherLevel()
{
	return m_GatherLevel;
}

//===================================================
// 回収レベルの設定(上昇)
//===================================================
void Enhance::SetGatherLevel()
{
	m_GatherLevel++;
	for (auto pGathers : GetShip()->GetGathers())
	{
		pGathers->SetCoolTimeMax(m_GatherLevel); //クールタイムの設定
	}
}

void Enhance::SetGatherLevel(int level)
{
	m_GatherLevel = level;
}

//===================================================
// 修復レベルの取得
//===================================================
int Enhance::GetRepairLevel()
{
	return m_RepairLevel;
}

//===================================================
// 修復レベルの設定(上昇)
//===================================================
void Enhance::SetRepairLevel()
{
	m_RepairLevel++;
}

void Enhance::SetRepairLevel(int level)
{
	m_RepairLevel = level;
}

//===================================================
// 強化素材の所持数の取得
//===================================================
int Enhance::GetStock()
{
	return m_Stock;
}

//===================================================
// 強化素材の所持数の設定(足す)
//===================================================
void Enhance::AddStock(int stock)
{
	static constexpr int STOCK_MAX = 25; //所持上限
	m_Stock += stock;

	m_Stock = std::min(m_Stock, STOCK_MAX); //所持上限を超えていたら修正
}

//===================================================
// 強化素材の所持数の設定(引く)
//===================================================
void Enhance::DecreaseStock(int stock)
{
	m_Stock -= stock;
}

void Enhance::SetStock(int stock)
{
	static constexpr int STOCK_MAX = 25; //所持上限
	m_Stock = stock;

	m_Stock = std::min(m_Stock, STOCK_MAX); //所持上限を超えていたら修正
}

void Enhance::SetUsingEnhance(bool buse)
{
	m_IsUsingEnhance = buse;
}

bool Enhance::GetUsingEnhance()
{
	return m_IsUsingEnhance;
}

void Enhance::Reset()
{
	m_IsUsingEnhance = true;
}
