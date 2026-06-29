#include "configuration.h"
#include "ship.h"
#include "deckblock.h"
#include "texture.h"
#include "sprite.h"
#include "deck.h"
#include <random>
#include <math.h> 
#include "mode.h"
#include "controller.h"
#include "player.h"
#include "camera_player.h"
#include "easing.h"
#include "client.h"

#include "cri.h"

#include "event_command.h"
#include "keyboard.h"

#define DAMAGE (20)

#define LEFTSIDE (SCREEN_WIDTH / 4) 
#define RIGHTSIDE (SCREEN_WIDTH * 3 / 4)

#define CHARASIZE (XMFLOAT2(200.0f,200.0f))
#define SHARKSIZE (XMFLOAT2(500.0f,300.0f))

typedef enum
{
	CHARAANIMATIONSTATE_NONE = -1,

	CHARAANIMATIONSTATE_FISHING1,
	CHARAANIMATIONSTATE_FISHING2,
	CHARAANIMATIONSTATE_LOSE,
	CHARAANIMATIONSTATE_WIN,

	CHARAANIMATIONSTATE_MAX,

}CharaAnimationState;

typedef enum
{
	SHARKANIMATION_NONE = -1,

	SHARKANIMATION_THROW,
	SHARKANIMATION_BITE,
	
	SHARKANIMATION_MAX,
	
}SHARKANIME;

CommandEvent::CommandEvent(std::vector<Ship*> pShip, std::vector<Controller*> pController) :Event(pShip, pController)
{
	SetEndFlag(false);
	
	for (int i = 0; i < GetShip().size(); i++)
	{
		m_Count[i] = 0;
		m_CharaAnimation[i] = CHARAANIMATIONSTATE_FISHING1;

		for (int j = 0; j < GetShip()[i]->GetPlayers().size(); j++)
		{
			m_Button[i][j] = XINPUT_GAMEPAD_DPAD_UP;
			m_Key[i][j] = KK_UP;
			m_ButtonInputFlg[i][j] = true;
			ButtonSlot(i, j);
		}
	}
	m_Button_online = XINPUT_GAMEPAD_DPAD_UP;
	ButtonSlot();
	
	m_FrameCount = 0;
	m_Time = 0;
	m_CharaPosition = XMFLOAT2(0.0f, -100.0f);
	m_CharaVelocity = XMFLOAT2(0.0f, 0.0f);
	m_SharkVelocity = XMFLOAT2(0.0f, 0.0f);

	m_InstructionTex = TextureLoad(L"asset\\texture\\Command!.png");
	
	m_EventBGTexture = TextureLoad(L"asset\\texture\\black.png");
	m_GageTexture = TextureLoad(L"asset\\texture\\eventgage.png");
	m_GageEffect[0] = TextureLoad(L"asset\\texture\\thunder.png");
	m_GageEffect[1] = TextureLoad(L"asset\\texture\\ConpetitionAnimation.png");

	m_Texture = TextureLoad(L"asset\\texture\\number_sansBold 3.png");
	m_ButtonTex[0] = TextureLoad(L"asset\\texture\\Buttons_normal.png");
	m_ButtonTex[1] = TextureLoad(L"asset\\texture\\Buttons_Pushing.png");
	m_SplashTexture = TextureLoad(L"asset\\texture\\wateWaveAnimation.png");
	m_SharkTexture = TextureLoad(L"asset\\texture\\shark.png");	// テクスチャもらったら
	m_CharaTex= TextureLoad(L"asset\\texture\\CharaFishing.png");
	m_RodTex= TextureLoad(L"asset\\texture\\fishing.png");
	m_Speed = 0;
	m_State = EVENTSTATE_COUNTDOWN;
	m_SharkPos = { 0,0 };
	m_SharkRot = 0;
	m_SharkAnimation = SHARKANIMATION_NONE;
	m_Drawflg = false;
	m_SharkReverseflg = false;
	m_CharaReverseflg = false;
	
}


CommandEvent::~CommandEvent()
{
}

void CommandEvent::Update()
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




void CommandEvent::Draw(int windownum)
{
	SetDepthEnable(false);

	// 背景の半透明黒
	SpriteSetColor(1.0f, 1.0f, 1.0f, 0.4f);
	SpriteDraw(m_EventBGTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (m_State == EVENTSTATE_COUNTDOWN)
	{
		SpriteDraw(m_EventBGTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);
		SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
		SpriteDraw(m_InstructionTex, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, m_CountSize * 50.0f + 1000.0f, m_CountSize * 50.0f + 200.0f);

		SpriteDraw(m_Texture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4,
			m_CountSize * 10.0f + 200.0f, m_CountSize * 10.0f + 200.0f,
			(EVENT_COUNTDOWN_TIME - 1 - m_Time) * 200, 0,
			200, TextureGetHeight(m_Texture));
	}
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
	// ゲージの設定
	int gage = (m_Count[0] - m_Count[1]) * EVENT_GAGE_UPPER ;

	// ゲージの描画
	SpriteDraw(m_GageTexture, LEFTSIDE + (gage / 2.0f), GAGEPOSITION_Y, (SCREEN_WIDTH / 2.0f) + gage, GAGESIZE_Y, 0, 0, 50, 100);
	SpriteDraw(m_GageTexture, RIGHTSIDE + (gage / 2.0f), GAGEPOSITION_Y, (SCREEN_WIDTH / 2.0f) - gage, GAGESIZE_Y, 50, 0, 50, 100);

	// ゲージのエフェクト
	SpriteSetColor(1.0f, 0.2f, 0.2f, 1.0f);
	SpriteDraw(m_GageEffect[0], LEFTSIDE + (gage / 2.0f), GAGEPOSITION_Y + GAGESIZE_Y / 2, (SCREEN_WIDTH / 2.0f) + gage, GAGESIZE_Y / 4, 0, (m_FrameCount % 2) * 40, 300, 40, 0, true);
	SpriteDraw(m_GageEffect[0], LEFTSIDE + (gage / 2.0f), GAGEPOSITION_Y - GAGESIZE_Y / 2, (SCREEN_WIDTH / 2.0f) + gage, GAGESIZE_Y / 4, 0, (m_FrameCount % 2) * 40, 300, 40, 0, true);

	SpriteSetColor(0.2f, 0.2f, 1.0f, 1.0f);
	SpriteDraw(m_GageEffect[0], RIGHTSIDE + (gage / 2.0f), GAGEPOSITION_Y + GAGESIZE_Y / 2, (SCREEN_WIDTH / 2.0f) - gage, GAGESIZE_Y / 4, 0, (m_FrameCount % 2) * 40, 300, 40, 0, false);
	SpriteDraw(m_GageEffect[0], RIGHTSIDE + (gage / 2.0f), GAGEPOSITION_Y - GAGESIZE_Y / 2, (SCREEN_WIDTH / 2.0f) - gage, GAGESIZE_Y / 4, 0, (m_FrameCount % 2) * 40, 300, 40, 0, false);

	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	SpriteDraw(m_GageEffect[1], SCREEN_WIDTH / 2.0f +gage, GAGEPOSITION_Y, 45, 125,
		(m_FrameCount % 2) * 40, 0, 40, 40);


	int fishingchara = -1;		// キャラのIDに合わせて絵を変える
	float charapositiontcap = 0.0f;	// 複数人プレイ時の横座標調整
	int rev = 0;			// 反転用位置調整

	// 船のサイズ分
	for (int shipid = 0; shipid < GetShip().size(); shipid++)
	{
		if (shipid == 0)	// 船のIDが０
		{
			m_CharaPosition.x = LEFTSIDE;
			// 反転あり
			rev = 1;	
			m_CharaReverseflg = true;
		}
		else				// 船のIDが１
		{
			m_CharaPosition.x = RIGHTSIDE;
			// 反転なし
			rev = -1;
			m_CharaReverseflg = false;
		}

		// プレイヤーのサイズ分
		for (int playerid = 0; playerid < GetShip()[shipid]->GetPlayers().size(); playerid++)
		{
			// 4人プレイ時の位置調整
			if (playerid == 0)
			{
				charapositiontcap = (CHARASIZE.x / 4) * rev;
			}
			else
			{
				charapositiontcap = 0;
			}
			// IDとキャラの連動
			fishingchara = GetShip()[shipid]->GetPlayers()[playerid]->GetId();

			// キャラの描画
			SpriteDraw(m_CharaTex, m_CharaPosition.x + charapositiontcap + m_CharaVelocity.x, m_CharaPosition.y,
				CHARASIZE.x, CHARASIZE.y,
				m_CharaAnimation[shipid] * (TextureGetWidth(m_CharaTex) / 4), fishingchara * 550, TextureGetWidth(m_CharaTex) / 4, TextureGetHeight(m_CharaTex) / 4,
				0, m_CharaReverseflg);
			
			// 入力時のみ表示
			if (m_State == EVENTSTATE_EVENT)
			{
				XMFLOAT2 rodpos = { 0.0f,0.0f };	// 釣竿の位置

				// アニメーションによる位置調整
				float rotmovenum = 0;		
				int rodanim = 0;
				if (m_CharaAnimation[shipid] == 0)
				{
					rotmovenum = 40.0f;
					rodpos.x = m_CharaPosition.x + charapositiontcap*2 + m_CharaVelocity.x + (rotmovenum * rev * 1.5f);
					rodpos.y = m_CharaPosition.y - 20.0f;
					rodanim = 0;
				}
				else
				{
					rotmovenum = 35.0f;
					// オウムの時だけ位置を変更
					if (GetShip()[shipid]->GetPlayers()[playerid]->GetId()==2)
					{
						rotmovenum = 55.0f;
					}
					rodpos.x = m_CharaPosition.x + charapositiontcap*2 + m_CharaVelocity.x + (rotmovenum * rev * 1.5f);
					rodpos.y = m_CharaPosition.y;
					rodanim = 1;
				}
				
				SpriteDraw(m_RodTex, rodpos.x, rodpos.y,
					200, 100,
					0, rodanim*50, TextureGetWidth(m_RodTex), TextureGetHeight(m_RodTex)/2,
					0, !m_CharaReverseflg);	

			}
		}
	}

	// 水しぶき
	SpriteDraw(m_SplashTexture, SCREEN_WIDTH / 2, m_CharaPosition.y, CHARASIZE.x*2, CHARASIZE.y, 40 * m_Speed, 0, 40, TextureGetHeight(m_SplashTexture), 0, true);
	
	// 入力時のみの表示
	if (m_State == EVENTSTATE_EVENT)
	{
		SpriteDraw(m_Texture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4,
			200, 200,
			(5 - m_Time) * 200, 0,
			200, TextureGetHeight(m_Texture));
	}

	// サメのテクスチャサイズ
	int sharktexwidth = TextureGetWidth(m_SharkTexture);
	int sharktexheight = TextureGetHeight(m_SharkTexture);
	
	// サメの描画
	SpriteSetColor(2.5f, 2.5f, 2.5f, 1.0f);
	if (m_State == EVENTSTATE_ANIM&&!m_Drawflg)
	{
		SpriteDraw(m_SharkTexture, m_SharkPos.x, m_SharkPos.y, SHARKSIZE.x, SHARKSIZE.y, 0, 0, sharktexwidth, sharktexheight, m_SharkRot, m_SharkReverseflg);
	}
	else if (m_Drawflg)
	{
		SpriteDraw(m_SharkTexture, LEFTSIDE , m_SharkPos.y, SHARKSIZE.x, SHARKSIZE.y, 0, 0, sharktexwidth,sharktexheight, m_SharkRot, false);
		SpriteDraw(m_SharkTexture, RIGHTSIDE , m_SharkPos.y, SHARKSIZE.x, SHARKSIZE.y, 0, 0,sharktexwidth,sharktexheight, m_SharkRot, true);
	}
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);

	ButtonDraw(windownum);
	
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetDepthEnable(true);
}

void CommandEvent::Damage(int shipid, int damage)
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
		GetShip()[shipid]->GetPlayers()[j]->SetBlowOff();
		GetShip()[shipid]->GetPlayers()[j]->GetCamera()->SetCameraShake(10.0f);
		GetShip()[shipid]->GetPlayers()[j]->GetController()->Controller_SetVibration(16000, 16000, 2000);
	}
}

void CommandEvent::ButtonSlot()
{
	// 完全乱数
	std::random_device rd;
	int buttunnum = rd() % 4;
	
	switch (buttunnum)
	{
	case 0:
		m_Button_online = XINPUT_GAMEPAD_B;
		m_TexCut_online = 0;

		// 間違いのボタン
		m_WrongButton_online[0] = XINPUT_GAMEPAD_A;
		m_WrongButton_online[1] = XINPUT_GAMEPAD_X;
		m_WrongButton_online[2] = XINPUT_GAMEPAD_Y;
		break;
	case 1:
		m_Button_online = XINPUT_GAMEPAD_A;
		m_TexCut_online = 1;

		// 間違いのボタン
		m_WrongButton_online[0] = XINPUT_GAMEPAD_B;
		m_WrongButton_online[1] = XINPUT_GAMEPAD_X;
		m_WrongButton_online[2] = XINPUT_GAMEPAD_Y;
		break;
	case 2:
		m_Button_online = XINPUT_GAMEPAD_X;
		m_TexCut_online = 2;

		// 間違いのボタン
		m_WrongButton_online[0] = XINPUT_GAMEPAD_A;
		m_WrongButton_online[1] = XINPUT_GAMEPAD_B;
		m_WrongButton_online[2] = XINPUT_GAMEPAD_Y;
		break;
	case 3:
		m_Button_online = XINPUT_GAMEPAD_Y;
		m_TexCut_online = 3;
		
		// 間違いのボタン
		m_WrongButton_online[0] = XINPUT_GAMEPAD_A;
		m_WrongButton_online[1] = XINPUT_GAMEPAD_X;
		m_WrongButton_online[2] = XINPUT_GAMEPAD_B;
		break;

	default:
		break;
	}
}

void CommandEvent::ButtonSlot(int shipID, int charaID)
{
	m_ButtonPush[shipID][charaID] = 0;

	// 完全乱数
	std::random_device rd; 
	int buttunnum = rd() % 4;
	

	switch (buttunnum)
	{
	case 0:	// B
		m_Button[shipID][charaID] = XINPUT_GAMEPAD_B;
		m_TexCut[shipID][charaID] = 0;
		// 間違っているボタン
		m_WrongButton[shipID][charaID][0] = XINPUT_GAMEPAD_A;
		m_WrongButton[shipID][charaID][1] = XINPUT_GAMEPAD_X;
		m_WrongButton[shipID][charaID][2] = XINPUT_GAMEPAD_Y;

		m_Key[shipID][charaID] = 1;
		break;
	case 1:	// A
		m_Button[shipID][charaID] = XINPUT_GAMEPAD_A;
		m_TexCut[shipID][charaID] = 1;

		// 間違っているボタン
		m_WrongButton[shipID][charaID][0] = XINPUT_GAMEPAD_B;
		m_WrongButton[shipID][charaID][1] = XINPUT_GAMEPAD_X;
		m_WrongButton[shipID][charaID][2] = XINPUT_GAMEPAD_Y;

		m_Key[shipID][charaID] = 1;
		break;
	case 2:	// X
		m_Button[shipID][charaID] = XINPUT_GAMEPAD_X;
		m_TexCut[shipID][charaID] = 2;

		// 間違っているボタン
		m_WrongButton[shipID][charaID][0] = XINPUT_GAMEPAD_A;
		m_WrongButton[shipID][charaID][1] = XINPUT_GAMEPAD_B;
		m_WrongButton[shipID][charaID][2] = XINPUT_GAMEPAD_Y; 

		m_Key[shipID][charaID] = 2;
		break;
	case 3:	// Y
		m_Button[shipID][charaID] = XINPUT_GAMEPAD_Y;
		m_TexCut[shipID][charaID] = 3;

		// 間違っているボタン
		m_WrongButton[shipID][charaID][0] = XINPUT_GAMEPAD_A;
		m_WrongButton[shipID][charaID][1] = XINPUT_GAMEPAD_X;
		m_WrongButton[shipID][charaID][2] = XINPUT_GAMEPAD_B;

		m_Key[shipID][charaID] = 3;
		break;
	case 4:	// LB
		m_Button[shipID][charaID] = XINPUT_GAMEPAD_LEFT_THUMB;
		m_TexCut[shipID][charaID] = 4;
		break;
	case 5:	// LT
		m_Button[shipID][charaID] = XINPUT_GAMEPAD_LEFT_SHOULDER;
		m_TexCut[shipID][charaID] = 5;
		break;
	case 6:	// RB
		m_Button[shipID][charaID] = XINPUT_GAMEPAD_RIGHT_THUMB;
		m_TexCut[shipID][charaID] = 6;
		break;
	case 7:	// RT
		m_Button[shipID][charaID] = XINPUT_GAMEPAD_RIGHT_SHOULDER;
		m_TexCut[shipID][charaID] = 7;
		break;
	default:
		break;
	}

}

void CommandEvent::ButtonDraw(int WindowNum)
{
	// デバッグ用　テクスチャもらってからいじる
	int tex=0;
	int shipID = 0;
	int playerID = 0;
	float buttonsize[2][2] = {};
	XMFLOAT2 texpos[2][2] = {};

	float wrongbuttunmove[2][2] = {};
	float wrongbuttonmove = 0;

	for (int i = 0; i < GetShip().size(); i++)
	{
		for (int j = 0; j < GetShip()[i]->GetPlayers().size(); j++)
		{
			wrongbuttunmove[i][j] = sinf(m_WrongButtonMove[i][j])*10.0f;
			texpos[i][j] = XMFLOAT2(SCREEN_WIDTH / 2 + wrongbuttunmove[i][j], SCREEN_HEIGHT / 2);
			if (m_ButtonPush[i][j]==1)
			{
				buttonsize[i][j] = 200;
				texpos[i][j].y = SCREEN_HEIGHT / 2 + 8.9f;
			}
			else
			{
				buttonsize[i][j] = 100;
			}
		}
	}
	
	switch (GetGameMode())
	{
	case GAMEMODE_SOLO://ソロが一画面

		break;
	case GAMEMODE_LOCAL_SPLITSCREEN://一画面分割あり
		if (m_State == EVENTSTATE_EVENT)
		{
			for (int i = 0; i < 2; i++)
			{
				tex = (TextureGetWidth(m_ButtonTex[m_ButtonPush[i][0]]) / 8) * m_TexCut[i][0];
				if (i == 0)
				{
					texpos[i][0].x = SCREEN_WIDTH * 0.5f - 320.0f + wrongbuttunmove[i][0];
				}
				else
				{
					texpos[i][0].x = SCREEN_WIDTH * 0.5f + 320.0f + wrongbuttunmove[i][0];
				}
				SpriteDraw(m_ButtonTex[m_ButtonPush[i][0]], texpos[i][0].x, texpos[i][0].y, buttonsize[i][0], buttonsize[i][0],
					tex, 0, TextureGetWidth(m_ButtonTex[m_ButtonPush[i][0]]) / 8, TextureGetHeight(m_ButtonTex[m_ButtonPush[i][0]]));
			}

		}
		break;
	case GAMEMODE_LOCAL_MULTIWINDOW://二画面分割なし
		tex = (TextureGetWidth(m_ButtonTex[m_ButtonPush[WindowNum][0]]) / 8) * (m_TexCut[WindowNum][0] % 8);


		if (m_State == EVENTSTATE_EVENT)
		{
			SpriteDraw(m_ButtonTex[m_ButtonPush[WindowNum][0]], texpos[WindowNum][0].x, texpos[WindowNum][0].y, buttonsize[WindowNum][0], buttonsize[WindowNum][0],
				tex, 0, TextureGetWidth(m_ButtonTex[m_ButtonPush[WindowNum][0]]) / 8, TextureGetHeight(m_ButtonTex[m_ButtonPush[WindowNum][0]]));
		}
		break;
	case GAMEMODE_SEGMENTATION://に画面分割あり

		if (m_State == EVENTSTATE_EVENT)
		{
			for (int i = 0; i < GetShip()[WindowNum]->GetPlayers().size(); i++)
			{
				tex = (TextureGetWidth(m_ButtonTex[m_ButtonPush[WindowNum][i]]) / 8) * (m_TexCut[WindowNum][i] % 8);
				if (i==0)
				{
					texpos[WindowNum][i].x = SCREEN_WIDTH * 0.5f - 320.0f + wrongbuttunmove[WindowNum][i];
				}
				else
				{
					texpos[WindowNum][i].x = SCREEN_WIDTH * 0.5f + 320.0f + wrongbuttunmove[WindowNum][i];
				}
				SpriteDraw(m_ButtonTex[m_ButtonPush[WindowNum][i]], texpos[WindowNum][i].x, texpos[WindowNum][i].y, buttonsize[WindowNum][i], buttonsize[WindowNum][i], tex, 0,
					TextureGetWidth(m_ButtonTex[m_ButtonPush[WindowNum][i]]) / 8, TextureGetHeight(m_ButtonTex[m_ButtonPush[WindowNum][i]]));
			}

		}
		break;	
	case GAMEMODE_MULTIWINDOW://四画面分割なし
		switch (WindowNum)
		{
		case 0:
			shipID = 0;
			playerID = 0;
			break;
		case 1:

			shipID = 0;
			playerID = 1;
			break;
		case 2:
			shipID = 1;
			playerID = 0;
			break;
		case 3:
			shipID = 1;
			playerID = 1;
			break;
		default:
			break;
		}
		tex = (TextureGetWidth(m_ButtonTex[m_ButtonPush[shipID][playerID]]) / 8) * (m_TexCut[shipID][playerID] % 8);

		if (m_State == EVENTSTATE_EVENT)
		{
			SpriteDraw(m_ButtonTex[m_ButtonPush[shipID][playerID]], texpos[shipID][playerID].x, texpos[shipID][playerID].y, buttonsize[shipID][playerID], buttonsize[shipID][playerID],
				tex, 0, TextureGetWidth(m_ButtonTex[m_ButtonPush[shipID][playerID]]) / 8, TextureGetHeight(m_ButtonTex[m_ButtonPush[shipID][playerID]]));
		}
	
		break;
	case GAMEMODE_ONLINE://一画面分割なし
		tex = (TextureGetWidth(m_ButtonTex[0]) / 8) * (m_TexCut_online % 8);
		wrongbuttonmove = sinf(m_WrongButtonMove_online) * 10.0f;

		if (m_State==EVENTSTATE_EVENT)
		{
			SpriteDraw(m_ButtonTex[0], texpos[0][0].x+wrongbuttonmove, texpos[0][0].y, buttonsize[0][0], buttonsize[0][0],
				tex, 0, TextureGetWidth(m_ButtonTex[0]) / 8, TextureGetHeight(m_ButtonTex[0]));
		}
		break;
	default:
		break;
	}
	
	
	
}

void CommandEvent::KeyBordInput(int i, int j)
{
	// コントローラーが接続していなかったら
	if (!GetShip()[i]->GetPlayers()[j]->GetController()->IsConnected())
	{
		switch (m_Key[i][j])
		{
		case 0:
			if (Keyboard_IsKeyTrigger(KK_D))
			{
				if (i == 0)
				{
					m_Count[0]++;
					m_CharaVelocity.x = -15;
				}
				else
				{
					m_Count[1]++;
					m_CharaVelocity.x = +15;
				}
				SEStart(GetAppSE(), CRI_SE_コマンド成功音);
				m_ButtonPush[i][j] = 1;
			}
			break;
		case 1:
			if (Keyboard_IsKeyTrigger(KK_D))
			{
				if (i == 0)
				{
					m_Count[0]++;
					m_CharaVelocity.x = -15;
				}
				else
				{
					m_Count[1]++;
					m_CharaVelocity.x = +15;
				}
				SEStart(GetAppSE(), CRI_SE_コマンド成功音);
				m_ButtonPush[i][j] = 1;
			}
			break;
		case 2:
			if (Keyboard_IsKeyTrigger(KK_D))
			{
				if (i == 0)
				{
					m_Count[0]++;
					m_CharaVelocity.x = -15;
				}
				else
				{
					m_Count[1]++;
					m_CharaVelocity.x = +15;
				}
				SEStart(GetAppSE(), CRI_SE_コマンド成功音);
				m_ButtonPush[i][j] = 1;
			}
			break;
		case 3:
			if (Keyboard_IsKeyTrigger(KK_D))
			{
				if (i == 0)
				{
					m_Count[0]++;
					m_CharaVelocity.x = -15;
				}
				else
				{
					m_Count[1]++;
					m_CharaVelocity.x = +15;
				}
				SEStart(GetAppSE(), CRI_SE_コマンド成功音);
				m_ButtonPush[i][j] = 1;
			}
			break;

		default:
			break;
		}

	}

}


void CommandEvent::CountUpdate()
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

void CommandEvent::EventUpdate()
{
	if (GetGameMode()==GAMEMODE_ONLINE)
	{
		// タイマー　= 1秒カウント
		m_Time = m_FrameCount / 60;
		if (m_Time % 2 == 0)
		{
			m_CharaAnimation[0] = CHARAANIMATIONSTATE_FISHING1;
			m_CharaAnimation[1] = CHARAANIMATIONSTATE_FISHING1;
		}
		else
		{
			m_CharaAnimation[0] = CHARAANIMATIONSTATE_FISHING2;
			m_CharaAnimation[1] = CHARAANIMATIONSTATE_FISHING2;
		}
		if (m_Time == 5)// 5秒で終了
		{
			// 入力処理を終了
			m_State = EVENTSTATE_ANIM_WAIT;

			m_Time = 0;
			m_FrameCount = 0;
			m_SharkPos.x = SCREEN_WIDTH / 2;
			m_SharkPos.y = m_CharaPosition.y;

		}
		// 正しいボタンなら
		if (GetController().front()->Controller_IsJustPressed(GetController().front()->GetButtonForTrigger(m_Button_online))&&
			m_PushWait_online==0&&
			GetController().front()->IsConnected())
		{
			// ボタンのスロット
			ButtonSlot();
			
			// カウントをサーバーに送る
			EventPlayerData data;
			data.teamid = GetClient()->GetMyTeamId();
			data.eventcount = 1;
			data.readyflag = EVENTPLAYERREADY_PLAY;

			GetClient()->SendEventData(data);

			// チームIDによって左右どちらかに移動する
			if (GetClient()->GetMyTeamId()==0)
			{
				m_CharaVelocity.x = -15;
			}
			else
			{
				m_CharaVelocity.x = +15;
			}

			SEStart(GetAppSE(), CRI_SE_コマンド成功音);
		}
		// 間違えた入力なら
		else if((GetController().front()->Controller_IsJustPressed(GetController().front()->GetButtonForTrigger(m_WrongButton_online[0]))||
			GetController().front()->Controller_IsJustPressed(GetController().front()->GetButtonForTrigger(m_WrongButton_online[1]))||
			GetController().front()->Controller_IsJustPressed(GetController().front()->GetButtonForTrigger(m_WrongButton_online[2])))
			&& GetController().front()->IsConnected())
		{
			// 待機時間 = 30フレーム
			m_PushWait_online = 30;
			// 間違えた際のボタンの動き
			m_WrongButtonMove_online = 100.0f;

			SEStart(GetAppSE(), CRI_SE_コマンド失敗音);
		}
		// 間違えた際の待機時間処理
		if (m_PushWait_online>0)
		{// 待機時間を減らす
			m_PushWait_online--;
		}
		else
		{// 0未満にしないための処理
			m_PushWait_online = 0;
		}
		m_WrongButtonMove_online *= 0.9f;

		// フレームカウント
		m_FrameCount++;

		m_CharaPosition.y = SCREEN_HEIGHT * 3 / 4.0f + sinf(m_FrameCount / 5.0f) * 3.0f;
		m_Speed = m_FrameCount / 15;

		// キャラの位置を徐々に戻す
		m_CharaVelocity.x *= 0.9f;

	}
	//----------------------
	// オンライン以外なら
	//----------------------
	else 
	{
		// タイマー　= 1秒カウント
		m_Time = m_FrameCount / 60;
		if (m_Time % 2 == 0)
		{
			m_CharaAnimation[0] = CHARAANIMATIONSTATE_FISHING1;
			m_CharaAnimation[1] = CHARAANIMATIONSTATE_FISHING1;
		}
		else
		{
			m_CharaAnimation[0] = CHARAANIMATIONSTATE_FISHING2;
			m_CharaAnimation[1] = CHARAANIMATIONSTATE_FISHING2;
		}
		if (m_Time == 5)// 5秒で終了
		{
			// 入力処理を終了
			m_State = EVENTSTATE_ANIM;

			m_Time = 0;
			m_FrameCount = 0;
			m_SharkPos.x = SCREEN_WIDTH / 2;
			m_SharkPos.y = m_CharaPosition.y;
		}

		// コントローラーの数分繰り返す
		for (int i = 0; i < GetShip().size(); i++) {
			for (int j = 0; j < GetShip()[i]->GetPlayers().size(); j++) {// ※各プレイヤーにコントローラーが入ってないとエラーが発生します
				if (GetShip()[i]->GetPlayers()[j]->GetController()->Controller_IsJustPressed(GetShip()[i]->GetPlayers()[j]->GetController()->GetButtonForTrigger(m_Button[i][j]))
					&& m_ButtonInputFlg[i][j])
				{
					if (i == 0)
					{
						m_Count[0]++;
						m_CharaVelocity.x = -15;
					}
					else
					{
						m_Count[1]++;
						m_CharaVelocity.x = +15;
					}

					SEStart(GetAppSE(), CRI_SE_コマンド成功音);
					m_ButtonPush[i][j] = 1;
				}
				else if (GetShip()[i]->GetPlayers()[j]->GetController()->Controller_IsJustPressed(GetShip()[i]->GetPlayers()[j]->GetController()->GetButtonForTrigger(m_WrongButton[i][j][0])) ||
					GetShip()[i]->GetPlayers()[j]->GetController()->Controller_IsJustPressed(GetShip()[i]->GetPlayers()[j]->GetController()->GetButtonForTrigger(m_WrongButton[i][j][1])) ||
					GetShip()[i]->GetPlayers()[j]->GetController()->Controller_IsJustPressed(GetShip()[i]->GetPlayers()[j]->GetController()->GetButtonForTrigger(m_WrongButton[i][j][2])))
				{
					m_ButtonInputFlg[i][j] = false;
					m_WrongButtonMove[i][j] = 100.0f;

					SEStart(GetAppSE(), CRI_SE_コマンド失敗音);
				}

				KeyBordInput(i, j);

				m_WrongButtonMove[i][j] *= 0.9f;
				if (!m_ButtonInputFlg[i][j])
				{
					if (m_PushWait[i][j] >= 30)
					{
						m_ButtonInputFlg[i][j] = true;
					}
					m_PushWait[i][j]++;
				}
				else
				{
					m_PushWait[i][j] = 0;
				}

				if (m_ButtonFrame[i][j] >= 3)
				{
					ButtonSlot(i, j);
				}

				if (m_ButtonPush[i][j] == 1)
				{
					m_ButtonFrame[i][j]++;
				}
				else
				{
					m_ButtonFrame[i][j] = 0.0f;
				}
			
				//// デバッグ用キーボード操作
				//if (Keyboard_IsKeyTrigger(KK_A) && !GetShip()[i]->GetPlayers()[j]->GetController()->IsConnected())
				//{
				//	m_Count[0]++;
				//	ButtonSlot(0, j);
				//	m_CharaVelocity.x = -15;
				//}
				//if (Keyboard_IsKeyTrigger(KK_D) && !GetShip()[i]->GetPlayers()[j]->GetController()->IsConnected())
				//{
				//	m_Count[1]++;
				//	ButtonSlot(1, j);
				//	m_CharaVelocity.x = +15;
				//}


			}
		}

		// フレームカウント
		m_FrameCount++;

		m_CharaPosition.y = SCREEN_HEIGHT * 3 / 4.0f + sinf(m_FrameCount / 5.0f) * 3.0f;
		m_Speed = m_FrameCount / 15;
		m_CharaVelocity.x *= 0.9f;
	}
}

void CommandEvent::AnimUpdate()
{
	// タイマー　= 1秒カウント
	m_Time = m_FrameCount / 60;

	if (m_Time==3)
	{
		if (GetGameMode() != GAMEMODE_ONLINE) {
			m_State = EVENTSTATE_DAMAGE;
		}
		else {
			m_State = EVENTSTATE_DAMAGE_WAIT;
		}

	}

	
	switch (m_SharkAnimation)
	{
	case SHARKANIMATION_NONE:
		m_SharkPos.y -= 45.0f;
		if (m_SharkPos.y + SHARKSIZE.y < 0)
		{
			if (m_Count[0] < m_Count[1])
			{
				m_SharkReverseflg = false;
				m_SharkPos.x = RIGHTSIDE;
				m_CharaAnimation[1] = CHARAANIMATIONSTATE_WIN;
			}
			else if (m_Count[0] > m_Count[1])
			{
				m_SharkReverseflg = true;
				m_SharkPos.x = LEFTSIDE;
				m_CharaAnimation[0] = CHARAANIMATIONSTATE_WIN;
			}
			else
			{
				m_Drawflg = true;
				m_CharaAnimation[0] = CHARAANIMATIONSTATE_LOSE;
				m_CharaAnimation[1] = CHARAANIMATIONSTATE_LOSE;
				SEStart(GetAppSE(), CRI_SE_魚が水面で暴れる);
			}
			m_SharkAnimation = SHARKANIMATION_THROW;
		}
		break;
	case SHARKANIMATION_THROW:
		if (m_SharkPos.y < m_CharaPosition.y - 200.0f)
		{
			m_SharkPos.y += 15.0f;
		}
		else
		{
			m_SharkRot += 15;

			if (m_Count[0] == m_Count[1])
			{
				m_SharkVelocity.y = -10.0f * sinf((float)m_FrameCount);
				if ((int)m_SharkRot % 30 == 0)
				{
					m_SharkRot *= -1;
				}
			}
			if (m_SharkRot > 150)
			{
				if (m_Count[0] < m_Count[1])
				{
					m_SharkVelocity.x -= 5.0f;
					m_SharkVelocity.y += 1.0f;
					if (m_SharkPos.x <= LEFTSIDE)
					{
						m_SharkPos.x = LEFTSIDE;
						m_SharkAnimation = SHARKANIMATION_BITE;
						m_CharaAnimation[0] = CHARAANIMATIONSTATE_LOSE;
						SEStart(GetAppSE(), CRI_SE_魚が水面で暴れる);
					}
				}
				else if (m_Count[0] > m_Count[1])
				{
					m_SharkVelocity.x += 5.0f;
					m_SharkVelocity.y += 1.0f;
					if (m_SharkPos.x >= RIGHTSIDE)
					{
						m_SharkPos.x = RIGHTSIDE;
						m_SharkAnimation = SHARKANIMATION_BITE;
						m_CharaAnimation[1] = CHARAANIMATIONSTATE_LOSE;
						SEStart(GetAppSE(), CRI_SE_魚が水面で暴れる);
					}
				}
			}

		}
		break;
	case SHARKANIMATION_BITE:
		m_SharkRot += 15;

		m_SharkVelocity.x = 0;
		m_SharkVelocity.y = -30.0f * sinf((float)m_FrameCount);
		if ((int)m_SharkRot % 30 == 0)
		{
			m_SharkRot *= -1;
		}
		break;
	default:
		break;
	}
	m_SharkPos.x += m_SharkVelocity.x;
	m_SharkPos.y += m_SharkVelocity.y;

	// フレームカウント
	m_FrameCount++;

}

void CommandEvent::DamageUpdate()
{
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
	else {
		int winteam;
		if (m_Count[0] > m_Count[1]) {
			winteam = 0;
		}
		else if (m_Count[0] < m_Count[1]) {
			winteam = 1;
		}
		else // 引き分け時のダメージ処理
		{
			winteam = -1;
		}

		if (winteam != GetClient()->GetMyTeamId() || winteam == -1) {
			GetShip()[GetClient()->GetMyTeamId()]->GetPlayers().front()->SetBlowOff(240);
			GetShip()[GetClient()->GetMyTeamId()]->GetPlayers().front()->GetCamera()->SetCameraShake(10.0f);
			GetShip()[GetClient()->GetMyTeamId()]->GetPlayers().front()->GetController()->Controller_SetVibration(16000, 16000, 2000);
		}
		m_State = EVENTSTATE_END_WAIT;
		
	}
}

int CommandEvent::IDSearch()
{
	return 0;
}
