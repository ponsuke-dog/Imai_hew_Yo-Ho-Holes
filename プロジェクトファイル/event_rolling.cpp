#include "configuration.h"
#include "ship.h"
#include "deckblock.h"
#include "texture.h"
#include "sprite.h"
#include "deck.h"
#include "cri.h"
#include "mode.h"
#include <random>
#include <math.h> 
#include "player.h"
#include "camera_player.h"
#include "controller.h"
#include "easing.h"
#include "client.h"
#include "event_rolling.h"

#include "keyboard.h"
// 甲板ダメージ数
#define DAMAGE (15)

// 船の吹っ飛ばされ高さ
#define SHIPVELO_Y (-30.0f)

// 船のデフォルトサイズ
#define SHIPSIZE (XMFLOAT2(200.0f,200.0f))

// 船のサイズ上昇幅
#define SHIPUPPER (10.0f)

static int e = 1;

static bool SE_flag = false;


RollingEvent::RollingEvent(std::vector<Ship*> pShip, std::vector<Controller*> pController):Event(pShip,pController)
{
	SetEndFlag(false);
	// 船のカウント
	m_Count[0] = 0;
	m_Count[1] = 0;

	// 時間
	m_FrameCount = 0;
	m_Time = 0;

	//船のアニメーション座標 
	m_ShipPosition[0] = XMFLOAT2(SCREEN_WIDTH / 4, -100.0f);
	m_ShipPosition[1] = XMFLOAT2(SCREEN_WIDTH * 3 / 4, -100.0f);

	// テクスチャ
	
	// 
	m_EventBGTexture = TextureLoad(L"asset\\texture\\black.png");
	m_State = EVENTSTATE_COUNTDOWN;

	// ゲージ
	m_Texture[0] = TextureLoad(L"asset\\texture\\eventgage.png");

	// 
	m_Texture[1] = TextureLoad(L"asset\\texture\\button_move_circle.png");
	// 
	m_Texture[2] = TextureLoad(L"asset\\texture\\number_sansBold 3.png");
	// 
	m_ShipTexture = TextureLoad(L"asset\\texture\\Boat.png");
	// 
	m_WaterTexture[0] = TextureLoad(L"asset\\texture\\waveFront.png");
	m_WaterTexture[1] = TextureLoad(L"asset\\texture\\waveAnimation.png");
	// 
	m_GageEffect[0] = TextureLoad(L"asset\\texture\\thunder.png");
	m_GageEffect[1] = TextureLoad(L"asset\\texture\\ConpetitionAnimation.png");
	// 
	m_InstructionTex = TextureLoad(L"asset\\texture\\Rolling!.png");

	//初期化処理
	m_Speed = 0;
	m_ShipRotate = 0;
	m_Rotate = 0;
	m_ShipVelocity = XMFLOAT2(0.0f, 0.0f);
	e = 1;
	m_AnimationTime = 0;
	m_KeyboardRollingFlag = false;

	m_StartRollingDirection = KEYBOARD_NONE;
	m_OldRollingDirection = KEYBOARD_NONE;
	m_NextRollingDirection = KEYBOARD_NONE;

	SE_flag = false;
}


RollingEvent::~RollingEvent()
{
}

void RollingEvent::Update()
{
	if (GetGameMode() == GAMEMODE_ONLINE) {
		m_Count[0] = GetClient()->GetEventData().count[0];
		m_Count[1] = GetClient()->GetEventData().count[1];
	}

	// イベント中なら
	if (!GetEndFlag())
	{
		switch (m_State)
		{
		case EVENTSTATE_COUNTDOWN:
			CountUpdate();		// カウントダウン
			break;
		case EVENTSTATE_EVENT:	// 入力イベント
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
		case EVENTSTATE_ANIM:	// アニメーションイベント
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
		case EVENTSTATE_DAMAGE:	// ダメージイベント
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
	}
}

void RollingEvent::Draw(int)
{
	SetDepthEnable(false);
	
	// 背景の半透明黒
	SpriteSetColor(1.0f, 1.0f, 1.0f, 0.4f);
	SpriteDraw(m_EventBGTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
	

	// ゲージの設定
	int gage = (m_Count[0] - m_Count[1]) * EVENT_GAGE_UPPER;

	

	// ゲージの描画
	SpriteDraw(m_Texture[0], (float)(SCREEN_WIDTH / 4) + (gage / 2), GAGEPOSITION_Y, (float)(SCREEN_WIDTH / 2) + gage, (float)GAGESIZE_Y, 0, 0, 50, 100);
	SpriteDraw(m_Texture[0], (float)(SCREEN_WIDTH * 3 / 4) + (gage / 2), GAGEPOSITION_Y, (float)(SCREEN_WIDTH / 2) - gage, (float)GAGESIZE_Y, 50, 0, 50, 100);

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

	XMINT2 texturesize = XMINT2(0, 0);

	// イベントアニメーション処理
	int ship_animation = m_AnimationTime % 5;

	// 後波のテクスチャサイズ入手
	texturesize = XMINT2(TextureGetWidth(m_WaterTexture[1]) / 5, TextureGetHeight(m_WaterTexture[1]));

	// 後波の描画
	if (m_ShipPosition[0].y >= SCREEN_HEIGHT * 3 / 4 - 10.0f) // 左の船
	{
		SpriteDraw(m_WaterTexture[1], m_ShipPosition[0].x + 29.0f, SCREEN_HEIGHT * 3 / 4 + 10.0f,
			(SHIPSIZE.x + SHIPUPPER * m_Count[0]) * 1.1f, (SHIPSIZE.y + SHIPUPPER * m_Count[0]) * 0.95f, ship_animation * 50, 0,
			texturesize.x, texturesize.y, 0, true);
	}
	if (m_ShipPosition[1].y >= SCREEN_HEIGHT * 3 / 4 - 10.0f) // 左の船
	{
		SpriteDraw(m_WaterTexture[1], m_ShipPosition[1].x - 29.0f, SCREEN_HEIGHT * 3 / 4 + 10.0f,
			(SHIPSIZE.x + SHIPUPPER * m_Count[1]) * 1.1f, (SHIPSIZE.y + SHIPUPPER * m_Count[1]) * 0.95f, ship_animation * 50, 0,
			texturesize.x, texturesize.y, 0, false);
	}
	// 後波のテクスチャサイズ入手
	texturesize = XMINT2(TextureGetWidth(m_ShipTexture) / 5, TextureGetHeight(m_ShipTexture) / 2);

	// 船の描画
	SpriteDraw(m_ShipTexture, m_ShipPosition[0].x, m_ShipPosition[0].y,
		SHIPSIZE.x + SHIPUPPER * m_Count[0], SHIPSIZE.y + SHIPUPPER * m_Count[0], ship_animation*250, 0,
		texturesize.x,texturesize.y , m_ShipRotate, true);
	// 船の描画
	SpriteDraw(m_ShipTexture, m_ShipPosition[1].x, m_ShipPosition[1].y,
		SHIPSIZE.x + SHIPUPPER * m_Count[1], SHIPSIZE.y + SHIPUPPER * m_Count[1], ship_animation*250, 200, 
		texturesize.x, texturesize.y, m_ShipRotate, false);

	// 後波のテクスチャサイズ入手
	texturesize = XMINT2(TextureGetWidth(m_WaterTexture[0]) / 5, TextureGetHeight(m_WaterTexture[0]));
	
	// 前波の描画
	if (m_ShipPosition[0].y >= SCREEN_HEIGHT * 3 / 4 - 10.0f) // 左の船
	{
		SpriteDraw(m_WaterTexture[0], m_ShipPosition[0].x, SCREEN_HEIGHT * 3 / 4 + 65.0f,
			(SHIPSIZE.x + SHIPUPPER * m_Count[0]) * 1.1f, (SHIPSIZE.y + SHIPUPPER * m_Count[0]) * 0.6f, ship_animation * 50, 0,
			texturesize.x, texturesize.y, 0, true);
	}
	if (m_ShipPosition[1].y >= SCREEN_HEIGHT * 3 / 4 - 10.0f) // 右の船
	{
		SpriteDraw(m_WaterTexture[0], m_ShipPosition[1].x, SCREEN_HEIGHT * 3 / 4 + 65.0f,
			(SHIPSIZE.x + SHIPUPPER * m_Count[1]) * 1.1f, (SHIPSIZE.y + SHIPUPPER * m_Count[1]) * 0.6f, ship_animation * 50, 0,
			texturesize.x, texturesize.y, 0, false);
	}

	switch (m_State)
	{
	case EVENTSTATE_COUNTDOWN:
		SpriteSetColor(1.0f, 1.0f, 1.0f, 0.4f);
		SpriteDraw(m_EventBGTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);
		SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
		SpriteDraw(m_InstructionTex, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, m_CountSize * 50.0f + 1000.0f, m_CountSize * 50.0f + 200.0f);

		SpriteDraw(m_Texture[2], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4,
			m_CountSize * 10.0f + 200.0f, m_CountSize * 10.0f + 200.0f,
			(EVENT_COUNTDOWN_TIME - 1 - m_Time) * 200, 0,
			200, TextureGetHeight(m_Texture[2]));
		break;

	case EVENTSTATE_EVENT:
		SpriteSetColor(2.0f, 2.0f,2.0f, 1.0f);
		XMINT2 animation = XMINT2(m_AnimationTime % 4, m_AnimationTime / 4);
		texturesize = XMINT2(TextureGetWidth(m_Texture[1])/4, TextureGetHeight(m_Texture[1])/2);
		if (GetGameMode() == GAMEMODE_SOLO ||
			GetGameMode() == GAMEMODE_LOCAL_MULTIWINDOW ||
			GetGameMode() == GAMEMODE_MULTIWINDOW ||
			GetGameMode() == GAMEMODE_ONLINE)
		{
			SpriteDraw(m_Texture[1], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, (float)texturesize.x * 4, (float)texturesize.y * 4,
				animation.x * texturesize.x, animation.y * texturesize.y, texturesize.x, texturesize.y);
		}
		else
		{
			SpriteDraw(m_Texture[1], SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, (float)(texturesize.x) * 4, (float)texturesize.y * 4,
				animation.x * texturesize.x, animation.y * texturesize.y, texturesize.x, texturesize.y);
			SpriteDraw(m_Texture[1], SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT / 2, (float)(texturesize.x) * 4, (float)texturesize.y * 4,
				animation.x * texturesize.x, animation.y * texturesize.y, texturesize.x, texturesize.y);
		}
		SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);

		// カウントダウン
		SpriteDraw(m_Texture[2], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4,
			100, 100,
			(5 - m_Time) * 200, 0,
			200, TextureGetHeight(m_Texture[2]));
		break;
	case EVENTSTATE_ANIM:
		break;
	default:
		break;
	}

	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetDepthEnable(true);
}

void RollingEvent::Damage(int shipid, int damage)
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
		GetShip()[shipid]->GetPlayers()[j]->SetBlowOff(240);
		GetShip()[shipid]->GetPlayers()[j]->GetCamera()->SetCameraShake(10.0f);
		GetShip()[shipid]->GetPlayers()[j]->GetController()->Controller_SetVibration(16000, 16000, 2000);
	}

}


void RollingEvent::CountUpdate()
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

void RollingEvent::EventUpdate()
{
	if (GetGameMode() == GAMEMODE_ONLINE)
	{
		// タイマー　= 1秒カウント
		m_Time = m_FrameCount / 60;

		if (m_Time == 5)// 5秒で終了
		{
			// 入力処理を終了
			m_State = EVENTSTATE_ANIM_WAIT;

			m_Time = 0;
			m_FrameCount = 0;

		}

		// 右スティック回転でカウント++ or 左スティック回転でカウント++
		if (GetController().front()->Controller_IsFullRotation_RightStick() ||
			GetController().front()->Controller_IsFullRotation_LeftStick())
		{
			EventPlayerData data;
			data.teamid = GetClient()->GetMyTeamId();
			data.eventcount = 1;
			data.readyflag = EVENTPLAYERREADY_PLAY;

			GetClient()->SendEventData(data);

			SEStart(GetAppSE(), CRI_SE_気を溜める1);
		}
		
		// フレームカウント
		m_FrameCount++;

		// 船の上下アニメーション
		m_ShipPosition[0].y = (SCREEN_HEIGHT * 3 / 4 + sinf((float)m_FrameCount / 5) * 3);
		m_ShipPosition[1].y = (SCREEN_HEIGHT * 3 / 4 + sinf((float)m_FrameCount / 5) * 3);

		// スティック回転のアニメーション
		m_AnimationTime = m_FrameCount / 10;
	}
	else
	{

		// タイマー　= 1秒カウント
		m_Time = m_FrameCount / 60;

		if (m_Time == 5)// 5秒で終了
		{
			// 入力処理を終了
			m_State = EVENTSTATE_ANIM;

			m_Time = 0;
			m_FrameCount = 0;
		}
		// コントローラーの数分繰り返す
		for (int i = 0; i < GetShip().size(); i++)
		{
			for (int j = 0; j < GetShip()[i]->GetPlayers().size(); j++)
			{
				// 右スティック回転でカウント++ or 左スティック回転でカウント++
				if (GetShip()[i]->GetPlayers()[j]->GetController()->Controller_IsFullRotation_RightStick() ||
					GetShip()[i]->GetPlayers()[j]->GetController()->Controller_IsFullRotation_LeftStick())
				{

					// コントローラーの総数のうち半分はチーム1
					if (GetShip()[i]->GetPlayers()[j]->GetShip()->GetTeamId() == 0)
					{
						m_Count[0]++;
						SEStart(GetAppSE(), CRI_SE_気を溜める1);
						continue;
					}
					else // コントローラーの総数のうち半分はチーム2
					{
						m_Count[1]++;
						SEStart(GetAppSE(), CRI_SE_気を溜める1);
						continue;
					}
				}
				// デバッグ用キーボード操作
				if (RollingKeyboard())
				{
					m_Count[1]++;
				}
			}
		}
		
		// フレームカウント
		m_FrameCount++;

		m_ShipPosition[0].y = (SCREEN_HEIGHT * 3 / 4 + sinf((float)m_FrameCount / 5) * 3);
		m_ShipPosition[1].y = (SCREEN_HEIGHT * 3 / 4 + sinf((float)m_FrameCount / 5) * 3);

		// スティック回転のアニメーション
		m_AnimationTime = m_FrameCount / 10;
	}
}

void RollingEvent::AnimUpdate()
{
	m_ShipVelocity.x = easeInBack((float)m_Speed / 5) / 180;

	m_Speed++;
	m_Time = m_FrameCount / 60;
	m_FrameCount++;


	if (m_Count[0] < m_Count[1])
	{
		if (m_ShipPosition[0].x >= SCREEN_WIDTH / 2)
		{
			e = -1;
			m_ShipVelocity.y += SHIPVELO_Y;
			m_ShipRotate += 10;

			if (SE_flag == false)
			{
				SE_flag = true;
				SEStart(GetAppSE(), CRI_SE_イベント吹っ飛ぶ);
			}
		}

		m_ShipPosition[0].x += m_ShipVelocity.x * e;
		m_ShipPosition[1].x -= m_ShipVelocity.x;

		m_ShipPosition[0].y += m_ShipVelocity.y;
	}
	else if (m_Count[0] > m_Count[1])
	{
		if (m_ShipPosition[1].x <= SCREEN_WIDTH / 2)
		{
			e = -1;
			m_ShipVelocity.y += SHIPVELO_Y;
			m_ShipRotate += 10;

			if (SE_flag == false)
			{
				SE_flag = true;
				SEStart(GetAppSE(), CRI_SE_イベント吹っ飛ぶ);
			}
		}
		m_ShipPosition[0].x += m_ShipVelocity.x;
		m_ShipPosition[1].x -= m_ShipVelocity.x * e;

		m_ShipPosition[1].y += m_ShipVelocity.y;
	}
	else
	{
		if (m_ShipPosition[0].x >= SCREEN_WIDTH / 2 || m_ShipPosition[1].x <= SCREEN_WIDTH / 2)
		{
			e = -1;
			m_ShipVelocity.y += SHIPVELO_Y;
			m_ShipRotate += 10;

			if (SE_flag == false)
			{
				SE_flag = true;
				SEStart(GetAppSE(), CRI_SE_イベント吹っ飛ぶ);
			}
		}
		m_ShipPosition[0].x += m_ShipVelocity.x * e;
		m_ShipPosition[1].x -= m_ShipVelocity.x * e;

		m_ShipPosition[0].y += m_ShipVelocity.y;
		m_ShipPosition[1].y += m_ShipVelocity.y;
	}

	if (m_Time == 3)
	{
		if (GetGameMode() != GAMEMODE_ONLINE) {
			m_State = EVENTSTATE_DAMAGE;
		}
		else {
			m_State = EVENTSTATE_DAMAGE_WAIT;
		}
	}

	m_AnimationTime = m_FrameCount / 10;
}

void RollingEvent::DamageUpdate()
{
	// オンラインじゃない時
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
	// イベントの終了
	SetEndFlag(true);
	}
	else // オンライン時
	{
		int winteam;
		if (m_Count[0] > m_Count[1]) {
			winteam = 0;
		}
		else if (m_Count[0] < m_Count[1]) {
			winteam = 1;
		}
		else // 引き分け時のダメージ処理
		{
			winteam =-1;
		}

		if (winteam != GetClient()->GetMyTeamId() || winteam == -1) {
			GetShip()[GetClient()->GetMyTeamId()]->GetPlayers().front()->SetBlowOff(240);
			GetShip()[GetClient()->GetMyTeamId()]->GetPlayers().front()->GetCamera()->SetCameraShake(10.0f);
			GetShip()[GetClient()->GetMyTeamId()]->GetPlayers().front()->GetController()->Controller_SetVibration(16000, 16000, 2000);
		}
		m_State = EVENTSTATE_END_WAIT;
	}
	// オンライン時にプレイヤーを吹っ飛ばす処理はチームごとのカウントを貰えるようになってから
}

bool RollingEvent::RollingKeyboard()
{
	if (!m_KeyboardRollingFlag)
	{
		if (Keyboard_IsKeyTrigger(KK_W))
		{
			m_StartRollingDirection = KEYBOARD_W;

			m_OldRollingDirection = KEYBOARD_W;

			m_KeyboardRollingFlag = true;
		}
		if (Keyboard_IsKeyTrigger(KK_A))
		{
			m_StartRollingDirection = KEYBOARD_A;

			m_OldRollingDirection = KEYBOARD_A;

			m_KeyboardRollingFlag = true;
		}
		if (Keyboard_IsKeyTrigger(KK_S))
		{
			m_StartRollingDirection = KEYBOARD_S;

			m_OldRollingDirection = KEYBOARD_S;

			m_KeyboardRollingFlag = true;
		}
		if (Keyboard_IsKeyTrigger(KK_D))
		{
			m_StartRollingDirection = KEYBOARD_D;

			m_OldRollingDirection = KEYBOARD_D;

			m_KeyboardRollingFlag = true;
		}

	}
	else
	{
		if (Keyboard_IsKeyTrigger(KK_W))
		{
			//初期の飛び対策
			if (m_StartRollingDirection == KEYBOARD_S && m_OldRollingDirection == KEYBOARD_S)
			{
				m_StartRollingDirection = KEYBOARD_NONE;

				m_OldRollingDirection = KEYBOARD_NONE;

				m_NextRollingDirection = KEYBOARD_NONE;

				m_KeyboardRollingFlag = false;

				return false;
			}

			//ローロング二回目の判定
			if (m_NextRollingDirection == KEYBOARD_NONE && m_StartRollingDirection == KEYBOARD_A)
			{
				m_NextRollingDirection = KEYBOARD_D;

				m_OldRollingDirection = KEYBOARD_W;

				return false;
			}
			else if (m_NextRollingDirection == KEYBOARD_NONE && m_StartRollingDirection == KEYBOARD_D)
			{
				m_NextRollingDirection = KEYBOARD_A;

				m_OldRollingDirection = KEYBOARD_W;

				return false;
			}

			//三回目のローリングの判定
			if (m_NextRollingDirection == KEYBOARD_W && m_OldRollingDirection == KEYBOARD_A && m_StartRollingDirection != KEYBOARD_D)
			{
				m_NextRollingDirection = KEYBOARD_D;

				m_OldRollingDirection = KEYBOARD_W;

				return false;
			}
			else if (m_NextRollingDirection == KEYBOARD_W && m_OldRollingDirection == KEYBOARD_D && m_StartRollingDirection != KEYBOARD_A)
			{
				m_NextRollingDirection = KEYBOARD_A;

				m_OldRollingDirection = KEYBOARD_W;

				return false;
			}

			//四回目のローリングの判定
			if (m_NextRollingDirection == KEYBOARD_W && m_OldRollingDirection == KEYBOARD_A && m_StartRollingDirection == KEYBOARD_D)
			{
				m_StartRollingDirection = KEYBOARD_NONE;

				m_OldRollingDirection = KEYBOARD_NONE;

				m_NextRollingDirection = KEYBOARD_NONE;

				m_KeyboardRollingFlag = false;

				return true;
			}
			else if (m_NextRollingDirection == KEYBOARD_W && m_OldRollingDirection == KEYBOARD_D && m_StartRollingDirection == KEYBOARD_A)
			{
				m_StartRollingDirection = KEYBOARD_NONE;

				m_OldRollingDirection = KEYBOARD_NONE;

				m_NextRollingDirection = KEYBOARD_NONE;

				m_KeyboardRollingFlag = false;

				return true;
			}

		}
		if (Keyboard_IsKeyTrigger(KK_A))
		{
			//初期の飛び対策
			if (m_StartRollingDirection == KEYBOARD_D && m_OldRollingDirection == KEYBOARD_D)
			{
				m_StartRollingDirection = KEYBOARD_NONE;

				m_OldRollingDirection = KEYBOARD_NONE;

				m_NextRollingDirection = KEYBOARD_NONE;

				m_KeyboardRollingFlag = false;

				return false;
			}

			//ローロング二回目の判定
			if (m_NextRollingDirection == KEYBOARD_NONE && m_StartRollingDirection == KEYBOARD_W)
			{
				m_NextRollingDirection = KEYBOARD_S;

				m_OldRollingDirection = KEYBOARD_A;

				return false;
			}
			else if (m_NextRollingDirection == KEYBOARD_NONE && m_StartRollingDirection == KEYBOARD_S)
			{
				m_NextRollingDirection = KEYBOARD_W;

				m_OldRollingDirection = KEYBOARD_A;

				return false;
			}

			//三回目のローリングの判定
			if (m_NextRollingDirection == KEYBOARD_A && m_OldRollingDirection == KEYBOARD_W && m_StartRollingDirection != KEYBOARD_S)
			{
				m_NextRollingDirection = KEYBOARD_S;

				m_OldRollingDirection = KEYBOARD_A;

				return false;
			}
			else if (m_NextRollingDirection == KEYBOARD_A && m_OldRollingDirection == KEYBOARD_S && m_StartRollingDirection != KEYBOARD_W)
			{
				m_NextRollingDirection = KEYBOARD_W;

				m_OldRollingDirection = KEYBOARD_A;

				return false;
			}

			//四回目のローリングの判定
			if (m_NextRollingDirection == KEYBOARD_A && m_OldRollingDirection == KEYBOARD_W && m_StartRollingDirection == KEYBOARD_S)
			{
				m_StartRollingDirection = KEYBOARD_NONE;

				m_OldRollingDirection = KEYBOARD_NONE;

				m_NextRollingDirection = KEYBOARD_NONE;

				m_KeyboardRollingFlag = false;

				return true;
			}
			else if (m_NextRollingDirection == KEYBOARD_A && m_OldRollingDirection == KEYBOARD_S && m_StartRollingDirection == KEYBOARD_W)
			{
				m_StartRollingDirection = KEYBOARD_NONE;

				m_OldRollingDirection = KEYBOARD_NONE;

				m_NextRollingDirection = KEYBOARD_NONE;

				m_KeyboardRollingFlag = false;

				return true;
			}
		}
		if (Keyboard_IsKeyTrigger(KK_S))
		{
			//初期の飛び対策
			if (m_StartRollingDirection == KEYBOARD_W && m_OldRollingDirection == KEYBOARD_W)
			{
				m_StartRollingDirection = KEYBOARD_NONE;

				m_OldRollingDirection = KEYBOARD_NONE;

				m_NextRollingDirection = KEYBOARD_NONE;

				m_KeyboardRollingFlag = false;

				return false;
			}

			//ローロング二回目の判定
			if (m_NextRollingDirection == KEYBOARD_NONE && m_StartRollingDirection == KEYBOARD_A)
			{
				m_NextRollingDirection = KEYBOARD_D;

				m_OldRollingDirection = KEYBOARD_S;

				return false;
			}
			else if (m_NextRollingDirection == KEYBOARD_NONE && m_StartRollingDirection == KEYBOARD_D)
			{
				m_NextRollingDirection = KEYBOARD_A;

				m_OldRollingDirection = KEYBOARD_S;

				return false;
			}

			//三回目のローリングの判定
			if (m_NextRollingDirection == KEYBOARD_S && m_OldRollingDirection == KEYBOARD_A && m_StartRollingDirection != KEYBOARD_D)
			{
				m_NextRollingDirection = KEYBOARD_D;

				m_OldRollingDirection = KEYBOARD_S;

				return false;
			}
			else if (m_NextRollingDirection == KEYBOARD_S && m_OldRollingDirection == KEYBOARD_D && m_StartRollingDirection != KEYBOARD_A)
			{
				m_NextRollingDirection = KEYBOARD_A;

				m_OldRollingDirection = KEYBOARD_S;

				return false;
			}


			//四回目のローリングの判定
			if (m_NextRollingDirection == KEYBOARD_S && m_OldRollingDirection == KEYBOARD_A && m_StartRollingDirection == KEYBOARD_D)
			{
				m_StartRollingDirection = KEYBOARD_NONE;

				m_OldRollingDirection = KEYBOARD_NONE;

				m_NextRollingDirection = KEYBOARD_NONE;

				m_KeyboardRollingFlag = false;

				return true;
			}
			else if (m_NextRollingDirection == KEYBOARD_S && m_OldRollingDirection == KEYBOARD_D && m_StartRollingDirection == KEYBOARD_A)
			{
				m_StartRollingDirection = KEYBOARD_NONE;

				m_OldRollingDirection = KEYBOARD_NONE;

				m_NextRollingDirection = KEYBOARD_NONE;

				m_KeyboardRollingFlag = false;

				return true;
			}
		}
		if (Keyboard_IsKeyTrigger(KK_D))
		{
			//初期の飛び対策
			if (m_StartRollingDirection == KEYBOARD_A && m_OldRollingDirection == KEYBOARD_A)
			{
				m_StartRollingDirection = KEYBOARD_NONE;

				m_OldRollingDirection = KEYBOARD_NONE;

				m_NextRollingDirection = KEYBOARD_NONE;

				m_KeyboardRollingFlag = false;

				return false;
			}

			//ローロング二回目の判定
			if (m_NextRollingDirection == KEYBOARD_NONE && m_StartRollingDirection == KEYBOARD_W)
			{
				m_NextRollingDirection = KEYBOARD_S;

				m_OldRollingDirection = KEYBOARD_D;

				return false;
			}
			else if (m_NextRollingDirection == KEYBOARD_NONE && m_StartRollingDirection == KEYBOARD_S)
			{
				m_NextRollingDirection = KEYBOARD_W;

				m_OldRollingDirection = KEYBOARD_D;

				return false;
			}

			//三回目のローリングの判定
			if (m_NextRollingDirection == KEYBOARD_D && m_OldRollingDirection == KEYBOARD_W && m_StartRollingDirection != KEYBOARD_S)
			{
				m_NextRollingDirection = KEYBOARD_S;

				m_OldRollingDirection = KEYBOARD_D;

				return false;
			}
			else if (m_NextRollingDirection == KEYBOARD_D && m_OldRollingDirection == KEYBOARD_S && m_StartRollingDirection != KEYBOARD_W)
			{
				m_NextRollingDirection = KEYBOARD_W;

				m_OldRollingDirection = KEYBOARD_D;

				return false;
			}

			//四回目のローリングの判定
			if (m_NextRollingDirection == KEYBOARD_D && m_OldRollingDirection == KEYBOARD_W && m_StartRollingDirection == KEYBOARD_S)
			{
				m_StartRollingDirection = KEYBOARD_NONE;

				m_OldRollingDirection = KEYBOARD_NONE;

				m_NextRollingDirection = KEYBOARD_NONE;

				m_KeyboardRollingFlag = false;

				return true;
			}
			else if (m_NextRollingDirection == KEYBOARD_D && m_OldRollingDirection == KEYBOARD_S && m_StartRollingDirection == KEYBOARD_W)
			{
				m_StartRollingDirection = KEYBOARD_NONE;

				m_OldRollingDirection = KEYBOARD_NONE;

				m_NextRollingDirection = KEYBOARD_NONE;

				m_KeyboardRollingFlag = false;

				return true;
			}
		}
	}

	return false;
}
