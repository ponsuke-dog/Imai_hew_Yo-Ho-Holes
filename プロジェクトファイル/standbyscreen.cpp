#include "configuration.h"
#include "directx.h"
#include "keyboard.h"
#include "texture.h"
#include "sprite.h"
#include "scene.h"
#include "cri.h"
#include "mode.h"
#include "fade.h"
#include "controller.h"
#include <vector>
#include "effect_2d.h"
#include "client.h"
#include "standbyscreen.h"

#define StandByPlayreNum	(4)

// 入力→フェード→ゲームシーンまでの切り替え用
typedef enum 
{
	STANDBYSCREENSTATE_NONE = -1,

	STANDBYSCREENSTATE_WAIT,
	STANDBYSCREENSTATE_FADE,
	STANDBYSCREENSTATE_SCENECHANGE,

	STANDBYSCREENSTATE_MAX,

}StandbyScreenState;

// 各プレイヤーの準備フラグ
static bool g_StandByFlags[StandByPlayreNum] = {};

// プレイヤーの人数
static int g_StandByPlayerNum = 0;

// テクスチャ
static int g_Texture[2] = {};
static int g_CharaTexture = 0;

// 長押し用のカウント変数
static int g_PushCount[StandByPlayreNum] = {};

// なにこれ？
static bool g_IsFading;

// コントローラー
static Controller controller(0);

static std::vector<Controller*>m_pControllers;

// ゲームモードの取得用
GAMEMODE g_GameMode = GAMEMODE_NONE;

static bool g_ReadyFlag = false;

static Effect2D* g_Effect2d = nullptr;

// ステート
static StandbyScreenState g_State = STANDBYSCREENSTATE_NONE;

// 全てのプレイヤーが準備完了かどうかの関数
bool CheckFlgAllReady();

void StandByScreenInitialize(){

	// 初期化
	for (int i = 0; i < StandByPlayreNum; i++) {
		g_StandByFlags[i] = false;
		g_PushCount[i] = 0;
	}

	// ゲームモードの取得
	g_GameMode = GetGameMode();
	
	// プレイヤーの数の初期化
	g_StandByPlayerNum = 0;

	// コントローラーの登録
	m_pControllers.push_back(new Controller(0));
	m_pControllers.push_back(new Controller(1));
	m_pControllers.push_back(new Controller(2));
	m_pControllers.push_back(new Controller(3));

	// なにに使ってるか不明,使わなければ削除
	g_IsFading = false;

	// ゲームモードによってプレイヤーの人数のお調整
	switch (g_GameMode)
	{
	case GAMEMODE_SOLO://1
		g_StandByPlayerNum = 1;
		break;

	case GAMEMODE_LOCAL_SPLITSCREEN://2
		g_StandByPlayerNum = 2;
		break;

	case GAMEMODE_LOCAL_MULTIWINDOW://2
		g_StandByPlayerNum = 2;
		break;

	case GAMEMODE_SEGMENTATION://4	
		g_StandByPlayerNum = 4;
		break;

	case GAMEMODE_MULTIWINDOW://4
		g_StandByPlayerNum = 4;
		break;

	case GAMEMODE_ONLINE://1		
		g_StandByPlayerNum = 1;
		g_ReadyFlag = false;
		break;

	default:
		break;
	}

	// ステートの初期化：コントローラーの入力受け付け
	g_State = STANDBYSCREENSTATE_WAIT;

	// テクスチャのロード
	g_Texture[0] = TextureLoad(L"asset\\texture\\02.png");
	g_Texture[1] = TextureLoad(L"asset\\texture\\waitgauge.png");
	g_CharaTexture = TextureLoad(L"asset\\texture\\Animals_Circle.png");

	g_Effect2d = new Effect2D(TextureLoad(L"asset\\texture\\logo_rotate.png"), { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f }, { 512,512 }, 8, 1, 5, EFFECTTYPE_LOOP);
}

void StandByScreenFinalize()
{
	delete g_Effect2d;
	g_Effect2d = nullptr;

	m_pControllers.clear();
}

void StandByScreenUpdate(){

	//if (GetFadeState() == FADE_STATE_OUT || GetFadeState() == FADE_STATE_IN) { return; }
	//ゲームモード判定
	m_pControllers[0]->Controller_Update();
	m_pControllers[1]->Controller_Update();
	m_pControllers[2]->Controller_Update();
	m_pControllers[3]->Controller_Update();

	
	//オンライン以外
	if (g_GameMode != GAMEMODE_ONLINE) {

		// ステートごとに動きを変える
		switch (g_State)
		{
		// コントローラーの入力受け付け
		case STANDBYSCREENSTATE_WAIT:
			for (int i = 0; i < g_StandByPlayerNum; i++) {
				// キーボード長押し
				if ((Keyboard_IsKeyDown(KK_ENTER) && !m_pControllers[i]->IsConnected()) && g_PushCount[i] < 60) {
					g_PushCount[i]++;
				}

				// コントローラー長押し
				if (m_pControllers[i]->IsConnected())
				{
					// 長押し中
					if (m_pControllers[i]->Controller_IsButtonDown(XINPUT_GAMEPAD_B)
						&& g_PushCount[i] < 60)
					{
						// カウントを増やす
						g_PushCount[i]++;
					}

					// 準備完了にならずに離したら 
					if ((!m_pControllers[i]->Controller_IsButtonDown(XINPUT_GAMEPAD_B) &&
						g_PushCount[i] < 60) &&
						g_PushCount[i] >= 0)
					{
						g_PushCount[i]--;
					}

				}

				// 準備完了
				if (g_PushCount[i] >= 60)
				{
					g_PushCount[i] = 60;

					if (g_StandByFlags[i] == false)
					{
						// ここに効果音？
						SEStart(GetAppSE(), CRI_SE_STANDBY);
					}
					g_StandByFlags[i] = true;
					
				}
			}
			if (CheckFlgAllReady())// 全てのプレイヤーの準備が完了したら
			{
				// フェードを開始するステートを始める
				g_State = STANDBYSCREENSTATE_FADE;
			}
			break;
		case STANDBYSCREENSTATE_FADE:	// フェードを開始するステート
			// フェードを始める
			FadeOut();
			// ゲームシーンに移るステートに変える
			g_State = STANDBYSCREENSTATE_SCENECHANGE;
			break;
		case STANDBYSCREENSTATE_SCENECHANGE:// ゲームシーンに移るステート
			// シーンを切り替える
			SceneChange(SCENE_GAME);
			break;
		default:
			break;
		}
	}
	//オンライン
	else {
		if (!g_ReadyFlag) {

			if (!GetClient()) {
				return;
			}

			if (GetClient()->GetGameScene().serverstate == SERVERSTATE_MATCHING) {
				// キーボード長押し
				if ((Keyboard_IsKeyDown(KK_ENTER) && !m_pControllers.front()->IsConnected()) && g_PushCount[0] < 60) {
					g_PushCount[0]++;
				}

				// コントローラー長押し
				if (m_pControllers.front()->IsConnected())
				{
					// 長押し中
					if (m_pControllers.front()->Controller_IsButtonDown(XINPUT_GAMEPAD_B)
						&& g_PushCount[0] < 60)
					{
						// カウントを増やす
						g_PushCount[0]++;
					}

					// 準備完了にならずに離したら 
					if ((!m_pControllers.front()->Controller_IsButtonDown(XINPUT_GAMEPAD_B) &&
						g_PushCount[0] < 60) &&
						g_PushCount[0] >= 0)
					{
						g_PushCount[0]--;
					}

				}

				// 準備完了
				if (g_PushCount[0] >= 60)
				{
					g_PushCount[0] = 60;

					if (g_StandByFlags[0] == false)
					{
						// ここに効果音？
						SEStart(GetAppSE(), CRI_SE_STANDBY);
					}
					g_StandByFlags[0] = true;

					MatchingData data;
					data.flag = PLAYERREADY_OK;
					data.playerID = GetClient()->GetMyPlayerID();
					data.teamID = GetClient()->GetMyTeamId();
					GetClient()->SetMyMatchingData(data);
					GetClient()->SendMyMatchingData();

				}

			}

			//if (GetClient()->GetGameScene().serverstate == SERVERSTATE_MATCHING) {

			//	if (Keyboard_IsKeyTrigger(KK_ENTER)) {
			//		MatchingData data;
			//		data.flag = PLAYERREADY_OK;
			//		data.playerID = GetClient()->GetMyPlayerID();
			//		data.teamID = GetClient()->GetMyTeamId();
			//		GetClient()->SetMyMatchingData(data);
			//		GetClient()->SendMyMatchingData();
			//		//MessageBox(NULL, "準備完了状態を送信しました", "状態送信", MB_OK);
			//	}
			//}
		}
		else {
			g_Effect2d->Draw();
		}

		if (GetFadeState() == FADE_STATE_OFF || GetFadeState() == FADE_STATE_FIXED) {
			if (GetClient()->GetGameScene().serverstate == SERVERSTATE_GAME) {
				//シーンチェンジ
				FadeOut();
				SceneChange(SCENE_GAME);
			}
		}
	}
	
}

void StandByScreenDraw(){
	SetDepthEnable(false);
	
	// 背景
	SpriteDraw(g_Texture[0], SCREEN_WIDTH *0.5f, SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	if (GetGameMode() == GAMEMODE_SOLO)
	{
		// ゲージ
		SpriteDraw(g_Texture[1], SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - 65.0f,
			100.0f, 100.0f,
			0, (TextureGetHeight(g_Texture[1]) / 4) * (g_PushCount[0] / 16), TextureGetWidth(g_Texture[1]), TextureGetHeight(g_Texture[1]) / 4);

		// キャラ
		SpriteDraw(g_CharaTexture, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - 65.0f,
			80.0f, 80.0f,
			0, 0, TextureGetWidth(g_CharaTexture), TextureGetHeight(g_CharaTexture) / 4);
	}
	else if (GetGameMode() == GAMEMODE_LOCAL_MULTIWINDOW ||
		GetGameMode() == GAMEMODE_LOCAL_SPLITSCREEN)
	{
		for (int i = 0; i < g_StandByPlayerNum; i++)
		{
			// ゲージ
			SpriteDraw(g_Texture[1], 560.0f + i * 150.0f, SCREEN_HEIGHT - 65.0f,
				100.0f, 100.0f,
				0, (TextureGetHeight(g_Texture[1]) / 4) * (g_PushCount[i] / 16), TextureGetWidth(g_Texture[1]), TextureGetHeight(g_Texture[1]) / 4);
		}
		// 左側のキャラ
		SpriteDraw(g_CharaTexture, 560.0f, SCREEN_HEIGHT - 65.0f,
			80.0f, 80.0f,
			0, (TextureGetHeight(g_CharaTexture) / 4) * 0, TextureGetWidth(g_CharaTexture), TextureGetHeight(g_CharaTexture) / 4);

		// 右側のキャラ
		SpriteDraw(g_CharaTexture, 710.0f, SCREEN_HEIGHT - 65.0f,
			80.0f, 80.0f,
			0, (TextureGetHeight(g_CharaTexture) / 4) * 1, TextureGetWidth(g_CharaTexture), TextureGetHeight(g_CharaTexture) / 4);


	}
	else if (GetGameMode() == GAMEMODE_SEGMENTATION || GetGameMode() == GAMEMODE_MULTIWINDOW)
	{
	
		{// 1人目 タコ
			// ゲージ
			SpriteDraw(g_Texture[1], 410.0f + 0 * 150.0f, SCREEN_HEIGHT - 65.0f,
				100.0f, 100.0f,
				0, (TextureGetHeight(g_Texture[1]) / 4) * (g_PushCount[0] / 16), TextureGetWidth(g_Texture[1]), TextureGetHeight(g_Texture[1]) / 4);

			// キャラ
			SpriteDraw(g_CharaTexture, 410.0f + 0 * 150.0f, SCREEN_HEIGHT - 65.0f,
				80.0f, 80.0f,
				0, (TextureGetHeight(g_CharaTexture) / 4) * 0, TextureGetWidth(g_CharaTexture), TextureGetHeight(g_CharaTexture) / 4);

		}
		{// 2人目 トリ
			// ゲージ
			SpriteDraw(g_Texture[1], 410.0f + 1 * 150.0f, SCREEN_HEIGHT - 65.0f,
				100.0f, 100.0f,
				0, (TextureGetHeight(g_Texture[1]) / 4) * (g_PushCount[1] / 16), TextureGetWidth(g_Texture[1]), TextureGetHeight(g_Texture[1]) / 4);

			// キャラ
			SpriteDraw(g_CharaTexture, 410.0f + 1 * 150.0f, SCREEN_HEIGHT - 65.0f,
				80.0f, 80.0f,
				0, (TextureGetHeight(g_CharaTexture) / 4) * 2, TextureGetWidth(g_CharaTexture), TextureGetHeight(g_CharaTexture) / 4);

		}
		{// 3人目 カジキ
			// ゲージ
			SpriteDraw(g_Texture[1], 410.0f + 2 * 150.0f, SCREEN_HEIGHT - 65.0f,
				100.0f, 100.0f,
				0, (TextureGetHeight(g_Texture[1]) / 4) * (g_PushCount[2] / 16), TextureGetWidth(g_Texture[1]), TextureGetHeight(g_Texture[1]) / 4);

			// キャラ
			SpriteDraw(g_CharaTexture, 410.0f + 2 * 150.0f, SCREEN_HEIGHT - 65.0f,
				80.0f, 80.0f,
				0, (TextureGetHeight(g_CharaTexture) / 4) * 1, TextureGetWidth(g_CharaTexture), TextureGetHeight(g_CharaTexture) / 4);

		}	
		{// 4人目 ネコ
			// ゲージ
			SpriteDraw(g_Texture[1], 410.0f + 3 * 150.0f, SCREEN_HEIGHT - 65.0f,
				100.0f, 100.0f,
				0, (TextureGetHeight(g_Texture[1]) / 4) * (g_PushCount[3] / 16), TextureGetWidth(g_Texture[1]), TextureGetHeight(g_Texture[1]) / 4);

			// キャラ
			SpriteDraw(g_CharaTexture, 410.0f + 3 * 150.0f, SCREEN_HEIGHT - 65.0f,
				80.0f, 80.0f,
				0, (TextureGetHeight(g_CharaTexture) / 4) * 3, TextureGetWidth(g_CharaTexture), TextureGetHeight(g_CharaTexture) / 4);

		}
	}
	else if (GetGameMode() == GAMEMODE_ONLINE) {

		// ゲージ
		SpriteDraw(g_Texture[1], SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - 65.0f,
			100.0f, 100.0f,
			0, (TextureGetHeight(g_Texture[1]) / 4) * (g_PushCount[0] / 16), TextureGetWidth(g_Texture[1]), TextureGetHeight(g_Texture[1]) / 4);

		// キャラ
		SpriteDraw(g_CharaTexture, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - 65.0f,
			80.0f, 80.0f,
			0, TextureGetHeight(g_CharaTexture) /4 *GetClient()->GetMyPlayerID(), TextureGetWidth(g_CharaTexture), TextureGetHeight(g_CharaTexture) / 4);

		if (g_ReadyFlag) {
			g_Effect2d->Draw();
		}
	}
	SetDepthEnable(true);
}

// 全てのプレイヤーが準備完了状態かの判別
bool CheckFlgAllReady()
{
// ゲームモードによって人数が変わる
	switch (GetGameMode())
	{
	case GAMEMODE_SOLO:	
		// ソロモード
		if (g_StandByFlags[0])
		{
			return true;
		}
		break;
	case GAMEMODE_LOCAL_MULTIWINDOW:
	case GAMEMODE_LOCAL_SPLITSCREEN:
		// 2人対戦
		if (g_StandByFlags[0]&&g_StandByFlags[1])
		{
			return true;
		}
		break;
	case GAMEMODE_SEGMENTATION:
	case GAMEMODE_MULTIWINDOW:
		// 4人対戦
		if (g_StandByFlags[0] && g_StandByFlags[1] &&
			g_StandByFlags[2] && g_StandByFlags[3])
		{
			return true;
		}
		break;
	default:
		break;
	}

	// 準備完了じゃない人がいれば
	return false;
}
