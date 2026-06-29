/*==============================================================================

   [event_rolling.h]
														 Author : Imai Hayato
														 Date   : 25/01/18
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef EVENT_ROLLING_H
#define EVENT_ROLLING_H

#include "event.h"
#include <vector>
#include <iostream>
#include "directx.h"
class Ship;
class Controller;

typedef enum
{
	KEYBOARD_NONE = -1,
	KEYBOARD_W,
	KEYBOARD_D,
	KEYBOARD_S,
	KEYBOARD_A,
}KeyboardRollingDirection;


class RollingEvent :public Event
{
private:
	int m_Count[2] = {};

	int m_InstructionTex = -1;
	int m_EventBGTexture = -1;
	int m_FrameCount = 0;
	int m_Time = 0;
	int m_Texture[3] = {};
	int m_ShipTexture = -1;
	int m_WaterTexture[2] = {};
	int m_GageEffect[2] = {};
	bool m_KeyboardRollingFlag;
	// スティックAnimation
	int m_AnimationTime = 0;
	// アニメーションの上下移動
	XMFLOAT2 m_ShipPosition[2] = {};
	XMFLOAT2 m_ShipVelocity = XMFLOAT2(0,0);
	int m_Speed = 0;
	float m_ShipRotate = 0;

	float m_CountSize = 0.0f;
	int m_Rotate = 0;

	KeyboardRollingDirection m_StartRollingDirection;
	KeyboardRollingDirection m_OldRollingDirection;
	KeyboardRollingDirection m_NextRollingDirection;

	void CountUpdate();
	void EventUpdate();
	void AnimUpdate();
	void DamageUpdate();
	bool RollingKeyboard();
public:
	RollingEvent() = default;
	RollingEvent(std::vector<Ship*>pShip, std::vector<Controller*>pController);
	~RollingEvent();

	void Update()override;
	void Draw(int)override;
	void Damage(int shipid, int damage) override;

};

#endif // !EVENT_ROLLING_H