#include "event_rolling.h"
#include "event_mash.h"
#include "event_command.h"

#include "ship.h"
#include "player.h"
#include "client.h"
#include <random>
#include <math.h>
#include "cri.h"
#include "manager_event_local.h"


LocalEventManager::LocalEventManager(std::vector<Ship*> pShip, std::vector<Controller*> pController):m_Ship(pShip), m_Controller(pController)
{
	m_EventMemory[0] = false;
	m_EventMemory[1] = false;
	m_EventMemory[2] = false;
	
}


LocalEventManager::~LocalEventManager()
{
}

void LocalEventManager::SetEvent()
{
	if (m_Event == nullptr)
	{
		std::random_device rd;
		int eventpop = rd() % 3;
		
		while (m_EventMemory[eventpop]==true)
		{
			eventpop = rd() % 3;
		}
		
		

		switch (eventpop)
		{
		case 0:
			m_Event = new MashEvent(m_Ship, m_Controller);
			m_EventMemory[0] = true;
			SEStart(GetAppSE(), CRI_SE_イベント開始);
			break;
		case 1:
			m_Event = new RollingEvent(m_Ship, m_Controller);
			m_EventMemory[1] = true;
			SEStart(GetAppSE(), CRI_SE_イベント開始);
			break;
		case 2:
			m_Event = new CommandEvent(m_Ship, m_Controller);
			m_EventMemory[2] = true;
			SEStart(GetAppSE(), CRI_SE_イベント開始);
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < m_Ship.size(); i++)
	{
		m_Ship[i]->Reset();
	}
	for (int i = 0; i < m_Ship.size(); i++)
	{
		for (int j = 0; j < m_Ship[i]->GetPlayers().size(); j++)
		{
			m_Ship[i]->GetPlayers()[j]->Respawn();
		}
	}
}

void LocalEventManager::SetEvent(EventType type)
{
	if (m_Event == nullptr)
	{

		switch (type)
		{
		case EVENTTYPE_MASH:
			m_Event = new MashEvent(m_Ship, m_Controller);
			SEStart(GetAppSE(), CRI_SE_イベント開始);
			break;
		case EVENTTYPE_ROLING:
			m_Event = new RollingEvent(m_Ship, m_Controller);
			SEStart(GetAppSE(), CRI_SE_イベント開始);
			break;
		case EVENTTYPE_COMMAND:
			m_Event = new CommandEvent(m_Ship, m_Controller);
			SEStart(GetAppSE(), CRI_SE_イベント開始);
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < m_Ship.size(); i++)
	{
		m_Ship[i]->Reset();
	}
	/*for (int i = 0; i < m_Ship.size(); i++)
	{
		for (int j = 0; j < m_Ship[i]->GetPlayers().size(); j++)
		{
			m_Ship[i]->GetPlayers()[j]->Respawn();
		}
	}*/

	m_Ship[GetClient()->GetMyTeamId()]->GetPlayers().front()->Respawn();
}

const bool LocalEventManager::GetEndFlag() const
{
	return m_Event->GetEndFlag();
}

void LocalEventManager::DeleteEvent()
{
	delete m_Event;
	m_Event = nullptr;
}

void LocalEventManager::Update()
{
	if (m_Event)
	{
		m_Event->Update();
	}
}
//
//void LocalEventManager::Draw()
//{
//	int a = 2;
//	SetDepthEnable(false);
//	if (m_EventType != EVENTTYPE_IDLE)
//	{
//		if (!m_Event->GetEndFlag())
//		{
//			m_Event->Draw(a);
//		}
//	}
//	SetDepthEnable(true);
//}

void LocalEventManager::Draw(int windownum)
{
	SetDepthEnable(false);
	if (m_Event)
	{
		if (!m_Event->GetEndFlag())
		{
			m_Event->Draw(windownum);
		}
	}
	SetDepthEnable(true);
}
