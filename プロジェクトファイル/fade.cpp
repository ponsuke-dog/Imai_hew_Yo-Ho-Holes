/*==============================================================================

   フェード表示 [fade.cpp]
														 Author : Riugo Honda
														 Date   : 2025/02/13
--------------------------------------------------------------------------------

==============================================================================*/
#include "directx.h"
#include "texture.h"
#include "configuration.h"
#include "sprite.h"
#include "scene.h"
#include "fade.h"


/*------------------------------------------------------------------------------
   グローバル変数
------------------------------------------------------------------------------*/
static int g_TextureWhite = -1;
static FADE_MODE g_Mode = FADE_MODE_NONE;
static int g_Frame = 0;
static float g_R = 0.0f, g_G = 0.0f, g_B = 0.0f, g_A = 0.0f;
static FADE_STATE g_State = FADE_STATE_OFF;
static int g_FrameCount = 0;



/*------------------------------------------------------------------------------
   関数定義
------------------------------------------------------------------------------*/
void FadeInitialize(void)
{
	g_TextureWhite = TextureLoad(L"asset\\texture\\White.png");


	g_Mode = FADE_MODE_NONE;
	g_Frame = 0;
	g_R = g_G = g_B = g_A = 0.0f;
	g_State = FADE_STATE_OFF;
	g_FrameCount = 0;
}

void FadeFinalize(void)
{
}

void FadeUpdate(void)
{
	if (g_State == FADE_STATE_OFF || g_State == FADE_STATE_FIXED)
	{
		return;
	}
	switch (g_State)
	{
	case FADE_STATE_IN:
		g_A =1.0f - (1.0f * ((float)g_FrameCount / g_Frame));
		break;
	case FADE_STATE_OUT:
		g_A = 1.0f * ((float)g_FrameCount / g_Frame);
		break;
	default:
		break;
	}
	
	if (g_FrameCount >= g_Frame)
	{
		//この式はステートがINであれば0.0違うのであれば1.0fにするような式
		g_A = g_State == FADE_STATE_IN ? 0.0f : 1.0f;

		g_State = g_State == FADE_STATE_OUT? FADE_STATE_IN:FADE_STATE_FIXED;

		if (g_State == FADE_MODE_IN)
		{
			FadeIn();
		}

		//g_State = g_State == FADE_STATE_IN ? FADE_STATE_OFF : FADE_STATE_FIXED;
	}
	
	g_FrameCount++;
	
}

void FadeDraw(void)
{
	if (g_State == FADE_STATE_OFF)
		return;

	SpriteSetColor(g_R, g_G, g_B, g_A);
	SpriteDraw(g_TextureWhite, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void FadeIn(int frame , float r , float g , float b )
{
	g_Mode = FADE_MODE_IN;

	g_Frame = frame;

	g_R = r;
	g_G = g;
	g_B = b;

	g_A = 1.0f;
	g_State = FADE_STATE_IN;

	g_FrameCount = 0;
}

void FadeOut(int frame, float r, float g, float b)
{
	g_Mode = FADE_MODE_OUT;

	g_Frame = frame;

	g_R = r;
	g_G = g;
	g_B = b;

	g_A = 0.0f;
	g_State = FADE_STATE_OUT;

	g_FrameCount = 0;
}


FADE_STATE GetFadeState()
{
	return g_State;
}

void FadePlay(FADE_MODE mode, int frame, float r, float g, float b)
{
	g_Mode = mode;
	g_Frame = frame;
	g_R = r, g_G = g, g_B = b;

	if (mode == FADE_MODE_IN)
	{
		g_A = 1.0f;
		g_State = FADE_STATE_IN;
	}
	else if(mode == FADE_MODE_OUT)
	{
		g_A = 0.0f;
		g_State = FADE_STATE_OUT;
	}
	else
	{
		g_State = FADE_STATE_OFF;
	}
	g_FrameCount = 0;
}

