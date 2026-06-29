//********************************************************************************
//
// イベント[event.h] 
// 
//															Author : Hiroki Nakajima
//															Date   : 2025/01/18
// -------------------------------------------------------------------------------
//															Last Edited:2025/01/18
//********************************************************************************
#ifndef EVENT_H
#define EVENT_H


#include <vector>
#include <iostream>
#include "directx.h"

#define EVENT_COUNTDOWN_TIME (2)
#define EVENT_GAGE_UPPER (100)

#define GAGEPOSITION_Y (50)
#define GAGESIZE_Y (75)

class Controller;
class Ship;

enum EventState
{
	EVENTSTATE_NONE = -1,

	EVENTSTATE_COUNTDOWN,
	EVENTSTATE_EVENT,
	EVENTSTATE_ANIM_WAIT,
	EVENTSTATE_ANIM,
	EVENTSTATE_DAMAGE_WAIT,
	EVENTSTATE_DAMAGE,
	EVENTSTATE_END_WAIT,

	EVENTSTATE_MAX,
};

class Event
{
private:
	std::vector<Controller*>m_Controller;
	std::vector<Ship*>m_Ship;
	bool m_EndFlag = false;
	
protected:
	EventState m_State;

public:
	Event() = default;
	Event(std::vector<Ship*>pShip, std::vector<Controller*>pController) : m_Ship(pShip), m_Controller(pController) {}
	virtual ~Event() {}
	
	void SetShip(std::vector<Ship*> pship) { m_Ship = pship; }
	std::vector<Ship*> GetShip() const { return m_Ship; }

	void SetController(std::vector<Controller*> pController) { m_Controller = pController; }
	std::vector<Controller*> GetController() const { return m_Controller; }

	void SetEndFlag(bool flg) { m_EndFlag = flg; }
	const bool GetEndFlag()const { return m_EndFlag; }

	//純粋仮想関数
	virtual void Update() = 0;
	//virtual void Draw() = 0;
	virtual void Draw(int) = 0;
	virtual void Damage(int shipid, int damage) = 0;

};

#endif // !EVENT_H

