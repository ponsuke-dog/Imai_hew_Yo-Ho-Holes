#include "configuration.h"
#include "ship.h"
#include "texture.h"
#include "shader.h"
#include "sprite.h"
#include "mode.h"

#include "ui_resource.h"


#define RESOURCEBGSIZE (150.0f)
#define RESOURCESIZE (120.0f)

Resource::Resource()
{
	m_Texture = TextureLoad(L"asset\\texture\\UI_Item_Outline.png");
	m_ResourceTexture[0]= TextureLoad(L"asset\\texture\\UI_Item_Fixer.png");
	m_ResourceTexture[1]= TextureLoad(L"asset\\texture\\UI_Item_Boomer.png");
	m_SizeRaito = 0;

	// 画面分割による予告サイズの調整
	if (GetGameMode() == GAMEMODE_LOCAL_SPLITSCREEN ||
		GetGameMode() == GAMEMODE_SEGMENTATION)
	{
		m_SizeRaito = 2;
	}
	else
	{
		m_SizeRaito = 1;
	}
}

Resource::~Resource()
{

}

void Resource::Update()
{
}

void Resource::Draw()
{
	// テクスチャの入れ替え用
	int texture = -1;

	switch (m_Player->GetItemState())
	{
	case ITEMSTATE_WOOD:	// 所持アイテム = 修復
		texture = m_ResourceTexture[0];
		break;
	case ITEMSTATE_CANNON:	// 所持アイテム = 砲弾
		texture = m_ResourceTexture[1];
		break;
	default:
		break;
	}
	// 所持アイテムの描画
	if (m_Player->GetItemState() == ITEMSTATE_CANNON || m_Player->GetItemState() == ITEMSTATE_WOOD)
	{
		SpriteDraw(texture, m_Position.x, m_Position.y, (RESOURCESIZE * m_SizeRaito), RESOURCESIZE);
	}
	// 背景
	SpriteDraw(m_Texture, m_Position.x, m_Position.y, (RESOURCEBGSIZE * m_SizeRaito), RESOURCEBGSIZE, 0, 0, 600, 600, 0, m_Reverse);
}

void Resource::SetPlayerResource(Player* pPlayer)
{
	m_Player = pPlayer;

	// 分割ありだったら
	if (GetGameMode() == GAMEMODE_LOCAL_SPLITSCREEN ||
		GetGameMode() == GAMEMODE_SEGMENTATION)
	{
		// 2画面2分割だったら
		if (GetGameMode() == GAMEMODE_SEGMENTATION)
		{
			// 画面左側のプレイヤーだったら
			if (m_Player==m_Player->GetShip()->GetPlayers()[0])
			{
				m_Position.x = RESOURCEBGSIZE;
				m_Reverse = false;
			}
			else
			{
				m_Position.x = SCREEN_WIDTH - RESOURCEBGSIZE;
				m_Reverse = true;
			}
		}
		else
		{
			// 船のチームIDが 0 だったら 
			if (m_Player->GetShip()->GetTeamId() == 0)
			{
				m_Position.x = RESOURCEBGSIZE;
				m_Reverse = false;
			}
			else
			{
				m_Position.x = SCREEN_WIDTH - RESOURCEBGSIZE;
				m_Reverse = true;
			}
		}
		m_Position.y = SCREEN_HEIGHT - RESOURCEBGSIZE/2;

	}
	else
	{
		m_Reverse = true;
		m_Position.x = SCREEN_WIDTH - RESOURCEBGSIZE * 0.5f;
		m_Position.y = SCREEN_HEIGHT - RESOURCEBGSIZE/2;
	}

}
