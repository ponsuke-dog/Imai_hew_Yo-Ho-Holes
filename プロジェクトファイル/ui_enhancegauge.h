/*==============================================================================

   [.h]
														 Author : Imai Hayato
														 Date   : //
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef UI_ENHANCEGAUGE_H
#define UI_ENHANCEGAUGE_H

#include "directx.h"

class Enhance;
class Ship;


class Enhancegauge
{
private:
	Enhance* m_pEnhance = nullptr;
	Ship* m_pShip = nullptr;
	int m_Gauge = -1;
	int m_Texture = -1;
	int m_BGTexture = -1;
	int m_space = 0;
	bool m_Reverse = false;
	XMFLOAT2 m_Position = XMFLOAT2(0.0f, 0.0f);
	XMFLOAT2 m_GaugePosition = XMFLOAT2(0.0f, 0.0f);
	XMFLOAT2 m_Size = XMFLOAT2(0.0f, 0.0f);
public:
	Enhancegauge(Ship* pShip);
	~Enhancegauge();
	void Update();
	void Draw();

};

#endif // !UI_ENHANCEGAUGE_H