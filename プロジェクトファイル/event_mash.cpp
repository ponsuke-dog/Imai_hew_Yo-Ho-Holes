//********************************************************************************
//
// 連打イベント[event_mash.cpp] 
// 
//															Author : Hiroki Nakajima
//															Date   : 2025/01/18
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/12
//********************************************************************************
#include "ship.h"
#include "texture.h"
#include "sprite.h"
#include "deck.h"
#include "configuration.h"
#include "keyboard.h"
#include "controller.h"
#include "easing.h"
#include "effect_2d.h"
#include <random>
#include <math.h>
#include "cri.h"
#include "mode.h"
#include "player.h"
#include "client.h"
#include "camera_player.h"
#include "event_mash.h"

#define DAMAGE (30) //イベント終了時に破壊するブロックの数

#define GAGEGEIGHT (50)
#define GAGESIZE_Y (75)

#define BUTTON_ANIM_FRAME (10)

#define SHIPPOS_X_LEFT (SCREEN_WIDTH * 0.15f)  //船の位置(左)
#define SHIPPOS_X_RIGHT (SCREEN_WIDTH * 0.85f) //船の位置(右)

#define BOMBSIZE_DEFAULT (200.0f) //爆弾のデフォルトサイズ

#define BOMB_VELOCITY (XMFLOAT2(24.0f, 24.0f)) //爆弾の吹っ飛ぶ速度


//===================================================
// コンストラクタ
//===================================================
MashEvent::MashEvent(std::vector<Ship*> pShip, std::vector<Controller*> pController) : Event(pShip, pController)
{
	m_Count[0] = 0;
	m_Count[1] = 0;
	m_FrameCount = 0;
	m_AnimCount = 0;
	m_Time = 5 * 60;
	m_ButtonAnimFrame = 0;
	m_BombSpeed = 0.0f;
	m_Angle = 0.0f;
	m_CountSize = 0.0f;
	for (int i = 0; i < EFFECT_MAX; i++)
		m_Effect[i] = nullptr;

	m_State = EVENTSTATE_COUNTDOWN;
	m_AnimState = MASHSTATE_NONE;

	
	m_InstructionTex = TextureLoad(L"asset\\texture\\Push!.png");
	m_EventBGTexture = TextureLoad(L"asset\\texture\\black.png");
	m_GageTexture = TextureLoad(L"asset\\texture\\eventgage.png");
	m_CountTexture = TextureLoad(L"asset\\texture\\number_sansBold 3.png");
	m_ButtonTex[0] = TextureLoad(L"asset\\texture\\Buttons_normal.png");
	m_ButtonTex[1] = TextureLoad(L"asset\\texture\\Buttons_Pushing.png");

	m_BombTex = TextureLoad(L"asset\\texture\\UI_Item_Boomer.png");
	m_BoatTex = TextureLoad(L"asset\\texture\\Boat.png");
	m_ExplosionTex = TextureLoad(L"asset\\texture\\bakuhatu_30f.png");

	m_WaterTexture[0] = TextureLoad(L"asset\\texture\\waveFront.png");
	m_WaterTexture[1] = TextureLoad(L"asset\\texture\\waveAnimation.png");

	m_GageEffect[0] = TextureLoad(L"asset\\texture\\thunder.png");
	m_GageEffect[1] = TextureLoad(L"asset\\texture\\ConpetitionAnimation.png");
}

//===================================================
// デストラクタ
//===================================================
MashEvent::~MashEvent()
{
}

//===================================================
// 更新
//===================================================
void MashEvent::Update()
{
	if (GetGameMode() == GAMEMODE_ONLINE) {
		m_Count[0] = GetClient()->GetEventData().count[0];
		m_Count[1] = GetClient()->GetEventData().count[1];
	}

	if (!GetEndFlag())
	{
		switch (m_State)
		{
		case EVENTSTATE_COUNTDOWN:
			CountUpdate();
			break;

		case EVENTSTATE_EVENT:
			EventUpdate();
			break;

		case EVENTSTATE_ANIM_WAIT: {
			EventPlayerData data;
			data.teamid = GetClient()->GetMyTeamId();
			data.eventcount = 0;
			data.readyflag = EVENTPLAYERREADY_INPUT_END;

			GetClient()->SendEventData(data);

			if (GetClient()->GetEventData().scene == EVENTSCENE_ANIM_WAIT) {
				m_State = EVENTSTATE_ANIM;
			}
			break;
		}

		case EVENTSTATE_ANIM:
			AnimUpdate();
			break;

		case EVENTSTATE_DAMAGE_WAIT: {
			EventPlayerData data;
			data.teamid = GetClient()->GetMyTeamId();
			data.eventcount = 0;
			data.readyflag = EVENTPLAYERREADY_ANIM_END;

			GetClient()->SendEventData(data);

			if (GetClient()->GetEventData().scene == EVENTSCENE_ENDWAIT) {
				m_State = EVENTSTATE_DAMAGE;
			}
			break;
		}

		case EVENTSTATE_DAMAGE:
			DamageUpdate();
			break;

		case EVENTSTATE_END_WAIT: {

			EventPlayerData data;
			data.teamid = GetClient()->GetMyTeamId();
			data.eventcount = 0;
			data.readyflag = EVENTPLAYERREADY_END;

			GetClient()->SendEventData(data);

			if (GetClient()->GetEventData().scene == EVENTSCENE_END) {
				SetEndFlag(true); //イベント終了
			}
			break;
		}
		default:
			break;
		}
		m_AnimCount++;
	}
}

//===================================================
// 描画
//===================================================
void MashEvent::Draw(int)
{
	SetDepthEnable(false);

	// 背景の黒
	SpriteSetColor(1.0f, 1.0f, 1.0f, 0.4f);
	SpriteDraw(m_EventBGTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	// カウントダウン
	if (m_State == EVENTSTATE_COUNTDOWN)
	{
		// 追加で黒くする
		SpriteDraw(m_EventBGTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);
		SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
		SpriteDraw(m_InstructionTex, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, m_CountSize * 50.0f + 1000.0f, m_CountSize * 50.0f + 200.0f);

		// カウントダウン
		SpriteDraw(m_CountTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4,
			m_CountSize * 10.0f + 200.0f, m_CountSize * 10.0f + 200.0f,
			(EVENT_COUNTDOWN_TIME - 1 - m_Time) * 200, 0,
			200, TextureGetHeight(m_CountTexture));
	}
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);

	// ゲージの設定
	int gage = (m_Count[0] - m_Count[1]) * EVENT_GAGE_UPPER;

	// ゲージの描画
	SpriteDraw(m_GageTexture, (float)(SCREEN_WIDTH / 4) + (float)(gage / 2), (float)GAGEGEIGHT, (float)(SCREEN_WIDTH / 2) + gage, (float)GAGESIZE_Y, 0, 0, 50, 100);
	SpriteDraw(m_GageTexture, (float)(SCREEN_WIDTH * 3 / 4) + (float)(gage / 2), GAGEGEIGHT, (float)(SCREEN_WIDTH / 2) - gage, (float)GAGESIZE_Y, 50, 0, 50, 100);

	// ゲージのエフェクト
	SpriteSetColor(1.0f, 0.2f, 0.2f, 1.0f);
	SpriteDraw(m_GageEffect[0], (SCREEN_WIDTH / 4) + (gage / 2.0f), GAGEPOSITION_Y + GAGESIZE_Y / 2, (SCREEN_WIDTH / 2.0f) + gage, GAGESIZE_Y / 4, 0, (m_FrameCount % 2) * 40, 300, 40, 0, true);
	SpriteDraw(m_GageEffect[0], (SCREEN_WIDTH / 4) + (gage / 2.0f), GAGEPOSITION_Y - GAGESIZE_Y / 2, (SCREEN_WIDTH / 2.0f) + gage, GAGESIZE_Y / 4, 0, (m_FrameCount % 2) * 40, 300, 40, 0, true);

	SpriteSetColor(0.2f, 0.2f, 1.0f, 1.0f);
	SpriteDraw(m_GageEffect[0], (SCREEN_WIDTH * 3 / 4) + (gage / 2.0f), GAGEPOSITION_Y + GAGESIZE_Y / 2, (SCREEN_WIDTH / 2.0f) - gage, GAGESIZE_Y / 4, 0, (m_FrameCount % 2) * 40, 300, 40, 0, false);
	SpriteDraw(m_GageEffect[0], (SCREEN_WIDTH * 3 / 4) + (gage / 2.0f), GAGEPOSITION_Y - GAGESIZE_Y / 2, (SCREEN_WIDTH / 2.0f) - gage, GAGESIZE_Y / 4, 0, (m_FrameCount % 2) * 40, 300, 40, 0, false);

	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);

	SpriteDraw(m_GageEffect[1], SCREEN_WIDTH / 2.0f + gage, GAGEPOSITION_Y, 45, 125,
		(m_FrameCount % 2) * 40, 0, 40, 40);

	float shippos_y = 550.0f + sinf((float)m_FrameCount / 8.0f) * 3.0f;


	XMFLOAT2 texturesize = XMFLOAT2(0, 0);

	int shipanimcount = m_AnimCount / 10;
	int shipanimation = shipanimcount % 5;
	// 後波のテクスチャサイズ入手
	texturesize = XMFLOAT2((float)TextureGetWidth(m_WaterTexture[1]) / 5, (float)TextureGetHeight(m_WaterTexture[1]));

	if (m_State != EVENTSTATE_COUNTDOWN)
	{
		// 後波の描画
		SpriteDraw(m_WaterTexture[1], SHIPPOS_X_LEFT + 29.0f, shippos_y,
			150 * 1.1, 150 * 0.95, shipanimation * 50, 0,
			(int)texturesize.x, (int)texturesize.y, 0, true);
		SpriteDraw(m_WaterTexture[1], SHIPPOS_X_RIGHT - 29.0f, shippos_y,
			150 * 1.1, 150 * 0.95, shipanimation * 50, 0,
			(int)texturesize.x, (int)texturesize.y, 0, false);

		//船
		SpriteDraw(m_BoatTex, SHIPPOS_X_LEFT, shippos_y, 150.0f, 150.0f, shipanimation * 250, 0, TextureGetWidth(m_BoatTex) / 5, TextureGetHeight(m_BoatTex) / 2, 0, true);
		SpriteDraw(m_BoatTex, SHIPPOS_X_RIGHT, shippos_y, 150.0f, 150.0f, shipanimation * 250, 0, TextureGetWidth(m_BoatTex) / 5, TextureGetHeight(m_BoatTex) / 2, 0, false);

		// 前波のテクスチャサイズ入手
		texturesize = XMFLOAT2((float)TextureGetWidth(m_WaterTexture[0]) / 5, (float)TextureGetHeight(m_WaterTexture[0]));

		// 前波の描画
		SpriteDraw(m_WaterTexture[0], SHIPPOS_X_LEFT, shippos_y + 32.5f,
			150 * 1.1f, 150 * 0.6f, shipanimation * 50, 0,
			(int)texturesize.x, (int)texturesize.y, 0, true);
		SpriteDraw(m_WaterTexture[0], SHIPPOS_X_RIGHT, shippos_y + 32.5f,
			150 * 1.1f, 150 * 0.6f, shipanimation * 50, 0,
			(int)texturesize.x, (int)texturesize.y, 0, false);
	}

	//爆弾の大きさ
	float bombsize1 = BOMBSIZE_DEFAULT + (m_Count[0] - m_Count[1]) * 10.0f;
	float bombsize2 = BOMBSIZE_DEFAULT + (m_Count[1] - m_Count[0]) * 10.0f;

	//爆弾の大きさの上限、下限
	bombsize1 = std::min(bombsize1, BOMBSIZE_DEFAULT + 10.0f * 10.0f);
	bombsize1 = std::max(bombsize1, BOMBSIZE_DEFAULT + -10.0f * 10.0f);
	bombsize2 = std::min(bombsize2, BOMBSIZE_DEFAULT + 10.0f * 10.0f);
	bombsize2 = std::max(bombsize2, BOMBSIZE_DEFAULT + -10.0f * 10.0f);

	switch (m_State)
	{
	case EVENTSTATE_EVENT:
	{
		//爆弾
		SpriteDraw(m_BombTex, m_BombPosition[0].x, m_BombPosition[0].y, bombsize1, bombsize1, 0, 0, TextureGetWidth(m_BombTex), TextureGetHeight(m_BombTex), 0, true);
		SpriteDraw(m_BombTex, m_BombPosition[1].x, m_BombPosition[1].y, bombsize2, bombsize2, 0, 0, TextureGetWidth(m_BombTex), TextureGetHeight(m_BombTex), 0, false);

		//カウントダウン
		SpriteDraw(m_CountTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 200.0f, 200.0f, (5-m_Time) * 200, 0, 200, 200);

		if (GetGameMode() == GAMEMODE_SEGMENTATION ||
			GetGameMode() == GAMEMODE_LOCAL_SPLITSCREEN)
		{
			//ボタンアニメーションの描画
			if (m_ButtonAnimFrame <= BUTTON_ANIM_FRAME / 2)
			{
				SpriteDraw(m_ButtonTex[0], SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 100.0f, 100.0f, 0, 0, TextureGetWidth(m_ButtonTex[0]) / 8, TextureGetHeight(m_ButtonTex[0]));
				SpriteDraw(m_ButtonTex[0], SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT / 2, 100.0f, 100.0f, 0, 0, TextureGetWidth(m_ButtonTex[0]) / 8, TextureGetHeight(m_ButtonTex[0]));
			}
			else
			{
				SpriteDraw(m_ButtonTex[1], SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2+8.9f , 200.0f, 200.0f, 0, 0, TextureGetWidth(m_ButtonTex[1]) / 8, TextureGetHeight(m_ButtonTex[1]));
				SpriteDraw(m_ButtonTex[1], SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT / 2+8.9f , 200.0f, 200.0f, 0, 0, TextureGetWidth(m_ButtonTex[1]) / 8,TextureGetHeight(m_ButtonTex[1]));
			}
		}
		else
		{
			//ボタンアニメーションの描画
			if (m_ButtonAnimFrame <= BUTTON_ANIM_FRAME / 2)
			{
				SpriteDraw(m_ButtonTex[0], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 100.0f, 100.0f, 0, 0, TextureGetWidth(m_ButtonTex[0]) / 8, TextureGetHeight(m_ButtonTex[0]));
					}
			else
			{
				SpriteDraw(m_ButtonTex[1], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 8.9f, 200.0f, 200.0f, 0, 0, TextureGetWidth(m_ButtonTex[1]) / 8, TextureGetHeight(m_ButtonTex[1]));
			}
		}

		break;
	}
	case EVENTSTATE_ANIM:
	{
		switch (m_AnimState)
		{
		case MASHSTATE_BOMBMOVE:
			SpriteDraw(m_BombTex, m_BombPosition[0].x, m_BombPosition[0].y, bombsize1, bombsize1, 0, 0, TextureGetWidth(m_BombTex), TextureGetHeight(m_BombTex), 0, true);
			SpriteDraw(m_BombTex, m_BombPosition[1].x, m_BombPosition[1].y, bombsize2, bombsize2, 0, 0, TextureGetWidth(m_BombTex), TextureGetHeight(m_BombTex), 0, false);
			break;

		case MASHSTATE_BLOWOFF:
			if (m_Count[0] > m_Count[1])
			{
				SpriteDraw(m_BombTex, m_BombPosition[0].x, m_BombPosition[0].y, bombsize1, bombsize1, 0, 0, TextureGetWidth(m_BombTex), TextureGetHeight(m_BombTex), 0, true);
				SpriteDraw(m_BombTex, m_BombPosition[1].x, m_BombPosition[1].y, bombsize2, bombsize2, 0, 0, TextureGetWidth(m_BombTex), TextureGetHeight(m_BombTex), m_Angle, false);
			}
			else if (m_Count[1] > m_Count[0])
			{
				SpriteDraw(m_BombTex, m_BombPosition[0].x, m_BombPosition[0].y, bombsize1, bombsize1, 0, 0, TextureGetWidth(m_BombTex), TextureGetHeight(m_BombTex), m_Angle, true);
				SpriteDraw(m_BombTex, m_BombPosition[1].x, m_BombPosition[1].y, bombsize2, bombsize2, 0, 0, TextureGetWidth(m_BombTex), TextureGetHeight(m_BombTex), 0, false);
			}
			else
			{
				SpriteDraw(m_BombTex, m_BombPosition[0].x, m_BombPosition[0].y, bombsize1, bombsize1, 0, 0, TextureGetWidth(m_BombTex), TextureGetHeight(m_BombTex), m_Angle, true);
				SpriteDraw(m_BombTex, m_BombPosition[1].x, m_BombPosition[1].y, bombsize2, bombsize2, 0, 0, TextureGetWidth(m_BombTex), TextureGetHeight(m_BombTex), m_Angle, false);
			}
			break;

		case MASHSTATE_EXPLOSION:
			if (m_Count[0] > m_Count[1])
			{
				SpriteDraw(m_BombTex, m_BombPosition[1].x, m_BombPosition[1].y, bombsize2, bombsize2, 0, 0, TextureGetWidth(m_BombTex), TextureGetHeight(m_BombTex), m_Angle, false);
			}
			else if (m_Count[1] > m_Count[0])
			{
				SpriteDraw(m_BombTex, m_BombPosition[0].x, m_BombPosition[0].y, bombsize1, bombsize1, 0, 0, TextureGetWidth(m_BombTex), TextureGetHeight(m_BombTex), m_Angle, true);
			}

			for (int i = 0; i < EFFECT_MAX; i++)
			{
				if (m_Effect[i])
					m_Effect[i]->Draw();
			}
			break;
		default:
			break;
		}

		

		break;
	}
	default:
		break;
	}

	SetDepthEnable(true);
}

//===================================================
// 船へのダメージ
//===================================================
void MashEvent::Damage(int shipid, int damage)
{
	std::random_device rd;

	//ダメージ分繰り返す
	for (int i = 0; i < damage; i++)
	{
		//マップの数が最大値の乱数取得
		int deckid = rd() % MAP_MAX_NUM;

		//破壊対象でないor対象のブロックが既に破壊されていたら
		if (GetShip()[shipid]->GetDeckBlocks()[deckid]->GetDeckType() != DECKTYPE_WOOD || GetShip()[shipid]->GetDeckBlocks()[deckid]->GetHitPoint() <= 0)
			i--;
		//破壊対象だったら
		else
			GetShip()[shipid]->GetDeckBlocks()[deckid]->Damage(100); //確定破壊
	}

	for (int j = 0; j < GetShip()[shipid]->GetPlayers().size(); j++)
	{
		//GetShip()[shipid]->GetPlayers()[j]->SetBlowOff();
		GetShip()[shipid]->GetPlayers()[j]->GetCamera()->SetCameraShake(10.0f);
		GetShip()[shipid]->GetPlayers()[j]->GetController()->Controller_SetVibration(16000, 16000, 2000);
	}
}

//===================================================
// イベント中の更新
//===================================================
void MashEvent::EventUpdate()
{
	if (GetGameMode()==GAMEMODE_ONLINE)
	{
		m_Time = m_FrameCount / 60;
		if (m_Time >= 5)
		{
			m_Time = 0;
			m_FrameCount = 0;
			m_State = EVENTSTATE_ANIM_WAIT;
			m_AnimState = MASHSTATE_BOMBMOVE;

		}

		m_BombPosition[0].x = SCREEN_WIDTH / 2 - 47.0f + sinf((float)m_FrameCount / 15.0f) * 85.0f;
		m_BombPosition[1].x = SCREEN_WIDTH / 2 + 47.0f + sinf((float)m_FrameCount / 15.0f) * 85.0f;

		// ABXY のいずれかが押されたら
		if ((GetController().front()->Controller_IsJustPressed(GetController().front()->GetButtonForTrigger(XINPUT_GAMEPAD_B)))&&
		GetController().front()->IsConnected())
		{
			// カウントをサーバーに送る
			EventPlayerData data;
			data.teamid = GetClient()->GetMyTeamId();
			data.eventcount = 1;
			data.readyflag = EVENTPLAYERREADY_PLAY;

			GetClient()->SendEventData(data);

			SEStart(GetAppSE(), CRI_SE_ボタン連打);
		}
			
		////キーボード入力(デバッグ用)
		//if (Keyboard_IsKeyTrigger(KK_A) && !GetController()[0]->IsConnected()) {
		//	m_Count[0]++;
		//}
		//if (Keyboard_IsKeyTrigger(KK_D) && !GetController()[1]->IsConnected()) {
		//	m_Count[1]++;
		//}

		m_ButtonAnimFrame++;

		if (m_ButtonAnimFrame > BUTTON_ANIM_FRAME)
			m_ButtonAnimFrame = 0;

		m_FrameCount++;
	}
	else
	{
		m_Time = m_FrameCount / 60;
		if (m_Time >= 5)
		{
			m_Time = 0;
			m_FrameCount = 0;
			m_State = EVENTSTATE_ANIM;
			m_AnimState = MASHSTATE_BOMBMOVE;
		}

		m_BombPosition[0].x = SCREEN_WIDTH / 2 - 47.0f + sinf((float)m_FrameCount / 15.0f) * 85.0f;
		m_BombPosition[1].x = SCREEN_WIDTH / 2 + 47.0f + sinf((float)m_FrameCount / 15.0f) * 85.0f;

		for (int i = 0; i < GetShip().size(); i++) {
			for (int j = 0; j < GetShip()[i]->GetPlayers().size(); j++)
			{
				if (GetShip()[i]->GetPlayers()[j]->GetController()->Controller_IsJustPressed(GetShip()[i]->GetPlayers()[j]->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_B)) ||
					GetShip()[i]->GetPlayers()[j]->GetController()->Controller_IsJustPressed(GetShip()[i]->GetPlayers()[j]->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_A)) ||
					GetShip()[i]->GetPlayers()[j]->GetController()->Controller_IsJustPressed(GetShip()[i]->GetPlayers()[j]->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_X)) ||
					GetShip()[i]->GetPlayers()[j]->GetController()->Controller_IsJustPressed(GetShip()[i]->GetPlayers()[j]->GetController()->GetButtonForTrigger(XINPUT_GAMEPAD_Y)) &&
					GetShip()[i]->GetPlayers()[j]->GetController()->IsConnected())
				{
					if (GetShip()[i]->GetTeamId() == 0)
					{
						m_Count[0]++;
						SEStart(GetAppSE(), CRI_SE_ボタン連打);
					}
					else
					{
						m_Count[1]++;
						SEStart(GetAppSE(), CRI_SE_ボタン連打);
					}
				}
			}
		}
		//キーボード入力(デバッグ用)
		if (Keyboard_IsKeyTrigger(KK_A) && !GetController()[0]->IsConnected())
			m_Count[0]++;
		if (Keyboard_IsKeyTrigger(KK_D) && !GetController()[1]->IsConnected())
			m_Count[1]++;

		m_ButtonAnimFrame++;

		if (m_ButtonAnimFrame > BUTTON_ANIM_FRAME)
			m_ButtonAnimFrame = 0;

		m_FrameCount++;
	}
}

//===================================================
// イベント終了後のアニメーションの更新
//===================================================
void MashEvent::AnimUpdate()
{
	m_Time = m_FrameCount / 60;

	switch (m_AnimState)
	{
	case MASHSTATE_BOMBMOVE:
		m_BombSpeed += 0.2f;

		m_BombPosition[0].x = SCREEN_WIDTH / 2 - 150.0f + easeInBack(m_BombSpeed) / 2.0f;
		m_BombPosition[1].x = SCREEN_WIDTH / 2 + 150.0f - easeInBack(m_BombSpeed) / 2.0f;

		if (m_BombPosition[0].x >= SCREEN_WIDTH / 2 - 47.0f)
			m_AnimState = MASHSTATE_BLOWOFF;
		
		break;

	case MASHSTATE_BLOWOFF:
		m_BombSpeed += 0.2f;
		m_Angle += 40.0f;

		if (m_Count[0] > m_Count[1])
		{
			m_BombPosition[0].x = SCREEN_WIDTH / 2 - 150.0f + easeInBack(m_BombSpeed) / 2.0f;
			m_BombPosition[1].x += BOMB_VELOCITY.x;
			m_BombPosition[1].y -= BOMB_VELOCITY.y;

			if (m_BombPosition[0].x >= SHIPPOS_X_RIGHT)
			{
				m_Effect[0] = new Effect2D(m_ExplosionTex, XMFLOAT2(SHIPPOS_X_RIGHT, 550.0f), XMFLOAT2(900.0f, 900.0f), 5, 6, 6, EFFECTTYPE_NOMAL);
				m_AnimState = MASHSTATE_EXPLOSION;

				SEStart(GetAppSE(), CRI_SE_イベント爆発);
			}
		}
		else if (m_Count[0] < m_Count[1])
		{
			m_BombPosition[1].x = SCREEN_WIDTH / 2 + 150.0f - easeInBack(m_BombSpeed) / 2.0f;
			m_BombPosition[0].x -= BOMB_VELOCITY.x;
			m_BombPosition[0].y -= BOMB_VELOCITY.y;

			if (m_BombPosition[1].x <= SHIPPOS_X_LEFT)
			{
				m_Effect[0] = new Effect2D(m_ExplosionTex, XMFLOAT2(SHIPPOS_X_LEFT, 550.0f), XMFLOAT2(900.0f, 900.0f), 5, 6, 6, EFFECTTYPE_NOMAL);
				m_AnimState = MASHSTATE_EXPLOSION;

				SEStart(GetAppSE(), CRI_SE_イベント爆発);
			}
		}
		else
		{
			m_BombPosition[0].x -= BOMB_VELOCITY.x;
			m_BombPosition[1].x += BOMB_VELOCITY.x;

			if (m_BombPosition[0].x <= SHIPPOS_X_LEFT)
			{
				m_Effect[0] = new Effect2D(m_ExplosionTex, XMFLOAT2(SHIPPOS_X_LEFT, 550.0f), XMFLOAT2(800.0f, 800.0f), 5, 6, 6, EFFECTTYPE_NOMAL);
				m_Effect[1] = new Effect2D(m_ExplosionTex, XMFLOAT2(SHIPPOS_X_RIGHT, 550.0f), XMFLOAT2(800.0f, 800.0f), 5, 6, 6, EFFECTTYPE_NOMAL);
				m_AnimState = MASHSTATE_EXPLOSION;

				SEStart(GetAppSE(), CRI_SE_イベント爆発);
			}
		}

		break;

	case MASHSTATE_EXPLOSION:
		m_Angle += 40.0f;
		if (m_Count[0] > m_Count[1])
		{
			m_BombPosition[1].x += BOMB_VELOCITY.x;
			m_BombPosition[1].y -= BOMB_VELOCITY.y;
		}
		else if (m_Count[0] < m_Count[1])
		{
			m_BombPosition[0].x -= BOMB_VELOCITY.x;
			m_BombPosition[0].y -= BOMB_VELOCITY.y;
		}

		for (int i = 0; i < EFFECT_MAX; i++)
		{
			if (m_Effect[i])
				m_Effect[i]->Update();
		}
		break;

	default:
		break;
	}

	if (m_Time == 3)
	{
		for (int i = 0; i < EFFECTTYPE_STOP; i++)
		{
			delete m_Effect[i];
			m_Effect[i] = nullptr;
		}

		if (GetGameMode() != GAMEMODE_ONLINE) {
			m_State = EVENTSTATE_DAMAGE;
		}
		else {
			m_State = EVENTSTATE_DAMAGE_WAIT;
		}
	}
	m_FrameCount++;

}

void MashEvent::CountUpdate()
{
	// タイマー　= 1秒カウント
	m_Time = m_FrameCount / 60;

	float count = m_FrameCount * 0.05f;
	m_CountSize = easeInOutBack(count);
	m_CountSize = sinf(count);
	m_CountSize = m_CountSize + 1.0f;

	if (m_Time == EVENT_COUNTDOWN_TIME)
	{
		m_State = EVENTSTATE_EVENT;
		m_Time = 0;
		m_FrameCount = 0;
	}
	// フレームカウント
	m_FrameCount++;
}

void MashEvent::DamageUpdate()
{
	// オンラインじゃなければ
	if (GetGameMode() != GAMEMODE_ONLINE)
	{
		if (m_Count[0] > m_Count[1])
			Damage(1, DAMAGE);
		else if (m_Count[0] < m_Count[1])
			Damage(0, DAMAGE);
		else // 引き分け時のダメージ処理
		{
			for (int i = 0; i < 2; i++)
				Damage(i, DAMAGE / 2); //両チームに半分ずつダメージ
		}
	SetEndFlag(true); //イベント終了
	}
	else // オンライン時
	{
		m_State = EVENTSTATE_END_WAIT;
		GetShip()[GetClient()->GetMyTeamId()]->GetPlayers().front()->GetCamera()->SetCameraShake(10.0f);
		GetShip()[GetClient()->GetMyTeamId()]->GetPlayers().front()->GetController()->Controller_SetVibration(16000, 16000, 2000);
	}
}
