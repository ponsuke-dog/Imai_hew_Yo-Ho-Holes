/*==============================================================================

   [.h]
														 Author : Imai Hayato
														 Date   : //
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef MINIMAP_H
#define MINIMAP_H

#include "directx.h"

class Ship;

class MiniMap
{
private:
	Ship* m_Ships = nullptr;
	XMFLOAT2 m_Texsize = XMFLOAT2(0.0f, 30.0f * 0.8f);
	int m_TeamID = -1;
	int m_PlayerID = -1;
	bool m_Reverse = false;

	XMFLOAT3 m_Position = XMFLOAT3(0.0f,0.0f,0.0f);
	int m_Texture = -1;
	int m_Texture_Player = -1;
	int m_Texture_MinimapBG = -1;
	int m_Texture_BackLight = -1;
	int m_Texture_Black = -1;
	int m_TextureIcon = -1;
	int m_SizeRaito = 0;
	float m_PositionRectification = 0;
	bool m_PlayerLightFlg = false;
	bool m_Use = true;
public:
	MiniMap();
	~MiniMap();

	void Update();
	void Draw();


	// minimapÇÃï\é¶ÅAîÒï\é¶
	void SetMinimapUse();
	void SetMiniMap(Ship* pShip);
	void SetMiniMap(Ship* pShip,int id);
};



#endif // !MINIMAP_H