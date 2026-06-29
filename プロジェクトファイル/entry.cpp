//	entry.cpp
//	
//
//	2025.02.10			�˓c�q��

#include "directx.h"
#include "configuration.h"
#include "texture.h"
#include "sprite.h"
#include "keyboard.h"
#include "scene.h"
#include "client.h"
#include <string>
#include "controller.h"
#include "effect_2d.h"
#include "fade.h"
#include "cri.h"
#include "entry.h"

static int numbers = -1;
static int g_BGTexture = -1;
static int g_SelectTexture = -1;
static int g_ButtonUITexture = -1;
static int g_GuideTexture = -1;
static int g_UnderlineTexture = -1;

static std::string g_num;
static int g_Select = 0;

static Controller* g_Controller = nullptr;

static bool g_Connected = false;
static Effect2D* g_Effect2d = nullptr;

#define NUMBER_OF_SELECT (11) 
#define INPUT_MAX (15) //入力上限

void EntryInitialize()
{
	numbers = TextureLoad(L"asset\\texture\\Dela_Number.png");
	g_BGTexture = TextureLoad(L"asset\\texture\\haikei_ita.jpg");
	g_SelectTexture = TextureLoad(L"asset\\texture\\IPaddressCursor.png");
	g_ButtonUITexture = TextureLoad(L"asset\\texture\\UI_B.png");
	g_GuideTexture = TextureLoad(L"asset\\texture\\IPaddressImage.png");
	g_UnderlineTexture = TextureLoad(L"asset\\texture\\black.png");

	//�A�h���X���͗p�ϐ�������
	if (!g_num.empty()) {
		g_num.clear();
	}

	g_Select = 0;

	g_Controller = new Controller(0);

	g_Connected = false;

	g_Effect2d = new Effect2D(TextureLoad(L"asset\\texture\\logo_rotate.png"), { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f }, { 512,512 }, 8, 1, 5, EFFECTTYPE_LOOP);

	CreateClient();
}

void EntryFinalize()
{
	delete g_Effect2d;
	g_Effect2d = nullptr;

	delete g_Controller;
	g_Controller = nullptr;
}

void EntryUpdate()
{
	g_Controller->Controller_Update();

	if (!g_Connected) {
			if (g_num.size() < INPUT_MAX)
			{
				if (Keyboard_IsKeyTrigger(KK_D1)) g_num += "1";
				if (Keyboard_IsKeyTrigger(KK_D2)) g_num += "2";
				if (Keyboard_IsKeyTrigger(KK_D3)) g_num += "3";
				if (Keyboard_IsKeyTrigger(KK_D4)) g_num += "4";
				if (Keyboard_IsKeyTrigger(KK_D5)) g_num += "5";
				if (Keyboard_IsKeyTrigger(KK_D6)) g_num += "6";
				if (Keyboard_IsKeyTrigger(KK_D7)) g_num += "7";
				if (Keyboard_IsKeyTrigger(KK_D8)) g_num += "8";
				if (Keyboard_IsKeyTrigger(KK_D9)) g_num += "9";
				if (Keyboard_IsKeyTrigger(KK_D0)) g_num += "0";
				if (Keyboard_IsKeyTrigger(KK_OEMPERIOD)) g_num += ".";
			}


		//十字キー左右でカーソル移動
		if ((!g_Controller->IsConnected() && Keyboard_IsKeyTrigger(KK_LEFT)) || (g_Controller->IsConnected() && g_Controller->Controller_IsJustPressed(g_Controller->GetButtonForTrigger(XINPUT_GAMEPAD_DPAD_LEFT))))
			MoveSelect(0);
		if ((!g_Controller->IsConnected() && Keyboard_IsKeyTrigger(KK_RIGHT)) || (g_Controller->IsConnected() && g_Controller->Controller_IsJustPressed(g_Controller->GetButtonForTrigger(XINPUT_GAMEPAD_DPAD_RIGHT))))
			MoveSelect(1);

		//Bボタンで数字入力
		if (((!g_Controller->IsConnected() && Keyboard_IsKeyTrigger(KK_SPACE)) || (g_Controller->IsConnected() && g_Controller->Controller_IsJustPressed(g_Controller->GetButtonForTrigger(XINPUT_GAMEPAD_B)))) && g_num.size() < INPUT_MAX)
		{
			if (g_Select == 10)
				g_num += ".";
			else
				g_num += std::to_string(g_Select);

			SEStart(GetAppSE(), CRI_SE_クリック音);
		}

		//Yボタンで全文字削除
		if ((!g_Controller->IsConnected() && Keyboard_IsKeyTrigger(KK_DELETE)) || (g_Controller->IsConnected() && g_Controller->Controller_IsJustPressed(g_Controller->GetButtonForTrigger(XINPUT_GAMEPAD_Y))))
		{
			if (!g_num.empty()) {
				g_num.clear(); // ���͂��N���A����
			}

			SEStart(GetAppSE(), CRI_SE_キャンセルボタン);
		}

		//Xボタンで一文字削除
		if ((!g_Controller->IsConnected() && Keyboard_IsKeyTrigger(KK_BACK)) || (g_Controller->IsConnected() && g_Controller->Controller_IsJustPressed(g_Controller->GetButtonForTrigger(XINPUT_GAMEPAD_X))))
		{
			if (!g_num.empty()) {
				g_num.pop_back(); // �Ō��1�������폜
			}

			SEStart(GetAppSE(), CRI_SE_キャンセルボタン);
		}

		//右トリガーでサーバー接続
		if ((!g_Controller->IsConnected() && Keyboard_IsKeyTrigger(KK_ENTER)) || (g_Controller->IsConnected() && g_Controller->Controller_IsJustPressed(g_Controller->GetButtonForTrigger(XINPUT_GAMEPAD_RIGHT_THUMB)))) {
			//�A�h���X�������͂̏ꍇ�����^�[��
			if (g_num.empty()) {
				return;
			}

				auto client = GetClient();


				if (!client->ConnectToServer(g_num, PORT)) {

					MessageBox(NULL, "接続失敗", "接続", MB_OK);
					g_Connected = false;
					g_num.clear();
				}
				else {

					auto start_time = std::chrono::high_resolution_clock::now();


					client->StartReceiveThread();


					while (true) {
						if (client->GetMyTeamId() != -1 && client->GetMyTeamId() < 2 && client->GetMyPlayerID() != -1 && client->GetMyPlayerID() < 4) {
							//SceneAndWindowChange(SCENE_STAND_BY);
							g_Connected = true;
							break;
						}

						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}

					SEStart(GetAppSE(), CRI_SE_決定ボタン);
				}

				
		}
	}
	else {
		g_Effect2d->Update();
		if (GetFadeState() == FADE_STATE_OFF || GetFadeState() == FADE_STATE_FIXED) {
			if (GetClient()->GetGameScene().serverstate != SERVERSTATE_CONNECT) {
				FadeOut();
				SceneAndWindowChange(SCENE_STAND_BY);
			}
		}
	}

}

void EntryDraw()
{
	SetDepthEnable(false);

	//背景
	SpriteDraw(g_BGTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);
	//下線
	SpriteDraw(g_UnderlineTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 45.0f, 1100.0f, 5.0f);
	//上の数字一覧
	SpriteDraw(numbers, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5, 1100.0f, 100.0f);

	float selectX = 140.0f + g_Select * 100;
	//選択カーソル
	SpriteDraw(g_SelectTexture, selectX, SCREEN_HEIGHT / 5 - 5.0f, 90.0f, 110.0f);
	//ボタン吹き出し
	SpriteDraw(g_ButtonUITexture, selectX, SCREEN_HEIGHT / 5 - 95.0f, 75.0f, 75.0f);
	//操作ガイド
	SpriteDraw(g_GuideTexture, SCREEN_WIDTH / 2 + 10.0f, SCREEN_HEIGHT - 104.0f, SCREEN_WIDTH + 30.0f, 208.0f);

	float x = SCREEN_WIDTH * 0.095f;
	for (char ch : g_num)
	{
		int texX = 0;

		if (ch == '.')
		{
			texX = 10 * (TextureGetWidth(numbers) / NUMBER_OF_SELECT);
		}
		else
		{
			texX = (ch - '0') * (TextureGetWidth(numbers) / NUMBER_OF_SELECT);
		}

		SpriteDraw(numbers, x, SCREEN_HEIGHT / 2,
			100.0f, 100.0f, texX, 0, (TextureGetWidth(numbers) / NUMBER_OF_SELECT), TextureGetHeight(numbers));

		x += 80;
	}

	if (g_Connected) {
		g_Effect2d->Draw();
	}

	SetDepthEnable(true);
}

void MoveSelect(int direction)
{
	//カーソルの移動
	switch (direction)
	{
	case 0: //左移動
		g_Select = (g_Select - 1 + NUMBER_OF_SELECT) % NUMBER_OF_SELECT;
		break;
	case 1: //右移動
		g_Select = (g_Select + 1) % NUMBER_OF_SELECT;
		break;
	default:
		break;
	}

	SEStart(GetAppSE(), CRI_SE_カーソル移動);
}