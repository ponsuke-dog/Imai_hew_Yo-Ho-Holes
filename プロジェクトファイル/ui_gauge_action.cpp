/*===============================================================


		ゲージアクションのUI [ui_gauge_action.cpp]

										Author : shusuke katori
										Date   : 2024/12/28

==================================================================*/

#include "sprite.h"
#include "texture.h"
#include "configuration.h"
#include "mode.h"

#include "ui_gauge_action.h"
//デバッグ用
#include "keyboard.h"

#define GAUGE_SIZE_Y (256.0f)


GaugeActionUI::GaugeActionUI()
{
	m_Texture[0] = TextureLoad(L"asset\\texture\\gageBoom.png");
	m_positionXY[0] = XMFLOAT2(SCREEN_WIDTH / 2 + 112.0f, SCREEN_HEIGHT / 2 + 128.0f);
	m_sizeXY[0] = XMFLOAT2(48.0f, 64.0f);

	m_Texture[1] = TextureLoad(L"asset\\texture\\gageStroke.png");
	m_positionXY[1] = XMFLOAT2(SCREEN_WIDTH / 2 + 100.0f, SCREEN_HEIGHT / 2);
	m_sizeXY[1] = XMFLOAT2(32.0f, 256.0f);

	m_CannonIcon = TextureLoad(L"asset\\texture\\gage_animation.png");

	m_BGblack = TextureLoad(L"asset\\texture\\black.png");
	m_AnimationTime = 0;
	m_FrameCount = 0;
	m_ControllersTex[0] = TextureLoad(L"asset\\texture\\button_Trigger_move.png");
	m_ControllersTex[1] = TextureLoad(L"asset\\texture\\RT.png");
	m_ControllersTex[2] = TextureLoad(L"asset\\texture\\gatherText.png");

	m_ReverseMove = 0;

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

GaugeActionUI::~GaugeActionUI()
{

}

void GaugeActionUI::Update()
{
	m_AnimationTime = m_FrameCount / 30;
	m_FrameCount++;
}

void GaugeActionUI::Draw()
{	
	int animX = 0;
	animX = (m_AnimationTime % 2) * 16;

	SpriteDraw(m_Texture[1], m_positionXY[1].x, m_positionXY[1].y, m_sizeXY[1].x * m_SizeRaito, m_sizeXY[1].y);
	SpriteDraw(m_Texture[0], m_positionXY[0].x, m_positionXY[0].y, m_sizeXY[0].x * m_SizeRaito, m_sizeXY[0].y, animX, 0, 16, 16, 0, m_ReverseFlg);

	animX = (m_AnimationTime % 5) * (TextureGetWidth(m_CannonIcon) / 5);

	SpriteDraw(m_CannonIcon, SCREEN_WIDTH / 2 + m_ReverseMove, SCREEN_HEIGHT / 2 - 200.0f, 175.0f * m_SizeRaito, 100.0f,
		animX, 0, (TextureGetWidth(m_CannonIcon) / 5), TextureGetHeight(m_CannonIcon), 0, m_ReverseFlg);
	
	animX = (m_AnimationTime % 3) * 12;

	SpriteSetColor(1.0f, 1.0f, 1.0f, 0.3f);
	SpriteDraw(m_BGblack, SCREEN_WIDTH - 125.0f * m_SizeRaito, SCREEN_HEIGHT - 50.0f, 250.0f * m_SizeRaito, 100.0f, 0, 0, 200, 100);
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 発射操作説明
	SpriteDraw(m_ControllersTex[0], SCREEN_WIDTH - 50.0f * m_SizeRaito, SCREEN_HEIGHT - 50.0f, 100.0f * m_SizeRaito, 100.0f, animX, 0, 12, 12);
	SpriteDraw(m_ControllersTex[1], SCREEN_WIDTH - 50.0f * m_SizeRaito, SCREEN_HEIGHT - 50.0f, 60.0f * m_SizeRaito, 60.0f);
	SpriteDraw(m_ControllersTex[2], SCREEN_WIDTH - 175.0f * m_SizeRaito, SCREEN_HEIGHT - 50.0f, 150.0f * m_SizeRaito, 75.0f, 0, 100, 200, 100);
}

void GaugeActionUI::MoveGaugeBar(float gaugeCount)
{

	float move = GAUGE_SIZE_Y * (gaugeCount * 0.1f);

	//ｙは後で修正
	m_positionXY[0].y = SCREEN_HEIGHT / 2 + 128.0f - move;

}

void GaugeActionUI::MoveGaugeBar(float gaugeCount, int TeamId)
{
	if(TeamId == 0)
	{
		float move = GAUGE_SIZE_Y * (gaugeCount * 0.1f);

		//ｙは後で修正
		m_positionXY[0].y = SCREEN_HEIGHT / 2 + 120.0f - move;
		m_positionXY[0].x = SCREEN_WIDTH / 2 + 112.0f;
		m_positionXY[1].x = SCREEN_WIDTH / 2 + 100.0f;
		m_ReverseFlg = false;
		m_ReverseMove = 156.0f;
	}
	else
	{
		float move = GAUGE_SIZE_Y * (/*1.0f - */(gaugeCount * 0.1f));

		//ｙは後で修正
		m_positionXY[0].y = SCREEN_HEIGHT / 2 - 136.0f + move;
		m_positionXY[0].x = SCREEN_WIDTH / 2 - 112.0f;
		m_positionXY[1].x = SCREEN_WIDTH / 2 - 100.0f;
		m_ReverseFlg = true;
		m_ReverseMove = -156.0f;
	}
}

