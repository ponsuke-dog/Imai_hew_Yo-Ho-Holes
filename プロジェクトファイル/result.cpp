#include "directx.h"
#include "configuration.h"
#include "cri.h"
#include "mode.h"
#include "texture.h"
#include "scene.h"
#include "shader.h"
#include "keyboard.h"
#include "controller.h"
#include "sprite.h"
#include "fade.h"
#include "effect_2d.h"
#include "easing.h"
#include "result.h"

typedef enum {
	RESULTSTATE_MOVE,
	RESULTSTATE_SIZE,
}ResultState;

static Controller controller(0);
//static int g_Texture[5] = {};
static int g_ResultTexture = -1;
static int g_WinTexture = -1;
static int g_PushTexture = -1;

static Effect2D* g_ResultMovie = nullptr;
static Effect2D* g_ResultButton = nullptr;

static ResultState g_state = RESULTSTATE_MOVE;


static int g_Num = -1;

static int g_Score;

static bool g_DidShipOneWin;
static bool g_IsFading;

int frame = 0;
float win_rate = 0.0f;

void ResultInitialize(void)
{
	/*g_Texture[0] = TextureLoad(L"asset\\texture\\4.png");
	g_Texture[1] = TextureLoad(L"asset\\texture\\lose_0.5_2.png");
	g_Texture[2] = TextureLoad(L"asset\\texture\\lose_1.png");
	g_Texture[3] = TextureLoad(L"asset\\texture\\win_0.5_2.png");
	g_Texture[4] = TextureLoad(L"asset\\texture\\win_1.png");*/
	g_WinTexture = TextureLoad(L"asset\\texture\\win.png");
	g_PushTexture = TextureLoad(L"asset\\texture\\button_title.png");
	g_Num = TextureLoad(L"asset\\texture\\number_sansBold 3.png");

	g_state = RESULTSTATE_MOVE;

	win_rate = 0.0f;
	frame = 0;

	if (GetGameMode() == GAMEMODE_LOCAL_SPLITSCREEN || GetGameMode() == GAMEMODE_LOCAL_MULTIWINDOW)
	{
		if(g_DidShipOneWin)
			g_ResultTexture = TextureLoad(L"asset\\texture\\result_ship0_solo.png");
		else
			g_ResultTexture = TextureLoad(L"asset\\texture\\result_ship1_solo.png");
	}
	else if (GetGameMode() == GAMEMODE_SOLO)
	{
		g_ResultTexture = TextureLoad(L"asset\\texture\\result_ship0_solo.png");
	}
	else
	{
		if (g_DidShipOneWin)
			g_ResultTexture = TextureLoad(L"asset\\texture\\result_ship0_multi.png");
		else
			g_ResultTexture = TextureLoad(L"asset\\texture\\result_ship1_multi.png");
	}

	g_ResultMovie = new Effect2D(g_ResultTexture, { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }, { SCREEN_WIDTH, SCREEN_HEIGHT }, 11, 11, 3, EFFECTTYPE_STOP);
	g_ResultButton = new Effect2D(g_PushTexture, XMFLOAT2(SCREEN_WIDTH / 6, SCREEN_HEIGHT * 0.9f ), XMFLOAT2(256.0f, 256.0f), 6, 6, 3, EFFECTTYPE_LOOP);
	g_IsFading = false;
}

void ResultFinalize(void)
{
	delete g_ResultMovie;
}

void ResultUpdate(void)
{
	controller.Controller_Update();

	g_ResultMovie->Update();
	g_ResultButton->Update();

	if ((Keyboard_IsKeyTrigger(KK_ENTER) || controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_B))) && g_IsFading == false)
	{
		g_IsFading = true;

		FadeOut();

		SceneChange(SCENE_TITLE);

		SEStart(GetAppSE(), CRI_SE_Œˆ’èƒ{ƒ^ƒ“);
	}

	switch (g_state)
	{
	case RESULTSTATE_MOVE:

		if (frame >= 60 * 5) {
			win_rate += 1.0f / 90.0f;


		}

		if (win_rate > 1.0f) {
			win_rate = 0.0f;
			g_state = RESULTSTATE_SIZE;
		}

		frame++;

		break;
	case RESULTSTATE_SIZE:

		win_rate += 1.0f / 30.0f;

		break;
	default:
		break;
	}
}

void ResultDraw(int windownumber)
{
	SetDepthEnable(false);

	g_ResultMovie->Draw();
	g_ResultButton->Draw();

	float win_height = 0.0f;
	float win_size = 1.0f;

	float scorepos = 0.0f;


	switch (g_state)
	{
	case RESULTSTATE_MOVE:

		if (win_rate <= 1.0f) {
			win_height = (SCREEN_HEIGHT * 1.5f) * (1.0f - easeOutBack(win_rate)) + (SCREEN_HEIGHT - 200.0f) * (easeOutBack(win_rate));
			scorepos = (SCREEN_HEIGHT * 1.5f) * (1.0f - easeOutBack(win_rate)) + 150.0f;
		}

		break;
	case RESULTSTATE_SIZE:

		win_height = SCREEN_HEIGHT - 200.0f;
		win_size = (1 + (-cos(win_rate))) * 10.0f;
		scorepos = SCREEN_HEIGHT/2 - 200.0f;
		break;
	default:
		break;
	}

	SpriteDraw(g_WinTexture, SCREEN_WIDTH - 300.0f, win_height, TextureGetWidth(g_WinTexture) * 0.25f + win_size, TextureGetHeight(g_WinTexture) * 0.25f + win_size);

	if (GetGameMode()==GAMEMODE_SOLO)
	{
		int score = g_Score;
		for (int i = 4; i > 0; i--)
		{
			SpriteDraw(g_Num, SCREEN_WIDTH / 2 - 150.0f + i * 75.0f, scorepos,
				100.0f, 100.0f,
				0, 0,
				200, 200);
		}
		for (int i = 3; i > 0; i--, score /= 10)
		{
			SpriteDraw(g_Num, SCREEN_WIDTH / 2 - 375.0f + i * 75.0f, scorepos,
				100.0f, 100.0f,
				(score % 10) * 200, 0,
				200, 200);
		}
	}

	SetDepthEnable(true);
}

void SetWhoWon(bool whowon)
{
	g_DidShipOneWin = whowon;
}

void SetScore(int score)
{
	g_Score = score;
}
