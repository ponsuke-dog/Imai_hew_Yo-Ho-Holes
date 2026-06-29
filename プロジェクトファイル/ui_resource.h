/*==============================================================================

   [.h]
														 Author : Imai Hayato
														 Date   : //
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef UI_RESOURCE_H
#define UI_RESOURCE_H

#include "directx.h"
#include "player.h"

class Player;
class Resource
{
private:
	Player* m_Player = nullptr;

	int m_Texture = -1;
	int m_ResourceTexture[2] = {};
	XMFLOAT3 m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_HandPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	bool m_Reverse = false;
	int m_SizeRaito = 0;
public:
	Resource();
	~Resource();
	void Update();
	void Draw();

	void SetPlayerResource(Player* pPlayer);
};


#endif // !UI_RESOURCE_H