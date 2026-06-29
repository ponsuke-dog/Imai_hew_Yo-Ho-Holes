//********************************************************************************
//
// エフェクトマネージャー[manager_effect.h] 
// 
//															Author :Riugo Honda
//															Date   :2025/01/18
// -------------------------------------------------------------------------------
//															Last Edited:2025/01/18
//********************************************************************************

#ifndef _MANAGER_EFFECT_H_
#define _MANAGER_EFFECT_H_

class Effect3D;

enum EffectType
{
	EFFECTTYPE_NONE = -1,
	EFFECTTYPE_EXPLOSION,
	EFFECTTYPE_RUN,
	EFFECTTYPE_SHOTSMOKE,
	EFFECTTYPE_SHOCKWAVE,
	EFFECTTYPE_SPLASH,
	EFFECTTYPE_FIRE,
	EFFECTTYPE_REPAIR,
	EFFECTTYPE_EXPLOSION_MOJI,
	EFFECTTYPE_SPLASH_MOJI,
	EFFECTTYPE_BANG_MOJI,
	EFFECTTYPE_CLUNK_MOJI,

	EFFECTTYPE_MAX
};

typedef struct
{
	int Tx;
	int Ty;
	int Texture;
	int FrameMax;
}Effect_Info;

void EffectManagerInitialize();
void EffectManagerFinalize();
void EffectManagerUpdate();
void EffectManagerDraw();

void EffectManagerRegister(EffectType effecttype, XMFLOAT3 pos , XMFLOAT3 size);
void EffectManagerRegister(Effect3D* object);

void SetEffectProjection(XMMATRIX projection);
void SetEffectView(XMMATRIX view);

int GetEffectFrameMaxNum(EffectType effecttype);

#endif // !_MANAGER_EFFECT_H_
