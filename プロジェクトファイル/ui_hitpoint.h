/*==============================================================================

   [ui_hitpoint.h]
														 Author : Imai Hayato
														 Date   : //
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef UI_HITPOINT_H
#define UI_HITPOINT_H

#include "directx.h"

class Ship;
class Effect2D;

class HitPoint
{
private:
	Ship* m_Ship = nullptr;
	int m_Texture = -1;
	int m_OutlineTexture = -1;
	int m_HP = -1;
	int m_MaxHP = -1;
	int m_HpBarBoat = -1;
	float m_HpProportion = 0.0f;	//å≥gageÅAî‰ó¶

	Effect2D* effect2d = nullptr;
	//0:HpBar

	XMFLOAT2 HP_BarSize;
	XMFLOAT2 HP_BarPos;

public:
	HitPoint(Ship* pShip);
	~HitPoint();

	void Update();
	void Draw();
	
	

};


#endif // !UI_HITPOINT_H