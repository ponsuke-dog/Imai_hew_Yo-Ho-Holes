#include "configuration.h"
#include "texture.h"
#include "sprite.h"

#include "ship.h"
#include "mode.h"

#include "ui_gather.h"

GatherUI::GatherUI()
{
	m_Texture[0] = TextureLoad(L"asset\\texture\\stick_move_animation.png");
	m_Texture[1] = TextureLoad(L"asset\\texture\\button_Trigger_move.png");
	m_Texture[2] = TextureLoad(L"asset\\texture\\RT.png");
	m_Texture[3] = TextureLoad(L"asset\\texture\\gatherText.png");
	m_BGBlack = TextureLoad(L"asset\\texture\\black.png");
	m_AnimationTime = 0;
	m_FrameCount = 0;
	if (GetGameMode() == GAMEMODE_LOCAL_SPLITSCREEN ||
		GetGameMode() == GAMEMODE_SEGMENTATION)
	{
		m_SizeRaito = 2;
		m_splitmove = 0;
	}
	else
	{
		m_SizeRaito = 1;
	}
}

GatherUI::~GatherUI()
{
}

void GatherUI::Update()
{
	m_AnimationTime = m_FrameCount / 30;
	m_FrameCount++;
}

void GatherUI::Draw()
{
	int animX = (m_AnimationTime % 3) * 48;
	int animY = (m_AnimationTime / 3) * 48;
	
	SpriteSetColor(1.0f, 1.0f, 1.0f, 0.3f);
	SpriteDraw(m_BGBlack, 125.0f*m_SizeRaito, SCREEN_HEIGHT - 50.0f, 250.0f * m_SizeRaito,100.0f , 0, 0, 200, 100);
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);

	// スティック操作説明
	SpriteDraw(m_Texture[0], 50.0f*m_SizeRaito, SCREEN_HEIGHT - 50.0f, 100.0f * m_SizeRaito, 100.0f, animX, animY, 48, 48);
	SpriteDraw(m_Texture[3], 175.0f*m_SizeRaito, SCREEN_HEIGHT - 50.0f, 150.0f * m_SizeRaito, 75.0f, 0, 0, 200, 100);

	
	animX = (m_AnimationTime % 3) * 12;


	SpriteSetColor(1.0f, 1.0f, 1.0f, 0.3f);
	SpriteDraw(m_BGBlack, SCREEN_WIDTH - 125.0f * m_SizeRaito, SCREEN_HEIGHT - 50.0f, 250.0f * m_SizeRaito, 100.0f, 0, 0, 200, 100);
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
	// 発射操作説明
	SpriteDraw(m_Texture[1], SCREEN_WIDTH - 50.0f * m_SizeRaito, SCREEN_HEIGHT - 50.0f, 100.0f * m_SizeRaito, 100.0f, animX, 0, 12, 12);
	SpriteDraw(m_Texture[2], SCREEN_WIDTH - 50.0f * m_SizeRaito, SCREEN_HEIGHT - 50.0f, 60.0f * m_SizeRaito, 60.0f);
	SpriteDraw(m_Texture[3], SCREEN_WIDTH - 175.0f * m_SizeRaito, SCREEN_HEIGHT - 50.0f, 150.0f * m_SizeRaito, 75.0f, 0, 100, 200, 100);

}
