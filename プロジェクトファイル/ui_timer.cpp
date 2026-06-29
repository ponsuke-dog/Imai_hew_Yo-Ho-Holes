#include "configuration.h"
#include "texture.h"
#include "shader.h"
#include "sprite.h"
#include "easing.h"
#include "mode.h"

#include "cri.h"

#include "ui_timer.h"

#define NUMSIZE (50.0f)		// ŠÔ‚Ì”š‚ÌƒTƒCƒY

#define MINUTES (3*60) // (İ’è‚µ‚½‚¢•ª * 60) Œ»İ3•ª‚Éİ’èi–{“c2025/02/20)

// c‚è‰½•b‚ÅŒx‚ğ”­‚·‚é‚©	(İ’è‚µ‚½‚¢•bˆÈ‰º‚É‚È‚Á‚½‚ç)
#define FIRSTATTENTIONTIME (15)
#define SECONDATTENTIONTIME (25)
#define THIRDATTENTIONTIME (35)


Timer::Timer()
{
	m_Texture = TextureLoad(L"asset\\texture\\UI_Timer_05.png");
	m_Texture_time = TextureLoad(L"asset\\texture\\number_sansBold 3.png");
	m_TextureAttention = TextureLoad(L"asset\\texture\\warningBar01.png");

	// ‰Šú‰» (”O‚Ì‚½‚ß)
	m_Attentionflg = false;
	m_AttentionTexPosX = 0;
	m_Time = 0;
	m_AttentionPositionX = SCREEN_WIDTH * 2;

}

Timer::~Timer()
{
}

void Timer::Update()
{
	if (GetGameMode() == GAMEMODE_SOLO)
	{
		m_maxtime = 60;	// maxtime‚Éc‚èŠÔ(•b)‚ğİ’è
	}
	else
	{
		m_maxtime = MINUTES;	// maxtime‚Éc‚èŠÔ(•b)‚ğİ’è
	}
	
	m_maxtime -= m_Time;	// c‚è‚ÌŠÔ‚ğˆø‚¢‚Ä‚¢‚­

	// 60•b‚Åˆê‰ñA5•b‘O‚©‚çŒx
	//if (m_maxtime % 60 <= ATTENTIONTIME && m_maxtime != MINUTES)
	//{
	//	if (m_AttentionPositionX >= SCREEN_WIDTH / 2)
	//	{
	//		float Insert = easeInQuad(m_AttentionAlpha);
	//		m_AttentionPositionX -= Insert/2;
	//		// §ŒÀ
	//		if (m_AttentionPositionX <= SCREEN_WIDTH / 2)
	//		{
	//			m_AttentionPositionX = SCREEN_WIDTH / 2;
	//		}
	//	}
	//	m_Attentionflg = true;
	//	m_AttentionTexPosX++;
	//	m_AttentionAlpha++;
	//}
	if (m_maxtime % 60 <= FIRSTATTENTIONTIME && (m_maxtime > 130 && m_maxtime < 160) && GetGameMode() != GAMEMODE_SOLO)
	{
		if (m_AttentionPositionX >= SCREEN_WIDTH / 2)
		{
			float Insert = easeInQuad(m_AttentionAlpha);
			m_AttentionPositionX -= Insert / 2;
			// §ŒÀ
			if (m_AttentionPositionX <= SCREEN_WIDTH / 2)
			{
				m_AttentionPositionX = SCREEN_WIDTH / 2;
			}
		}

		if (m_Attentionflg == false)
		{
			SEStart(GetAppSE(), CRI_SE_Œx•ñ‚ª–Â‚é);
		}
		m_Attentionflg = true;
		m_AttentionTexPosX++;
		m_AttentionAlpha++;

		
	}
	else if (m_maxtime % 60 <= SECONDATTENTIONTIME && (m_maxtime > 80 && m_maxtime < 110) && GetGameMode() != GAMEMODE_SOLO)
	{
		if (m_AttentionPositionX >= SCREEN_WIDTH / 2)
		{
			float Insert = easeInQuad(m_AttentionAlpha);
			m_AttentionPositionX -= Insert / 2;
			// §ŒÀ
			if (m_AttentionPositionX <= SCREEN_WIDTH / 2)
			{
				m_AttentionPositionX = SCREEN_WIDTH / 2;
			}
		}

		if (m_Attentionflg == false)
		{
			SEStart(GetAppSE(), CRI_SE_Œx•ñ‚ª–Â‚é);
		}

		m_Attentionflg = true;
		m_AttentionTexPosX++;
		m_AttentionAlpha++;

	}
	else if (m_maxtime % 60 <= THIRDATTENTIONTIME && (m_maxtime > 30 && m_maxtime < 60) && GetGameMode() != GAMEMODE_SOLO)
	{
		if (m_AttentionPositionX >= SCREEN_WIDTH / 2)
		{
			float Insert = easeInQuad(m_AttentionAlpha);
			m_AttentionPositionX -= Insert / 2;
			// §ŒÀ
			if (m_AttentionPositionX <= SCREEN_WIDTH / 2)
			{
				m_AttentionPositionX = SCREEN_WIDTH / 2;
			}
		}

		if (m_Attentionflg == false)
		{
			SEStart(GetAppSE(), CRI_SE_Œx•ñ‚ª–Â‚é);
		}

		m_Attentionflg = true;
		m_AttentionTexPosX++;
		m_AttentionAlpha++;

		
	}
	else // Œx‚È‚µ
	{
		m_Attentionflg = false;
		m_AttentionTexPosX = 0;
		m_AttentionAlpha = 0;
		m_AttentionPositionX = SCREEN_WIDTH * 2;
	}
	
}

void Timer::Draw()
{
	// F•ÏX
	SpriteSetColor(1.0f, 1.0f, 0.0f, 1.0f);


	m_Position.y = 50.0f;
	m_Size = XMFLOAT2(25.0f, 25.0f);
	if (GetGameMode()==GAMEMODE_SOLO)
	{
		m_Position.x = 150.0f;
	}
	else
	{
		m_Position.x = SCREEN_WIDTH / 2;
	}
	if (GetGameMode()==GAMEMODE_SOLO)
	{
		// 15•bˆÈ‰º‚È‚ç
		if (m_maxtime<=15)
		{
			SpriteSetColor(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}
	else
	{
		// 30•bˆÈ‰º‚È‚ç
		if (m_maxtime <= 30)
		{
			SpriteSetColor(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}
	// •ª‚İ
	SpriteDraw(m_Texture_time, m_Position.x - 43.5f, m_Position.y,
		m_Size.x, m_Size.y, ((m_maxtime / 60) % (MINUTES + 1)) * 200, 0, 200, 200);

	// ƒRƒƒ“@: 
	SpriteDraw(m_Texture_time, m_Position.x - 15.0f, m_Position.y,
		m_Size.x, m_Size.y, 10 * 200, 0, 200, 200);

	// •b”‚Ì‚İ‚¾‚¯•\¦
	int second = m_maxtime % 60;

	// •b”
	for (int i = 2; i > 0; i--, second /= 10)
	{
		SpriteDraw(m_Texture_time, m_Position.x + (i * m_Size.x) - 10.0f, m_Position.y,
			m_Size.x, m_Size.y, second % 10 * 200, 0, 200, 200);
	}
	
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);

	// ”wŒi˜g‘g‚İ
	SpriteDraw(m_Texture, m_Position.x, 100.0f, 200, 200);

	// ƒCƒxƒ“ƒg—\ƒAƒjƒ[ƒVƒ‡ƒ“
	if (m_Attentionflg)
	{
		// easingŠ|‚¯‚é‚½‚ß‚Ìˆ—
		float repeat = m_AttentionAlpha * 0.05f;

		float alpha = easeInOutSine(repeat);
		alpha = sinf(repeat);
		alpha = (alpha / 2)+0.8f;
		// “_–Å
		SpriteSetColor(2.0f, 2.0f, 2.0f, alpha);
		// —\
		SpriteDraw(m_TextureAttention, m_AttentionPositionX, SCREEN_HEIGHT / 6,
			SCREEN_WIDTH, 50.0f, m_AttentionTexPosX, 0,
			TextureGetWidth(m_TextureAttention) * 3, TextureGetHeight(m_TextureAttention));
	}

	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Timer::SetTimer(int time)
{
	m_Time = time;
}

