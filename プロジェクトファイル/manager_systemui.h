/*==============================================================================

   [.h]
														 Author : Imai Hayato
														 Date   : //
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef MANAGER_SYSTEMUI_LOCAL_H
#define MANAGER_SYSTEMUI_LOCAL_H

#include <vector>
#include <iostream>

class Ship;
class HitPoint;
class Timer;
class Player;
class Enhancegauge;

class SystemUI_Local_Manager
{
private:
	std::vector<Ship*> m_Ship;
	std::vector<HitPoint*> m_HitPoint;
	std::vector< Enhancegauge*> m_Enhancegauge;
	std::vector<Player*> m_Player;
	Timer* m_Timer = nullptr;
	int m_RopeTex = -1;
public:
	SystemUI_Local_Manager(std::vector<Ship*> ship);
	~SystemUI_Local_Manager();

	void Update();
	void Draw()const;

	void SetTimer(int time);
};

#endif // !MANAGER_SYSTEMUI_LOCAL_H