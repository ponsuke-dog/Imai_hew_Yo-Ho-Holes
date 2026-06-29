#include "directx.h"
#include "configuration.h"
#include "sprite.h"
#include "texture.h"
#include "countdown.h"
#include "easing.h"
#include "resultannounce.h"
#include "controller.h"
#include "mode.h"
#include "keyboard.h"

static int g_Texture_Black_ResultAnnounce = -1;
static int g_Texture_Button = -1;
static int g_Texture_GameSet = -1;

static bool PushFlags = false;

static XMFLOAT2 g_GameSetPosition = { 0.0f,0.0f };

static float g_rate = 0.0f;

typedef enum {
	ANOUNCE_SCENE_MOVE,
	ANOUNCE_SCENE_INPUT

}ResultAnouunceScene;

static ResultAnouunceScene g_Scene = ANOUNCE_SCENE_MOVE;

//bool g_CountFlag_ResultAnnounce = false;

void ResultAnnounceInitialize() {
	
	g_Texture_Black_ResultAnnounce = TextureLoad(L"asset\\texture\\black.png");
	g_Texture_GameSet = TextureLoad(L"asset\\texture\\game_set.png");
	g_Texture_Button = TextureLoad(L"asset\\texture\\game_set_button.png");


	PushFlags = false;

	g_GameSetPosition = { 0.0f,0.0f };
	
	g_rate = 0.0f;

	g_Scene = ANOUNCE_SCENE_MOVE;

}

void ResultAnnounceFinalize() {
	
}

void ResultAnnounceUpdate(std::vector<Controller*> con) {
	
	
	switch (g_Scene)
	{
	case ANOUNCE_SCENE_MOVE:
	{
		float ease = easeOutBounce(g_rate);
		g_GameSetPosition.y = (-SCREEN_HEIGHT * 0.5f) * (1.0f - ease) + (SCREEN_HEIGHT * 0.5f) * (ease);

		g_rate += 1.0f / 90.0f;
		if (g_rate >= 1.0f) {
			g_rate = 1.0f;
			g_GameSetPosition.y = SCREEN_HEIGHT * 0.5f;
			g_Scene = ANOUNCE_SCENE_INPUT;
		}
	}
		break;

	case ANOUNCE_SCENE_INPUT:
		if (!PushFlags) {
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				PushFlags = true;
			}

			for (auto controller : con) {
			
			if (controller->IsConnected() && controller->Controller_IsJustPressed(controller->GetButtonForTrigger(XINPUT_GAMEPAD_B))) {
				PushFlags = true;
				break;
			}
			}
		}
		break;

	default:
		break;
	}
}

void ResultAnnounceDraw() {
	
	SetDepthEnable(false);

	SpriteSetColor(1.0f, 1.0f, 1.0f, 0.4f);
	SpriteDraw(g_Texture_Black_ResultAnnounce, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH, SCREEN_HEIGHT);
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);

	SpriteDraw(g_Texture_GameSet, SCREEN_WIDTH * 0.5f, g_GameSetPosition.y, 1000.0f, 200.0f);
	SpriteDraw(g_Texture_Button, SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f+250.0f , 300.0f, 150.0f);


	SetDepthEnable(true);
}

bool GetResultAnnouncePushFlags()
{
	return PushFlags;
}


