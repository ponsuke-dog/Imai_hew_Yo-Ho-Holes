/*==============================================================================

   [.h]
														 Author : Imai Hayato
														 Date   : //
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef MANAGER_EVENT_LOCAL_H
#define MANAGER_EVENT_LOCAL_H

#include <iostream>
#include <vector>
#include "packet.h"

class Controller;
class Ship;
class Event;
class RollingEvent;

class LocalEventManager
{
private:
	std::vector<Controller*> m_Controller;
	std::vector<Ship*> m_Ship;
	Event* m_Event = nullptr;
	bool m_EventMemory[3] = {};
public:
	LocalEventManager(std::vector<Ship*>pShip, std::vector<Controller*> pController);
	~LocalEventManager();

	void SetEvent();
	void SetEvent(EventType type);

	const bool GetEndFlag()const;
	void DeleteEvent();

	void Update();
	//void Draw();
	void Draw(int windownum);


};

#endif // !MANAGER_EVENT_LOCAL_H