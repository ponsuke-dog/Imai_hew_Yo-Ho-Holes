#include "directx.h"
#include "ui_hitpoint.h"
#include "ui_enhancegauge.h"
#include "ui_timer.h"

#include "texture.h"
#include "sprite.h"
#include "mode.h"

#include "manager_systemui.h"

SystemUI_Local_Manager::SystemUI_Local_Manager(std::vector<Ship*> ship):m_Ship(ship)
{
	if (GetGameMode() == GAMEMODE_SOLO)
	{
		m_HitPoint.push_back(new HitPoint(m_Ship[1]));
	}
	else
	{
		for (auto ShipNum : m_Ship)
		{
			m_HitPoint.push_back(new HitPoint(ShipNum));
			m_Enhancegauge.push_back(new Enhancegauge(ShipNum));
		}
	}
	m_Timer = new Timer;
	m_RopeTex = TextureLoad(L"asset\\texture\\rope.png");
}

SystemUI_Local_Manager::~SystemUI_Local_Manager()
{
	for (int i = 0; i < m_HitPoint.size(); i++)
	{
		delete m_HitPoint[i];
		m_HitPoint[i]= nullptr;
	}
	for (int i = 0; i < m_Enhancegauge.size(); i++)
	{
		delete m_Enhancegauge[i];
		m_Enhancegauge[i]= nullptr;
	}
	
	delete m_Timer;
	m_Timer = nullptr;
	
}

void SystemUI_Local_Manager::Update()
{
	for (int i = 0; i < m_HitPoint.size(); i++)
	{
		m_HitPoint[i]->Update();
	}
	for (int i = 0; i < m_Enhancegauge.size(); i++)
	{
		m_Enhancegauge[i]->Update();
	}

	m_Timer->Update();
}

void SystemUI_Local_Manager::Draw() const
{
	SetDepthEnable(false);

	if (GetGameMode() == GAMEMODE_LOCAL_SPLITSCREEN ||
		GetGameMode() == GAMEMODE_SEGMENTATION)
	{
		SpriteDraw(m_RopeTex, SCREEN_WIDTH / 2, SCREEN_HEIGHT * 3/ 4, 40.0f, SCREEN_HEIGHT/2);
		SpriteDraw(m_RopeTex, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 40.0f, SCREEN_HEIGHT/2);
	}

	for (int i = 0; i < m_HitPoint.size(); i++)
	{
		m_HitPoint[i]->Draw();
	}
	for (int i = 0; i < m_Enhancegauge.size(); i++)
	{
		m_Enhancegauge[i]->Draw();
	}
	
	m_Timer->Draw();


	SetDepthEnable(true);
}

void SystemUI_Local_Manager::SetTimer(int time)
{
	m_Timer->SetTimer(time);
}
