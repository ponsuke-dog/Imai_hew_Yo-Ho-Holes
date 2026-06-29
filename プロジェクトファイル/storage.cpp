//	storage.cpp
//	ストレージクラス
//
//	2024.12.20			中島寛樹

#include "manager_model.h"
#include "ship.h"
#include "player.h"
#include "collision.h"
#include "object_billboard.h"
#include "manager_billboard.h"
#include "mode.h"
#include "client.h"
#include "storage.h"


Storage::Storage(StorageType type, XMFLOAT3 position, Ship* pship):Object3D(pship,OBJECTTYPE_STORAGE,position)
{
	SetRotation(XMFLOAT3(0.0f, (180.0f * (3.1415f / 180.0f)), 0.0f));
	SetPosition(XMFLOAT3(GetPosition().x, GetPosition().y + 1.0f, GetPosition().z - 1.4f));
	m_Type = type;
	m_Stock = 0;

	if (GetGameMode() == GAMEMODE_SOLO)
	{
		m_Stock = 10;
	}

	m_BillboardObject = new BillboardObject(BILLBOARDTYPE_STORAGE, { GetPosition().x, GetPosition().y + 2.5f, GetPosition().z }, { 0.4f, 0.4f, 0.4f });
	BillBoradManagerRegister(m_BillboardObject);

}

Storage::~Storage()
{
}

void Storage::Update()
{
	float ObjectRadius = 4.0f;

	if (m_BillboardObject)
	{
		if (m_Stock > 0)
		{
			for (auto object : GetShip()->GetPlayers())
			{
				m_BillboardObject->SetDrawFlag(false);


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
		else {
			m_BillboardObject->SetDrawFlag(false);
		}
	}
}

void Storage::Draw() const
{
	switch (m_Type)
	{
	case STORAGETYPE_CANNON:
		ModelDraw(MODELNAME_STORAGE_CANNON, GetPosition(), GetRotation());
		break;
	case STORAGETYPE_REPAIR:
		ModelDraw(MODELNAME_STORAGE_WOOD, GetPosition(), GetRotation());
		break;
	default:
		break;
	}
}

void Storage::Increase(int increment)
{
	m_Stock += increment;
	if (m_Stock >= 99)
	{
		m_Stock = 99;
	}
}

void Storage::Decrease(int decrement)
{
	m_Stock -= decrement;
	if (m_Stock <= 0)
	{
		m_Stock = 0;
	}
}

StorageType Storage::GetType()
{
	return m_Type;
}

void Storage::SetType(StorageType type)
{
	m_Type = type;
}

int Storage::GetStock()
{
	return m_Stock;
}

void Storage::SetStock(int stock)
{
	m_Stock = stock;
}
