//	deckblock.h
//	
//
//	2024.11.19			ポンスケ

#include "manager_model.h"
#include "ship.h"
#include "enhance.h"
#include "object_billboard.h"
#include "manager_billboard.h"
#include "effect_3d.h"
#include "manager_effect.h"
#include "player.h"
#include "collision.h"
#include "client.h"
#include "mode.h"
#include "deckblock.h"

// コンストラクター
DeckBlock::DeckBlock(Ship* ship, XMFLOAT3 pos, DeckType decktype, int id):Object3D(ship, OBJECTTYPE_DECK,pos),m_id(id)
{
	m_decktype = decktype;						// タイプ
	m_hitpoint = 100.0f;							// HP
	m_buse = true;								// フラグ
}

// デストラクター
DeckBlock::~DeckBlock()
{

}

void DeckBlock::Update()
{
	float ObjectRadius = 2.0f;

	if (m_BillboardObject)
	{
		for (auto object : GetShip()->GetPlayers())
		{
			if (object->GetItemState() == ITEMSTATE_WOOD && object->GetAction() != ACTIONSTATE_REPAIR)
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
			else
			{
				m_BillboardObject->SetDrawFlag(false);
			}

		}
	}
}

void DeckBlock::Draw()const
{
	if (m_buse)
	{
		ModelDraw(MODELNAME_DECKBLOCK, GetPosition());
	}

}

void DeckBlock::SetDeckType(DeckType Type)
{
	m_decktype = Type;
}

void DeckBlock::SetHitPoint(float HP)
{
	m_hitpoint = HP;
}

void DeckBlock::SetUse(bool use)
{
	m_buse = use;
}

void DeckBlock::Damage(float damage)
{
	m_hitpoint -= damage;

	if (m_hitpoint <= 0.0f) {
		m_hitpoint = 0.0f;
		m_buse = false;

		if (m_BillboardObject == nullptr)
		{
			m_BillboardObject = new BillboardObject(BILLBOARDTYPE_REPAIR, { GetPosition().x, GetPosition().y + 2.0f, GetPosition().z }, { 0.4f, 0.4f, 0.4f });
			BillBoradManagerRegister(m_BillboardObject);
		}
		
		if (m_pEffectobject == nullptr)
		{
			m_pEffectobject = new Effect3D(EFFECTTYPE_FIRE, { GetPosition().x, GetPosition().y + 0.5f, GetPosition().z }, { 1.0f, 1.0f, 1.0f }, GetEffectFrameMaxNum(EFFECTTYPE_FIRE), EFFECT3DTYPE_LOOP);
			EffectManagerRegister(m_pEffectobject);
		}
		
	}
}

void DeckBlock::Damage(BlockData data)
{
	m_hitpoint = data.hp;

	if (m_hitpoint <= 0.0f) {
		m_hitpoint = 0.0f;
		m_buse = false;

		if (m_BillboardObject == nullptr)
		{
			m_BillboardObject = new BillboardObject(BILLBOARDTYPE_REPAIR, { GetPosition().x, GetPosition().y + 2.0f, GetPosition().z }, { 0.4f, 0.4f, 0.4f });
			BillBoradManagerRegister(m_BillboardObject);
		}
	
		if (m_pEffectobject == nullptr)
		{
			m_pEffectobject = new Effect3D(EFFECTTYPE_FIRE, { GetPosition().x, GetPosition().y + 0.5f, GetPosition().z }, { 1.0f, 1.0f, 1.0f }, GetEffectFrameMaxNum(EFFECTTYPE_FIRE), EFFECT3DTYPE_LOOP);
			EffectManagerRegister(m_pEffectobject);
		}
		
	}
}

void DeckBlock::Repair()
{
	int HEALING_POINT = 0;

	m_RepairCount++;

	switch (GetShip()->GetEnhance()->GetRepairLevel())
	{
	case 0: 
		HEALING_POINT = 0;
		break;
	case 1:
		HEALING_POINT = 10;
		break;
	case 2:
		HEALING_POINT = 15;
		break;
	case 3:
		HEALING_POINT = 20;
		break;
	case 4:
		HEALING_POINT = 25;
		break;
	case 5:
		HEALING_POINT = 35;
		break;
	}

	m_hitpoint = float((50 / m_RepairCount) + HEALING_POINT);

	if (m_hitpoint > 0.0f)
	{
		m_buse = true;
		if (m_BillboardObject) {
			m_BillboardObject->SetDestroyFlag(true);
			m_BillboardObject = nullptr;
		}

		if (m_pEffectobject) {
			m_pEffectobject->SetDestroyFlag(true);
			m_pEffectobject = nullptr;
		}
	}
}

//オンライン用
void DeckBlock::Repair(float hp)
{
	m_hitpoint = hp;

	m_buse = true;
	if (m_BillboardObject) {
		m_BillboardObject->SetDestroyFlag(true);
		m_BillboardObject = nullptr;
	}
	if (m_pEffectobject) {
		m_pEffectobject->SetDestroyFlag(true);
		m_pEffectobject = nullptr;
	}

	EffectManagerRegister(EFFECTTYPE_REPAIR, { GetPosition().x,GetPosition().y + 1.5f,GetPosition().z }, { 2.5f, 2.5f, 2.5f });
	EffectManagerRegister(EFFECTTYPE_CLUNK_MOJI, { GetPosition().x,GetPosition().y + 1.5f,GetPosition().z }, { 3.0f, 3.0f, 3.0f });
	//EffectManagerRegister(EFFECTTYPE_EXPLOSION, { GetPosition().x, GetPosition().y + 1.5f, GetPosition().z }, { 2.5f, 2.5f, 2.5f });
	//EffectManagerRegister(new Effect3D(EFFECTTYPE_REPAIR, { GetPosition().x, GetPosition().y + 1.5f, GetPosition().z }, { 2.5f, 2.5f, 2.5f }, 60, EFFECT3DTYPE_NORMAL));

}

DeckType DeckBlock::GetDeckType()
{
	return m_decktype;
}

float DeckBlock::GetHitPoint()
{
	return m_hitpoint;
}
bool DeckBlock::GetUse()
{
	return m_buse;
}

int DeckBlock::GetId()
{
	return m_id;
}
