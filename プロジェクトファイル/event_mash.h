//*******************************************************************************
//
// 連打イベント[event_mash.h] 
// 
//															Author : Hiroki Nakajima
//															Date   : 2025/01/18
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/12
//********************************************************************************
#ifndef EVENT_MASH_H
#define EVENT_MASH_H

#include "event.h"
class Effect2D;

#define EFFECT_MAX (2) //エフェクトの最大数

enum MashEventAnimState
{
	MASHSTATE_NONE = -1,

	MASHSTATE_BOMBMOVE,
	MASHSTATE_BLOWOFF,
	MASHSTATE_EXPLOSION,

	MASHSTATE_MAX,
};

class MashEvent : public Event
{
private:
	int m_Count[2] = {}; //連打カウント
	int m_FrameCount = 0; //フレーム数
	int m_Time = 5 * 60; //残り時間
	int m_ButtonAnimFrame = 0; 
	//テクスチャ
	int m_GageTexture = -1; 
	int m_CountTexture = -1; 
	int m_ButtonTex[2] = {};
	int m_BoatTex = -1;
	int m_BombTex = -1;
	int m_ExplosionTex = -1;
	int m_EventBGTexture = -1;
	int m_InstructionTex = -1; 
	int m_WaterTexture[2] = {};
	int m_GageEffect[2] = {};
	int m_AnimCount = 0;
	MashEventAnimState m_AnimState = MASHSTATE_NONE;
	float m_CountSize = 0.0f;
	float m_Angle = 0.0f;
	float m_BombSpeed = 0.0f;
	
	Effect2D* m_Effect[EFFECT_MAX]; //爆発エフェクト
	XMFLOAT2 m_BombPosition[2] = { {SCREEN_WIDTH / 2 - 47.0f, 550.0f}, {SCREEN_WIDTH / 2 + 47.0f, 550.0f} }; 

public:
	MashEvent(std::vector<Ship*> pShip, std::vector<Controller*> pController);
	~MashEvent();

	void Update() override;
	void Draw(int) override;

private: //コードを見やすくするための関数
	void Damage(int shipid, int damage)override;
	void EventUpdate();
	void AnimUpdate();
	void CountUpdate();
	void DamageUpdate();
};


#endif // !EVENT_MASH_H

