#include "directx.h"
#include "configuration.h"
#include "sprite.h"
#include "texture.h"
#include "countdown.h"
#include "resultannounce.h"

int g_Texture_CountDown = -1;
int g_Texture_Black_CountDown = -1;
float g_Count_CountDown = 0.0f;
bool g_CountFlag_CountDown = false;

void CountDownInitialize(){
	g_Texture_CountDown = TextureLoad(L"asset\\texture\\number_sansBold 3.png");
	g_Texture_Black_CountDown = TextureLoad(L"asset\\texture\\black.png");
	g_Count_CountDown = 0.0f;
	g_CountFlag_CountDown = false;
}

void CountDownFinalize(){
}

void CountDownUpdate(){
	if (g_CountFlag_CountDown == true) {
		if (g_Count_CountDown <= 0.0f) {
			g_CountFlag_CountDown = false;
			return;
		}

		float dt = 1.0 / 60.0f;
		if (g_Count_CountDown > 0.0f) {
			g_Count_CountDown -= dt;
			if (g_Count_CountDown < 0.0f) {
				g_Count_CountDown = 0.0f;

			}

		}

	}
	
}

void CountDownDraw(){
	if (g_CountFlag_CountDown == true) {
		int Tx = 11;
		int Ty = 1;
		int Th = 200;
		int Tw = 2200 / 11;

		if (g_Count_CountDown > 0.0f) {
			SetDepthEnable(false);

			SpriteSetColor(1.0f, 1.0f, 1.0f, 0.4f);
			SpriteDraw(g_Texture_Black_CountDown, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH, SCREEN_HEIGHT);
			SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);

			SpriteDraw(g_Texture_CountDown,
				SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 300.0f, 300.0f,
				((int)(g_Count_CountDown + 1) % Tx) * (TextureGetWidth(g_Texture_CountDown) / Tx), ((int)(g_Count_CountDown + 1) / Tx) * (TextureGetHeight(g_Texture_CountDown) / Ty),
				TextureGetWidth(g_Texture_CountDown) / Tx, TextureGetHeight(g_Texture_CountDown) / Ty);

			SetDepthEnable(true);

		}
	}

}


void StartCountDown(float count){
	g_Count_CountDown = count;
	g_CountFlag_CountDown = true;

}

bool GetCountFlag()
{
	return g_CountFlag_CountDown;
}
