#include "configuration.h"
#include "texture.h"
#include "sprite.h"

#include "ship.h"
#include "enhance.h"

#include "ui_enhancegauge.h"

#define ENHANCEGAUGE_MAX (25)

Enhancegauge::Enhancegauge(Ship* pShip):m_pShip(pShip)
{
	m_Texture = TextureLoad(L"asset\\texture\\UI_buffBar_Single_bar.png");
	m_BGTexture = TextureLoad(L"asset\\texture\\UI_buffOutline.png");
	m_pEnhance = m_pShip->GetEnhance();

	if (m_pShip->GetTeamId()== 0)
	{
		m_Position = XMFLOAT2(178.0f, 80.0f);
		m_GaugePosition = XMFLOAT2(10.0f, 78.0f);
		m_space = 11;
		m_Reverse = false;
	}
	else
	{
		m_Position = XMFLOAT2(SCREEN_WIDTH - 178.0f, 80.0f);
		m_GaugePosition = XMFLOAT2(SCREEN_WIDTH - 10.0f, 78.0f);
		m_space = -11;
		m_Reverse = true;
	}

	
	m_Size = XMFLOAT2(350.0f, 60.0f);
}

Enhancegauge::~Enhancegauge()
{

}

void Enhancegauge::Update()
{
	m_Gauge = m_pEnhance->GetStock();
}

void Enhancegauge::Draw()
{

	SetDepthEnable(false);
		
	SpriteDraw(m_BGTexture, m_Position.x, m_Position.y, m_Size.x, m_Size.y, 0, 0, 1000, 170, 0, m_Reverse);
	for (int i = 0; i < ENHANCEGAUGE_MAX; i++)
	{
		if (m_Gauge > i)
		{
			SpriteSetColor(1.0f, 1.0f, 1.0f, 0.8f);
		}
		else
		{
			SpriteSetColor(0.0f, 0.0f, 0.0f, 0.8f);
		}
		SpriteDraw(m_Texture, m_GaugePosition.x + (m_space * i), m_GaugePosition.y, 10, 15);
	}	
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetDepthEnable(true);
}

