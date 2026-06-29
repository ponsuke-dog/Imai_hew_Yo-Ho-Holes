#include "configuration.h"
#include "ship.h"
#include "texture.h"
#include "shader.h"
#include "sprite.h"
#include "keyboard.h"
#include "effect_2d.h"
#include "mode.h"

#include "ui_hitpoint.h"

#define HPBARSIZE (350.0f)
#define HP_OUTLINE_LENGTH
#define HP_OUTLINE_HEIGHT
#define HP_BAR_LENGTH
#define HP_BAR_HEIGHT


HitPoint::HitPoint(Ship* pShip) :m_Ship(pShip)
{
	m_Texture = TextureLoad(L"asset\\texture\\UI_HpBarAnimation_Patan.png");
	m_OutlineTexture = TextureLoad(L"asset\\texture\\UI_hpOutline.png");
	m_HpBarBoat = TextureLoad(L"asset\\texture\\UI_hpBarBoat.png");
	m_MaxHP = m_Ship->GetShipHP()/* - 1000*/;

	HP_BarPos.y = 30.0f;
	if (GetGameMode()==GAMEMODE_SOLO)
	{
		HP_BarPos.y = 45.0f;
		effect2d = new Effect2D(m_Texture, HP_BarPos, HP_BarSize, 1, 5, 12, EFFECTTYPE_LOOP, true);
	}
	else
	{
		if (pShip->GetTeamId() == 0)
		{
			effect2d = new Effect2D(m_Texture, HP_BarPos, HP_BarSize, 1, 5, 12, EFFECTTYPE_LOOP);
		}
		else
		{
			effect2d = new Effect2D(m_Texture, HP_BarPos, HP_BarSize, 1, 5, 12, EFFECTTYPE_LOOP, true);
		}
	}



}

HitPoint::~HitPoint()
{
	delete effect2d;
}

void HitPoint::Update()
{
	//Hpの割合計算はちゃんとfloatになるように計算すること...
	m_HpProportion = (float)m_Ship->GetShipHP() / (float)m_MaxHP;

	if (m_HpProportion > 1.0f)
	{
		m_HpProportion = 1.0f;
	}

	if (GetGameMode() == GAMEMODE_SOLO)
	{
		HP_BarPos.x = SCREEN_WIDTH - 10.0f - (HPBARSIZE / 2) * m_HpProportion * 1.5f;

		HP_BarSize.x = HPBARSIZE * m_HpProportion * 1.5f;
		HP_BarSize.y = 75.0f;
	}
	else
	{
		if (m_Ship->GetTeamId() == 0)
			HP_BarPos.x = 10.0f + (HPBARSIZE / 2) * m_HpProportion;
		else
			HP_BarPos.x = SCREEN_WIDTH - 10.0f - (HPBARSIZE / 2) * m_HpProportion;

		HP_BarSize.x = HPBARSIZE * m_HpProportion;
		HP_BarSize.y = 50.0f;
	}
	effect2d->SetSize(HP_BarSize);
	effect2d->SetPosition(HP_BarPos);

	//m_HP = m_Ship->GetShipHP() /*- 22100*/;
	/*if (m_HP <= 0.0f)
	{
		m_HP = 0.0f;
	}*/

	effect2d->Update();
}

void HitPoint::Draw()
{
	SetDepthEnable(false);

	effect2d->Draw();
	if (GetGameMode() == GAMEMODE_SOLO)
	{
		// HPbarボート
		SpriteDraw(m_HpBarBoat, HP_BarPos.x - (HP_BarSize.x / 2), HP_BarPos.y, 50.0f, 50.0f,
			0, 0, TextureGetWidth(m_HpBarBoat), TextureGetHeight(m_HpBarBoat), 0, true);
		// Outline
		SpriteSetColor(0.5f, 0.5f, 1.0f, 0.7f);
		SpriteDraw(m_OutlineTexture, SCREEN_WIDTH - 55.0f - (HPBARSIZE / 2)*1.5f, 45.0f, (HPBARSIZE + 70.0f)*1.5f, 60.0f*1.5f,
			0, 0, TextureGetWidth(m_OutlineTexture), TextureGetHeight(m_OutlineTexture), 0, true);

	}
	else
	{
		if (m_Ship->GetTeamId() == 0)
		{
			// HPbarボート
			SpriteDraw(m_HpBarBoat, HP_BarPos.x + (HP_BarSize.x / 2), HP_BarPos.y, 50.0f, 50.0f,
				0, 0, TextureGetWidth(m_HpBarBoat), TextureGetHeight(m_HpBarBoat), 0, false);
			// Outline
			SpriteSetColor(1.0f, 0.5f, 0.5f, 0.7f);
			SpriteDraw(m_OutlineTexture, 40.0f + (HPBARSIZE / 2), 30.0f, HPBARSIZE + 70.0f, 60.0f,
				0, 0, TextureGetWidth(m_OutlineTexture), TextureGetHeight(m_OutlineTexture), 0, false);
		}
		else
		{
			// HPbarボート
			SpriteDraw(m_HpBarBoat, HP_BarPos.x - (HP_BarSize.x / 2), HP_BarPos.y, 50.0f, 50.0f,
				0, 0, TextureGetWidth(m_HpBarBoat), TextureGetHeight(m_HpBarBoat), 0, true);
			// Outline
			SpriteSetColor(0.5f, 0.5f, 1.0f, 0.7f);
			SpriteDraw(m_OutlineTexture, SCREEN_WIDTH - 40.0f - (HPBARSIZE / 2), 30.0f, HPBARSIZE + 70.0f, 60.0f,
				0, 0, TextureGetWidth(m_OutlineTexture), TextureGetHeight(m_OutlineTexture), 0, true);
		}
	}
	
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);

	//float damage = m_MaxHP - m_HP;

	//if (m_Ship->GetTeamId() == 0)
	//{
	//	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
	//	effect2d->Draw();

	//	//SpriteDraw(
	//	//	m_Texture, 
	//	//	10.0f + (HPBARSIZE / 2) * m_HpProportion,
	//	//	30.0f, 
	//	//	HPBARSIZE* m_HpProportion,
	//	//	50.0f, 
	//	//	0 + m_HpProportion *damage,
	//	//	0, 
	//	//	950 , 
	//	//	200, 
	//	//	0, 
	//	//	false);
	//	//SpriteDraw(m_OutlineTexture, 10.0f + (HPBARSIZE / 2) *gage, 30.0f, HPBARSIZE *gage , 50.0f, 0 + gage*damage, 0, 1000 , 200, 0, false);
	//}
	//else
	//{
	//	effect2d->Draw();
	//	/*SpriteDraw(
	//		m_Texture,
	//		SCREEN_WIDTH - (HPBARSIZE / 2) * m_HpProportion - 10.0f,
	//		30.0f,
	//		HPBARSIZE * m_HpProportion,
	//		50.0f,
	//		0 + m_HpProportion *damage,
	//		0,
	//		950,
	//		200,
	//		0, true);*/
	//		//SpriteDraw(m_OutlineTexture, SCREEN_WIDTH - (HPBARSIZE / 2) * gage - 10.0f, 30.0f, HPBARSIZE * gage, 50.0f, 0 +gage*damage, 0, 1000, 200, 0, true);
	//}
	SetDepthEnable(true);
}
