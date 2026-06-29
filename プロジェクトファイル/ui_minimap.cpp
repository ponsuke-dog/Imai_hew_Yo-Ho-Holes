#include "configuration.h"
#include "directx.h"
#include "sprite.h"
#include "texture.h"
#include "ship.h"
#include "deck.h"
#include "mode.h"
#include "player.h"
#include "ui_minimap.h"

#include <vector>
#include <iostream>

#define ICON_PLAYER_SIZE (30.0f*0.8f)
#define ICON_CANNON_SIZE (30.0f*0.8f)
#define ICON_GATHER_SIZE (30.0f*0.8f)
#define ICON_STORAGE_SIZE (30.0f*0.8f)
#define ICON_UPPER_SIZE (60.0f*0.8f)

#define ONE_ICON_SIZE (7 * 0.8f)
#define ICON_DECK_SIZE ((float)ONE_ICON_SIZE)

#define MINIMAP_RECTIFICATION_X (280.0f*0.8f) 
#define MINIMAP_RECTIFICATION_Y (280.0f*0.8f)


#define MINIMAPBG_SIZE (XMFLOAT3(175.0f*0.8f,0.0f,270.0f*0.8f))




MiniMap::MiniMap()
{
	m_Texture_Player = TextureLoad(L"asset\\texture\\minimap_players_icon.png");
	m_Texture_MinimapBG = TextureLoad(L"asset\\texture\\minimap_shipOutline.png");
	m_Texture_BackLight = TextureLoad(L"asset\\texture\\backlight.png");
	m_Texture = TextureLoad(L"asset\\texture\\minimap_field03.png");
	m_Texture_Black = TextureLoad(L"asset\\texture\\black.png");
	m_TextureIcon = TextureLoad(L"asset\\texture\\minimap_icons.png");
	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_PlayerID = 0;
	if (GetGameMode()==GAMEMODE_SOLO||
		GetGameMode()==GAMEMODE_MULTIWINDOW||
		GetGameMode()==GAMEMODE_LOCAL_MULTIWINDOW||
		GetGameMode()==GAMEMODE_ONLINE)
	{
		m_SizeRaito = 1;
	}
	else
	{
		m_SizeRaito = 2;
	}
}

MiniMap::~MiniMap()
{
}

void MiniMap::Update()
{
}

void MiniMap::Draw()
{
	SetDepthEnable(false);
	// ミニマップ背景の座標調整
	float bgpos = 0;

	if (GetGameMode() == GAMEMODE_LOCAL_SPLITSCREEN)
	{
		if (m_Ships->GetTeamId() == 0)
		{
			bgpos = m_Position.x - (175.0f * m_SizeRaito) * 0.8f;
		}
		else
		{
			bgpos = m_Position.x + (175.0f * m_SizeRaito) * 0.8f;
		}
	}
	else if(GetGameMode()==GAMEMODE_SEGMENTATION)
	{
		bgpos = m_Position.x;
	}
	else
	{
		bgpos = m_Position.x - (175.0f * m_SizeRaito) * 0.8f;
	}

	// minimap背景の描画
	SpriteDraw(m_Texture_Black, bgpos, m_Position.z, MAP_WIDTH_NUM * ICON_DECK_SIZE * m_SizeRaito, MAP_HEIGHT_NUM * ICON_DECK_SIZE);
	SpriteDraw(m_Texture_MinimapBG, bgpos, m_Position.z - 17.0f * 0.8f, MINIMAPBG_SIZE.x * m_SizeRaito, MINIMAPBG_SIZE.z);

	SpriteSetColor(2.5f, 2.5f, 2.0f, 1.0f);
	// 甲板の描画(Deck,Unbreakable)
	for (auto deckblocks : m_Ships->GetDeckBlocks())
	{

		XMFLOAT3 deckpos;
		deckpos.x = ((deckblocks->GetPosition().x) * ONE_ICON_SIZE * m_SizeRaito) + m_Position.x + m_PositionRectification;
		deckpos.y = 0.0f;
		deckpos.z = (-deckblocks->GetPosition().z * ONE_ICON_SIZE) + m_Position.z;
		m_Texsize.x = ICON_DECK_SIZE;
		m_Texsize.y = ICON_DECK_SIZE;

		if (deckblocks->GetUse())
		{
			SpriteDraw(m_Texture, deckpos.x, deckpos.z, m_Texsize.x * m_SizeRaito, m_Texsize.y);
		}
	}
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 前面のアイコン(Cannon,Gather,Storage)
	for (auto deckblocks : m_Ships->GetDeckBlocks())
	{
		XMFLOAT3 fronticon;
		fronticon.x = (deckblocks->GetPosition().x * ONE_ICON_SIZE)* m_SizeRaito + m_Position.x + m_PositionRectification;
		fronticon.y = 0.0f;
		fronticon.z = (-deckblocks->GetPosition().z * ONE_ICON_SIZE) + m_Position.z;
		
		int TextureUV = -1;
		float upperposplus = 0;

		switch (deckblocks->GetDeckType())
		{
		case DECKTYPE_CANNON:
			m_Texsize.x = ICON_CANNON_SIZE;
			m_Texsize.y = ICON_CANNON_SIZE;
			TextureUV = 0;
			break;
		case DECKTYPE_GATHER:
			m_Texsize.x = ICON_GATHER_SIZE;
			m_Texsize.y = ICON_GATHER_SIZE;
			TextureUV = 1;
			break;
		case DECKTYPE_CANNON_STORAGE:
			m_Texsize.x = ICON_STORAGE_SIZE;
			m_Texsize.y = ICON_STORAGE_SIZE;
			TextureUV = 2;
			break;
		case DECKTYPE_REPAIR_STORAGE:
			m_Texsize.x = ICON_STORAGE_SIZE;
			m_Texsize.y = ICON_STORAGE_SIZE;
			TextureUV = 3;
			break;
		case DECKTYPE_ENHANCE:
			m_Texsize.x = ICON_UPPER_SIZE;
			m_Texsize.y = ICON_UPPER_SIZE;
			TextureUV = 4;
			upperposplus =ONE_ICON_SIZE*3.0f;
			fronticon.z -= upperposplus;
			break;
		default:
			break;
		}
		if (deckblocks->GetDeckType() == DECKTYPE_CANNON || deckblocks->GetDeckType() == DECKTYPE_GATHER
			|| deckblocks->GetDeckType() == DECKTYPE_CANNON_STORAGE || deckblocks->GetDeckType() == DECKTYPE_REPAIR_STORAGE
			|| deckblocks->GetDeckType() == DECKTYPE_ENHANCE)
		{
			SpriteDraw(m_TextureIcon, fronticon.x, fronticon.z, m_Texsize.x * m_SizeRaito, m_Texsize.y, TextureUV * 60, 0, 60, 60, 0, m_Reverse);
		}

	}
	
	for (int i = 0; i < m_Ships->GetPlayers().size(); i++)
	{
		// ミニマップ上のプレイヤーの位置
		XMFLOAT3 playerpos = m_Ships->GetPlayerPos(i);
		playerpos.x = (m_Ships->GetPlayerPos(i).x * ONE_ICON_SIZE) * m_SizeRaito + m_Position.x + m_PositionRectification;
		playerpos.z = -m_Ships->GetPlayerPos(i).z * ONE_ICON_SIZE + m_Position.z;

		int playericon = m_Ships->GetPlayers()[i]->GetId();
		if (m_PlayerLightFlg && m_Ships->GetPlayers()[i]->GetId() == m_PlayerID)
		{
			SpriteDraw(m_Texture_BackLight, playerpos.x, playerpos.z, ICON_PLAYER_SIZE * m_SizeRaito, ICON_PLAYER_SIZE);
		}
		SpriteDraw(m_Texture_Player, playerpos.x, playerpos.z, ICON_PLAYER_SIZE* m_SizeRaito, ICON_PLAYER_SIZE, playericon * 60, 0, 60, 60);
	}
	SetDepthEnable(true);

}

// 後で使う
void MiniMap::SetMinimapUse()
{
	m_Use = !m_Use;
}

void MiniMap::SetMiniMap(Ship* pShip)
{
	m_Ships = pShip;

	switch (GetGameMode())
	{
	case GAMEMODE_LOCAL_SPLITSCREEN:
		if (m_Ships->GetTeamId() == 0)
		{
			// ここでミニマップの位置調整
			m_Position.x = MINIMAP_RECTIFICATION_X * m_SizeRaito;
			m_Position.z = MINIMAP_RECTIFICATION_Y;
			m_PositionRectification = 0;
		}
		else
		{
			// ここでミニマップの位置調整
			m_Position.x = SCREEN_WIDTH - MINIMAP_RECTIFICATION_X * m_SizeRaito;
			m_Position.z = MINIMAP_RECTIFICATION_Y;
			m_PositionRectification = 0;
		}
		break;

	case GAMEMODE_SOLO:
	case GAMEMODE_LOCAL_MULTIWINDOW:
	case GAMEMODE_MULTIWINDOW:
	case GAMEMODE_ONLINE:
		if (m_Ships->GetTeamId() == 0)
		{
			// ここでミニマップの位置調整
			m_Position.x = MINIMAP_RECTIFICATION_X * m_SizeRaito;
			m_Position.z = SCREEN_HEIGHT - MINIMAP_RECTIFICATION_Y/2;
			m_PositionRectification = +0;
		}
		else
		{
			// ここでミニマップの位置調整
			m_Position.x = MINIMAP_RECTIFICATION_X * m_SizeRaito ;
			m_Position.z = SCREEN_HEIGHT - MINIMAP_RECTIFICATION_Y/2;
			m_PositionRectification = -280.0f;
		}
		break;
	default:
		// ここでミニマップの位置調整
		m_Position.x = MINIMAP_RECTIFICATION_X * m_SizeRaito;
		m_Position.z = MINIMAP_RECTIFICATION_Y;
		break;
	}

	// チームIDによって画像を反転させる
	if (m_Ships->GetTeamId() == 0)
	{
		m_Reverse = false;
	}
	else
	{
		m_Reverse = true;
	}
	// ミニマップのアイコン変更も追加する↓
}

void MiniMap::SetMiniMap(Ship* pShip, int id)
{
	m_Ships = pShip;
	m_PlayerID = id;
	// ここでミニマップの位置調整
	m_Position.z = MINIMAP_RECTIFICATION_Y;
	if (id == m_Ships->GetPlayers()[0]->GetId())
	{
		m_Position.x = (MINIMAP_RECTIFICATION_X / 2) * m_SizeRaito;
		if (m_Ships->GetTeamId() == 0)
		{
			m_PositionRectification = +280.0f;
			m_PlayerLightFlg = true;
		}
		else
		{
			m_PositionRectification = -280.0f;
			m_PlayerLightFlg = true;
		}
	}
	else if (id == m_Ships->GetPlayers()[1]->GetId())
	{
		m_Position.x = SCREEN_WIDTH - (MINIMAP_RECTIFICATION_X / 2) * m_SizeRaito;
		if (m_Ships->GetTeamId() == 0)
		{
			m_PositionRectification = +280.0f;
			m_PlayerLightFlg = true;
		}
		else
		{
			m_PositionRectification = -280.0f;
			m_PlayerLightFlg = true;

		}
	}

}
