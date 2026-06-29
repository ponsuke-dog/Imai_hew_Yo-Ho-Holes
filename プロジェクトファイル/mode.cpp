/*==============================================================================

  ゲームモードの変更 [mode.cpp]
														 Author : tomoya saita 
														 Date   : 2025/02/01
--------------------------------------------------------------------------------

==============================================================================*/

#include "directx.h"
#include "texture.h"
#include "keyboard.h"
#include "scene.h"
#include "sprite.h"
#include "controller.h"
#include "easing.h"

#include "cri.h"

#include "mode.h"


static GAMEMODE g_GameMode = GAMEMODE_NONE;
static ModeSceneState g_ModeSceneState = MODESCENESTATE_NONE;
#define TEXTURE_NUM	(7)
static int g_Texture[TEXTURE_NUM];

static Controller controller(0);

int g_SelectNumber = -1;
static int g_SelectIllustration = 0;
static float g_AnimationFrame = 0.0f;
static float SizePlus = 0.0f;

static float g_Interpolation[8] = {};

struct Button {
	XMFLOAT2 pos = { 0.0f,0.0f };
	XMFLOAT2 start_pos = { 0.0f,0.0f };
	XMFLOAT2 stop_pos = { 0.0f,0.0f };
	XMFLOAT2 size = { 0.0f,0.0f };

	float rate = 0.0f;

};

static Button g_button[8];

void ModeInitialize()
{
	g_GameMode = GAMEMODE_NONE;
	g_ModeSceneState = MODESCENESTATE_START;

	g_Texture[0] = TextureLoad(L"asset\\texture\\textAB.png");
	g_Texture[1] = TextureLoad(L"asset\\texture\\Button_Frame.png");
	g_Texture[2] = TextureLoad(L"asset\\texture\\GameModeAnimals.png");

	g_Texture[4] = TextureLoad(L"asset\\texture\\haikei_ita.jpg");
	g_Texture[5] = TextureLoad(L"asset\\texture\\gamemode.png");
	g_Texture[6] = TextureLoad(L"asset\\texture\\GameModeButton.png");
	

		
	// START
	for (int j = 0; j < 3; j++) {
		g_button[j].size = { 500.0f,450.0f / 3 };
		g_button[j].start_pos = { -(SCREEN_WIDTH / 4),(SCREEN_HEIGHT / 2) + (j * 150.0f) - 100 };
		g_button[j].stop_pos = { SCREEN_WIDTH / 4,(SCREEN_HEIGHT / 2) + (j * 150.0f) - 100 };
		g_button[j].pos = g_button[j].start_pos;
	}
	
	// VERSUS
	for (int j = 0; j < 2; j++) {
		g_button[3 + j].size = { 500.0f,450.0f / 3 };
		g_button[3 + j].start_pos = { -(SCREEN_WIDTH / 4),(SCREEN_HEIGHT / 2) + (j * 150.0f) - 100 };
		g_button[3 + j].stop_pos = { SCREEN_WIDTH / 4,(SCREEN_HEIGHT / 2) + (j * 150.0f) - 100 };
		g_button[3 + j].pos =g_button[3+j].start_pos;
	}

	// TEAM
	for (int j = 0; j < 3; j++) {
		g_button[5 + j].size = { 500.0f,450.0f / 3 };
		g_button[5 + j].start_pos = { -(SCREEN_WIDTH / 4),(SCREEN_HEIGHT / 2) + (j * 150.0f) - 100 };
		g_button[5 + j].stop_pos = { SCREEN_WIDTH / 4,(SCREEN_HEIGHT / 2) + (j * 150.0f) - 100 };
		g_button[5 + j].pos = g_button[5+j].start_pos;
	}
		
	g_SelectNumber = -1;		// 選択している変数
	g_AnimationFrame = 0.0f;	// アニメーションのフレーム数
	SizePlus = 0.0f;			// サイズの増減
	for (int i = 0; i < 8; i++)
	{
		g_Interpolation[i] = 0.0f;	// 各ボタンの位置移動用変数
	}
}

void ModeFinalize(){

}



void ModeUpdate() {
	controller.Controller_Update();

	// イージング用処理
	float t[8];
	for (int i = 0; i < 8; i++)
	{
		t[i] = easeInOutBack(g_Interpolation[i]) * 2;
		switch (i)
		{
		case 0:
		case 3:
		case 5:
			g_Interpolation[i] += 0.018f;
			break;
		case 1:
		case 2:
		case 4:
		case 6:
		case 7:
			if (g_button[i - 1].pos.x != g_button[i-1].start_pos.x) // 前のボタンが出発したら
			{
				g_Interpolation[i] += 0.018f;
			}
			break;
		default:
			break;
		}
		// イージング変数制限
		if (g_Interpolation[i] >= 1.0f)
		{
			g_Interpolation[i] = 1.0f;
		}
	}

	switch (g_ModeSceneState)
	{
	case MODESCENESTATE_START:	// START
		for (int i = 0; i < 3; i++)
		{
			g_button[i].pos.x = g_button[i].start_pos.x + g_button[i].stop_pos.x * t[i];
		}
		// ボタンのアニメーションが終わったら入力受け付け
		if (g_Interpolation[2] == 1.0f)
		{
			if (g_SelectNumber < 0) {
				g_SelectNumber = 0;
			}

			// モードのイラスト=選択中のモードナンバー
			g_SelectIllustration = g_SelectNumber;
			
			// メインメニューの選択処理
			if (Keyboard_IsKeyTrigger(KK_UP) ||
				controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_DPAD_UP))) {
				
				g_SelectNumber = (g_SelectNumber - 1 + 3) % 3;
				g_AnimationFrame = 0.0f;
				
				SEStart(GetAppSE(), CRI_SE_カーソル移動);
			}

			if (Keyboard_IsKeyTrigger(KK_DOWN) ||
				controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_DPAD_DOWN))) {
				
				g_SelectNumber = (g_SelectNumber + 1) % 3;
				g_AnimationFrame = 0.0f;

				SEStart(GetAppSE(), CRI_SE_カーソル移動);
			}
			// 決定キーが押されたら g_SelectMode を変更し、return して次フレームへ
			if (g_SelectNumber >= 0) {
				if (Keyboard_IsKeyTrigger(KK_SPACE) ||
					controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_B))) {
					switch (g_SelectNumber) {
					case 0:
						// 2VS2へ
						g_ModeSceneState = MODESCENESTATE_TEAM;

						break;
					case 1:
						// 1VS1へ
						g_ModeSceneState = MODESCENESTATE_VERSUS;

						break;

					case 2:
						// ソロモードへ
						g_GameMode = GAMEMODE_SOLO;
						SceneAndWindowChange(SCENE_STAND_BY);

						break;
					}
					// アニメーション関連の初期化
					g_AnimationFrame = 0.0f;
					g_SelectNumber = -1;
					SizePlus = 0;
					for (int i = 0; i < 8; i++)
					{
						g_Interpolation[i] = 0.0f;
						g_button[i].pos.x = g_button[i].start_pos.x;
					}

					SEStart(GetAppSE(), CRI_SE_決定ボタン);
				}
			}
		}
		break;

	case MODESCENESTATE_VERSUS:
		for (int i = 3; i < 5; i++)
		{
			g_button[i].pos.x = g_button[i].start_pos.x + g_button[i].stop_pos.x * t[i];
		}
		// ボタンのアニメーションが終わったら入力受け付け
		if (g_Interpolation[4] == 1.0f)
		{
			if (g_SelectNumber < 0) {
				g_SelectNumber = 0;
			}
			if (Keyboard_IsKeyTrigger(KK_TAB) ||
				controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_A))) {
				// 1つ戻る
				g_ModeSceneState = MODESCENESTATE_START;

				// アニメーション関連の初期化
				g_SelectNumber = -1;
				g_AnimationFrame = 0.0f;
				for (int i = 0; i < 8; i++)
				{
					g_Interpolation[i] = 0.0f;
					g_button[i].pos.x = g_button[i].start_pos.x;
				}

				SEStart(GetAppSE(), CRI_SE_キャンセルボタン);
			}
			// 1対1のモード選択処理
			if (Keyboard_IsKeyTrigger(KK_UP) ||
				controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_DPAD_UP))) {

				g_SelectNumber = (g_SelectNumber - 1 + 2) % 2;
				g_AnimationFrame = 0.0f;

				SEStart(GetAppSE(), CRI_SE_カーソル移動);
			}

			if (Keyboard_IsKeyTrigger(KK_DOWN) ||
				controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_DPAD_DOWN))) {

				g_SelectNumber = (g_SelectNumber + 1) % 2;
				g_AnimationFrame = 0.0f;

				SEStart(GetAppSE(), CRI_SE_カーソル移動);
			}

			if (g_SelectNumber >= 0) {
				if (Keyboard_IsKeyTrigger(KK_SPACE) ||
					controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_B))) {
					switch (g_SelectNumber) {
					case 0:
						g_GameMode = GAMEMODE_LOCAL_MULTIWINDOW;	// 2画面分割なし
						SceneAndWindowChange(SCENE_STAND_BY);
						break;
					case 1:
						g_GameMode = GAMEMODE_LOCAL_SPLITSCREEN;	// 1画面分割あり
						SceneAndWindowChange(SCENE_STAND_BY);
						break;
					}
					
					// アニメーション関連の初期化
					g_AnimationFrame = 0.0f;
					g_SelectNumber = -1;

					SEStart(GetAppSE(), CRI_SE_決定ボタン);
				}

			}
		}
		
		break;
	case MODESCENESTATE_TEAM:
		for (int i = 5; i < 8; i++)
		{
			g_button[i].pos.x = g_button[i].start_pos.x + g_button[i].stop_pos.x * t[i];
		}
		// ボタンのアニメーションが終わったら入力受け付け
		if (g_Interpolation[7] == 1.0f)
		{
			if (g_SelectNumber < 0) {
				g_SelectNumber = 0;
			}
			if (Keyboard_IsKeyTrigger(KK_TAB) ||
				controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_A))) {
				// 1つ戻る
				g_ModeSceneState = MODESCENESTATE_START;
				
				// アニメーション関連の初期化
				g_SelectNumber = -1;
				g_AnimationFrame = 0.0f;
				for (int i = 0; i < 8; i++)
				{
					g_Interpolation[i] = 0.0f;
					g_button[i].pos.x = g_button[i].start_pos.x;
				}

				SEStart(GetAppSE(), CRI_SE_キャンセルボタン);
			}
			// 2対2のモード選択処理
			if (Keyboard_IsKeyTrigger(KK_UP) ||
				controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_DPAD_UP))) {

				g_SelectNumber = (g_SelectNumber - 1 + 3) % 3;
				g_AnimationFrame = 0.0f;

				SEStart(GetAppSE(), CRI_SE_カーソル移動);
			}

			if (Keyboard_IsKeyTrigger(KK_DOWN) ||
				controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_DPAD_DOWN))) {

				g_SelectNumber = (g_SelectNumber + 1) % 3;
				g_AnimationFrame = 0.0f;

				SEStart(GetAppSE(), CRI_SE_カーソル移動);
			}

			if (g_SelectNumber >= 0) {
				if (Keyboard_IsKeyTrigger(KK_SPACE) ||
					controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_B))) {
					switch (g_SelectNumber) {
					case 0:
						g_GameMode = GAMEMODE_MULTIWINDOW;		// 4画面分割なし
						SceneAndWindowChange(SCENE_STAND_BY);
						break;
					case 1:
						g_GameMode = GAMEMODE_SEGMENTATION;		// 2画面2分割
						SceneAndWindowChange(SCENE_STAND_BY);
						break;
					case 2:
						g_GameMode = GAMEMODE_ONLINE;			// オンライン
						SceneAndWindowChange(SCENE_ENTRY);
						break;
					}
					// アニメーション関連の初期化
					g_SelectNumber = -1;

					SEStart(GetAppSE(), CRI_SE_決定ボタン);
				}

			}
		}
		break;

	default:
		break;
	}	
	g_AnimationFrame++;
}

void ModeDraw() {
	SetDepthEnable(false);

	// 共通のテクスチャ情報を取得
	int textureWidth = TextureGetWidth(g_Texture[6]);
	int textureHeight = TextureGetHeight(g_Texture[6]);
	
	// 背景
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f); // 色をリセット
	SpriteDraw(g_Texture[5], SCREEN_WIDTH / 2, 80, SCREEN_WIDTH * 0.4, SCREEN_HEIGHT * 0.2);
	SpriteDraw(g_Texture[4], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);
 
	// 右側のモードのイラスト
	SpriteDraw(g_Texture[2], SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT / 2
		, 500.0f,500.0f,
		0, (TextureGetHeight(g_Texture[2]) / 3) * (2-g_SelectIllustration),
		TextureGetWidth(g_Texture[2]), TextureGetHeight(g_Texture[2]) / 3);
	


	switch (g_ModeSceneState) {
	case MODESCENESTATE_START:
	{

		int partHeight = textureHeight / 8;
		for (int i = 0; i < 3; i++) {
			// サイズを増減
			if (g_SelectNumber == i) {
			
				SizePlus = (sinf(g_AnimationFrame / 15) + 1) * 5.0f;
			}
			else {
				SizePlus = 0;
			}
			// ボタンの表示
			SpriteDraw(
				g_Texture[6],
				g_button[i].pos.x,
				g_button[i].pos.y,
				g_button[i].size.x + SizePlus,
				g_button[i].size.y + SizePlus,
				0, i * partHeight,
				textureWidth, partHeight,
				0.0f
			);

		}
		if (g_SelectNumber != -1)	// 選択時のみ
		{
			SizePlus = (sinf(g_AnimationFrame / 15) + 1) * 5.0f;
			// 選択時の枠の表示
			SpriteDraw(g_Texture[1],
				SCREEN_WIDTH / 4,
				(SCREEN_HEIGHT / 2) + (g_SelectNumber * 150.0f) - 100,
				500.0f + SizePlus,
				(450.0f / 3) + SizePlus);
		}

	}
	break;

	case MODESCENESTATE_VERSUS:
	{
		int partHeight = textureHeight / 8;
		for (int i = 0; i < 2; i++) {
			// サイズを増減
			if (g_SelectNumber == i) {
				SizePlus = (sinf(g_AnimationFrame / 15) + 1) * 5.0f;
			}
			else {
				SizePlus = 0;
			}

			SpriteDraw(
				g_Texture[6],
				g_button[i+3].pos.x,
				g_button[i+3].pos.y,
				g_button[i+3].size.x + SizePlus,
				g_button[i+3].size.y + SizePlus,
				0, (i + 3) * partHeight,
				textureWidth, partHeight,
				0.0f
			);

		}
		if (g_SelectNumber != -1)
		{
			SizePlus = (sinf(g_AnimationFrame / 15) + 1) * 5.0f;

			// 選択時の枠の表示
			SpriteDraw(g_Texture[1],
				SCREEN_WIDTH / 4,
				(SCREEN_HEIGHT / 2) + (g_SelectNumber * 150.0f) - 100,
				500.0f + SizePlus,
				(450.0f / 3) + SizePlus);
		}

		// 「Aボタンで戻る」
		SpriteDraw(g_Texture[0], 100.0f, SCREEN_HEIGHT - 50.0f,
			200.0f, 50.0f,
			0, 200, 800, 100);

	}
	break;

	case MODESCENESTATE_TEAM:
	{
		int partHeight = textureHeight / 8;
		for (int i = 0; i < 3; i++) {
			// サイズを増減
			if (g_SelectNumber == i) { 
				SizePlus = (sinf(g_AnimationFrame / 15) + 1) * 5.0f;
			}
			else {
				SizePlus = 0;
			}

			SpriteDraw(
				g_Texture[6],
				g_button[i+5].pos.x,
				g_button[i+5].pos.y,
				g_button[i+5].size.x+SizePlus,
				g_button[i+5].size.y+SizePlus,
				0, (i + 5) * partHeight,
				textureWidth, partHeight,
				0.0f
			);
			
		}
		if (g_SelectNumber != -1)
		{
			SizePlus = (sinf(g_AnimationFrame / 15) + 1) * 5.0f;

			// 選択時の枠の表示
			SpriteDraw(g_Texture[1],
				SCREEN_WIDTH / 4,
				(SCREEN_HEIGHT / 2) + (g_SelectNumber * 150.0f) - 100,
				500.0f + SizePlus,
				(450.0f / 3) + SizePlus);
		}

		// 「Aボタンで戻る」
		SpriteDraw(g_Texture[0], 100.0f, SCREEN_HEIGHT - 50.0f,
			200.0f, 50.0f,
			0, 200, 800, 100);
	}
	break;

	default:
		break;
	}
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f); // 色をリセット


	SetDepthEnable(true);
}


void SetGameMode(GAMEMODE gamemode)
{
	g_GameMode = gamemode;

	g_SelectNumber = gamemode;
}

GAMEMODE GetGameMode()
{
	return g_GameMode;
}

//初期化を再度踏まずにゲームモードのみをNONEにもどす
GAMEMODE ResetGameMode()
{
	if (GetModeChangePossible())
	{
		g_GameMode = GAMEMODE_NONE;
	}

	return g_GameMode;
}

