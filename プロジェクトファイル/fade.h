/*==============================================================================

   フェード表示 [fade.h]
														 Author : Riugo Honda
														 Date   : 2025/02/13
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef FADE_H
#define FADE_H

#define FADE_TIME_IN (60)
#define FADE_TIME_OUT (60)

//外部に発信するFadeの状態
typedef enum
{
	FADE_STATE_OFF, //描画状態でもない
	FADE_STATE_IN,
	FADE_STATE_OUT,
	FADE_STATE_FIXED,//終了状態
} FADE_STATE;

//内部でのFadeの型
typedef enum 
{ 
	FADE_MODE_NONE, 
	FADE_MODE_IN, 
	FADE_MODE_OUT 
} FADE_MODE;

void FadeInitialize(void);
void FadeFinalize(void);
void FadeUpdate(void);
void FadeDraw(void);

void FadeIn(int frame = FADE_TIME_IN, float r = 0.0f, float g = 0.0f, float b = 0.0f);
void FadeOut(int frame = FADE_TIME_OUT, float r = 0.0f, float g = 0.0f, float b = 0.0f);

FADE_STATE GetFadeState();


void FadePlay(FADE_MODE mode, int frame, float r = 0.0f, float g = 0.0f, float b = 0.0f);

#endif // FADE_H

