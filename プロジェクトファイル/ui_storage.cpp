#include "configuration.h"
#include "texture.h"
#include "shader.h"
#include "sprite.h"
#include "storage.h"
#include "ship.h"
#include "mode.h"

#include "ui_storage.h"

#define STORAGE_BOMB_POS (XMFLOAT2(440.0f,30.0f))

StorageUI::StorageUI(Ship* pShip):m_pShip(pShip)
{
	m_NumTex=TextureLoad(L"asset\\texture\\number_sansBold 3.png");
	m_NumMultiplication=TextureLoad(L"asset\\texture\\×.png");
	m_StorageTex[0] = TextureLoad(L"asset\\texture\\UI_Item_Boomer.png");
	m_StorageTex[1] = TextureLoad(L"asset\\texture\\UI_Item_Fixer.png");
	m_Size = { 50.f,50.0f };
	m_Postion = { STORAGE_BOMB_POS.x,STORAGE_BOMB_POS.y };
	m_StorageNum[0] = m_pShip->GetStorages()[0]->GetStock();
	m_TextureSize = { TextureGetWidth(m_NumTex),TextureGetHeight(m_NumTex) };
	if (GetGameMode() == GAMEMODE_SOLO ||
		GetGameMode() == GAMEMODE_MULTIWINDOW ||
		GetGameMode() == GAMEMODE_LOCAL_MULTIWINDOW ||
		GetGameMode() == GAMEMODE_ONLINE)
	{
		m_SizeRaito = 1;
	}
	else
	{
		m_SizeRaito = 2;
	}
}

StorageUI::~StorageUI()
{
}

void StorageUI::Update()
{
	if (m_pShip->GetTeamId() == 0)
	{
		m_StorageNum[0] = m_pShip->GetStorages()[1]->GetStock();
		m_StorageNum[1] = m_pShip->GetStorages()[0]->GetStock();

		m_EnemyStorageNum[0] = m_pShip->GetEnemyShip()->GetStorages()[0]->GetStock();
		m_EnemyStorageNum[1] = m_pShip->GetEnemyShip()->GetStorages()[1]->GetStock();
	}
	else
	{
		m_StorageNum[0] = m_pShip->GetStorages()[0]->GetStock();
		m_StorageNum[1] = m_pShip->GetStorages()[1]->GetStock();

		m_EnemyStorageNum[0] = m_pShip->GetEnemyShip()->GetStorages()[1]->GetStock();
		m_EnemyStorageNum[1] = m_pShip->GetEnemyShip()->GetStorages()[0]->GetStock();
	}
}

void StorageUI::Draw()
{
	if (GetGameMode()==GAMEMODE_SOLO)
	{
		m_Postion.x = 300.0f;
		if (m_pShip->GetTeamId() == 0)
		{
			m_Postion.x = STORAGE_BOMB_POS.x;
			for (int i = 0; i < 2; i++)
			{
				// ストレージ
				SpriteDraw(m_StorageTex[i], m_Postion.x * m_SizeRaito, m_Postion.y + 40.0f * i, m_Size.x * m_SizeRaito, m_Size.y);
				SpriteDraw(m_NumMultiplication, (m_Postion.x + 30.0f) * m_SizeRaito, m_Postion.y + 40.0f * i, (m_Size.x - 20.0f) * m_SizeRaito, m_Size.y - 20.0f);

				// 10の位
				SpriteDraw(m_NumTex, (m_Postion.x + 50.0f) * m_SizeRaito, m_Postion.y + 40.0f * i, (m_Size.x / 2) * m_SizeRaito, m_Size.y - 20.0f,
					(m_StorageNum[i] / 10) * m_TextureSize.x / 11, 0, m_TextureSize.x / 11, m_TextureSize.y);
				// 1の位
				SpriteDraw(m_NumTex, (m_Postion.x + 70.0f) * m_SizeRaito, m_Postion.y + 40.0f * i, (m_Size.x / 2) * m_SizeRaito, m_Size.y - 20.0f,
					(m_StorageNum[i] % 10) * m_TextureSize.x / 11, 0, m_TextureSize.x / 11, m_TextureSize.y);
			}
		}
	}
	else
	{
		if (m_pShip->GetTeamId() == 0)
		{
			m_Postion.x = STORAGE_BOMB_POS.x;
			for (int i = 0; i < 2; i++)
			{
				// ストレージ
				SpriteDraw(m_StorageTex[i], m_Postion.x * m_SizeRaito, m_Postion.y + 40.0f * i, m_Size.x * m_SizeRaito, m_Size.y);
				SpriteDraw(m_NumMultiplication, (m_Postion.x + 30.0f) * m_SizeRaito, m_Postion.y + 40.0f * i, (m_Size.x - 20.0f) * m_SizeRaito, m_Size.y - 20.0f);

				// 10の位
				SpriteDraw(m_NumTex, (m_Postion.x + 50.0f) * m_SizeRaito, m_Postion.y + 40.0f * i, (m_Size.x / 2) * m_SizeRaito, m_Size.y - 20.0f,
					(m_StorageNum[i] / 10) * m_TextureSize.x / 11, 0, m_TextureSize.x / 11, m_TextureSize.y);
				// 1の位
				SpriteDraw(m_NumTex, (m_Postion.x + 70.0f) * m_SizeRaito, m_Postion.y + 40.0f * i, (m_Size.x / 2) * m_SizeRaito, m_Size.y - 20.0f,
					(m_StorageNum[i] % 10) * m_TextureSize.x / 11, 0, m_TextureSize.x / 11, m_TextureSize.y);

				// 敵船のストレージ
				if (GetGameMode() == GAMEMODE_LOCAL_MULTIWINDOW ||
					GetGameMode() == GAMEMODE_SEGMENTATION ||
					GetGameMode() == GAMEMODE_MULTIWINDOW ||
					GetGameMode() == GAMEMODE_ONLINE)
				{
					m_Postion.x = SCREEN_WIDTH - STORAGE_BOMB_POS.x * m_SizeRaito;

					// ストレージ
					SpriteDraw(m_StorageTex[i], m_Postion.x - 70.0f * m_SizeRaito, m_Postion.y + 40.0f * i, m_Size.x * m_SizeRaito, m_Size.y);
					SpriteDraw(m_NumMultiplication, m_Postion.x - 40.0f * m_SizeRaito, m_Postion.y + 40.0f * i, (m_Size.x - 20.0f) * m_SizeRaito, m_Size.y - 20.0f);

					// 10の位
					SpriteDraw(m_NumTex, m_Postion.x - 20.0f * m_SizeRaito, m_Postion.y + 40.0f * i, (m_Size.x / 2) * m_SizeRaito, m_Size.y - 20.0f,
						(m_EnemyStorageNum[i] / 10) * m_TextureSize.x / 11, 0, m_TextureSize.x / 11, m_TextureSize.y);
					// 1の位
					SpriteDraw(m_NumTex, m_Postion.x, m_Postion.y + 40.0f * i, (m_Size.x / 2) * m_SizeRaito, m_Size.y - 20.0f,
						(m_EnemyStorageNum[i] % 10) * m_TextureSize.x / 11, 0, m_TextureSize.x / 11, m_TextureSize.y);
					m_Postion.x = STORAGE_BOMB_POS.x;

				}
			}
		}
		else
		{
			m_Postion.x = SCREEN_WIDTH - STORAGE_BOMB_POS.x * m_SizeRaito;
			for (int i = 0; i < 2; i++)
			{
				// 砲弾
				SpriteDraw(m_StorageTex[i], m_Postion.x - 70.0f * m_SizeRaito, m_Postion.y + 40.0f * i, m_Size.x * m_SizeRaito, m_Size.y);
				SpriteDraw(m_NumMultiplication, m_Postion.x - 40.0f * m_SizeRaito, m_Postion.y+40.0f*i, (m_Size.x - 20.0f) * m_SizeRaito, m_Size.y - 20.0f);

				// 10の位
				SpriteDraw(m_NumTex, m_Postion.x - 20.0f * m_SizeRaito, m_Postion.y + 40.0f * i, (m_Size.x / 2) * m_SizeRaito, m_Size.y - 20.0f,
					(m_StorageNum[i] / 10) * m_TextureSize.x / 11, 0, m_TextureSize.x / 11, m_TextureSize.y);
				// 1の位
				SpriteDraw(m_NumTex, m_Postion.x , m_Postion.y + 40.0f * i, (m_Size.x / 2) * m_SizeRaito, m_Size.y - 20.0f,
					(m_StorageNum[i] % 10) * m_TextureSize.x / 11, 0, m_TextureSize.x / 11, m_TextureSize.y);
			
				// 敵船のストレージ
				if (GetGameMode() == GAMEMODE_LOCAL_MULTIWINDOW ||
					GetGameMode() == GAMEMODE_SEGMENTATION ||
					GetGameMode() == GAMEMODE_MULTIWINDOW ||
					GetGameMode() == GAMEMODE_ONLINE)
				{
					m_Postion.x = STORAGE_BOMB_POS.x;
					// 砲弾
					SpriteDraw(m_StorageTex[i], m_Postion.x * m_SizeRaito, m_Postion.y + 40.0f * i, m_Size.x * m_SizeRaito, m_Size.y);
					SpriteDraw(m_NumMultiplication, (m_Postion.x + 30.0f) * m_SizeRaito, m_Postion.y + 40.0f * i, (m_Size.x - 20.0f) * m_SizeRaito, m_Size.y - 20.0f);

					// 10の位
					SpriteDraw(m_NumTex, (m_Postion.x + 50.0f) * m_SizeRaito, m_Postion.y + 40.0f * i, (m_Size.x / 2) * m_SizeRaito, m_Size.y - 20.0f,
						(m_EnemyStorageNum[i] / 10) * m_TextureSize.x / 11, 0, m_TextureSize.x / 11, m_TextureSize.y);
					// 1の位
					SpriteDraw(m_NumTex, (m_Postion.x + 70.0f) * m_SizeRaito, m_Postion.y + 40.0f * i, (m_Size.x / 2) * m_SizeRaito, m_Size.y - 20.0f,
						(m_EnemyStorageNum[i] % 10) * m_TextureSize.x / 11, 0, m_TextureSize.x / 11, m_TextureSize.y);
					m_Postion.x = SCREEN_WIDTH - STORAGE_BOMB_POS.x * m_SizeRaito;


				}
			}
		}
	}
}
