#include "directx.h"
#include "configuration.h"
#include "texture.h"
#include "scene.h"
#include "shader.h"
#include "keyboard.h"
#include "controller.h"
#include "sprite.h"
#include "fade.h"
#include "effect_2d.h"
#include "easing.h"
#include "cri.h"
#include "title.h"

static Controller controller(0);
static int g_TitleVideoTexture = -1;
static int g_TitleLogoTexture = -1;
static float g_TextureZoomFrameCount = -1.0f;
static float g_TextureZoomFrameMax = 90.0f;
static int g_TextureButton = -1;

float rate = 0.0f;

Effect2D* g_TitleMovie = nullptr;
Effect2D* g_TitleButton = nullptr;

static std::vector<Controller*>m_pControllers;

void TitleInitialize(void)
{
	//g_Texture = TextureLoad(L"asset\\texture\\hune.png");
	g_TextureZoomFrameCount = -1.0f;

	g_TitleVideoTexture = TextureLoad(L"asset\\texture\\opening_sheet.png");
	g_TitleLogoTexture = TextureLoad(L"asset\\texture\\TitleLogo.png");
	g_TextureButton = TextureLoad(L"asset\\texture\\button_title.png");

	g_TitleMovie = new Effect2D(g_TitleVideoTexture, XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), 10, 10, 3, EFFECTTYPE_LOOP);
	g_TitleButton = new Effect2D(g_TextureButton, XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.3f), XMFLOAT2(720 * 0.5f, 720 * 0.5f), 6, 6, 3, EFFECTTYPE_LOOP);

	rate = 0.0f;

	m_pControllers.push_back(new Controller(0));
	m_pControllers.push_back(new Controller(1));
	m_pControllers.push_back(new Controller(2));
	m_pControllers.push_back(new Controller(3));
}

void TitleFinalize(void)
{
	/*for (int i = 0; i < m_pControllers.size(); i++) {
		if (m_pControllers[i]) {
			delete m_pControllers[i];
			m_pControllers[i] = nullptr;
		}
	}*/
	m_pControllers.clear();

	delete g_TitleButton;
	delete g_TitleMovie;
}

void TitleUpdate(void)
{



	if (GetFadeState() != FADE_STATE_IN)
	{
		//controller.Controller_Update();

		m_pControllers[0]->Controller_Update();
		m_pControllers[1]->Controller_Update();
		m_pControllers[2]->Controller_Update();
		m_pControllers[3]->Controller_Update();

		for (int i = 0 ; i < 4 ; i++)
		{
			if (Keyboard_IsKeyTrigger(KK_ENTER) || m_pControllers[i]->Controller_IsJustPressed(m_pControllers[i]->GetButtonForTrigger(XINPUT_GAMEPAD_B)))
			{
				SceneChange(SCENE_MODE_SELECT);
				SEStart(GetAppSE(), CRI_SE_ゲームスタート);
			}
		}

		g_TitleMovie->Update();
		g_TitleButton->Update();
		
		if (g_TextureZoomFrameCount < g_TextureZoomFrameMax)
			g_TextureZoomFrameCount += 1.0f;


		/*if (Keyboard_IsKeyTrigger(KK_ENTER) || controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_B)))
		{
			SceneChange(SCENE_MODE_SELECT);
		}

		if (Keyboard_IsKeyTrigger(KK_SPACE) || controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_A)))
		{
			SceneAndWindowChange(SCENE_TITLE);
		}

		if (Keyboard_IsKeyTrigger(KK_TAB) || controller.Controller_IsJustPressed(controller.GetButtonForTrigger(XINPUT_GAMEPAD_X)))
		{
			SceneAndWindowChange(SCENE_TITLE);
		}*/

	rate += 0.1f;
		
	}
	


}

void TitleDraw(void)
{
	SetDepthEnable(false);

	
	float TextureRatio = g_TextureZoomFrameCount / g_TextureZoomFrameMax;

	float TextureWidth = 0.0f;
	float TextureHeight = 0.0f;
	if (g_TextureZoomFrameCount != g_TextureZoomFrameMax) {
		TextureWidth = SCREEN_WIDTH / 2 * easeOutBack(TextureRatio);
		TextureHeight = SCREEN_HEIGHT / 2 * easeOutBack(TextureRatio);

	}
	else {
		TextureWidth = SCREEN_WIDTH / 2 + ((1 + cosf(rate)) * 3.0f);
		TextureHeight = SCREEN_HEIGHT / 2 + ((1 + cosf(rate)) * 3.0f);
	}



	g_TitleMovie->Draw();
	SpriteDraw(g_TitleLogoTexture, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, TextureWidth, TextureHeight);

	float position_x = SCREEN_WIDTH / 2 + (-sinf(rate) * 10.0f);
	float position_y = SCREEN_HEIGHT / 1.3f + (cosf(rate) * 10.0f);
	g_TitleButton->SetPosition({ position_x,position_y });
	g_TitleButton->Draw();

	SetDepthEnable(true);

}
