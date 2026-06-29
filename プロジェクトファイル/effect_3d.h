//ーーーーーーーーーーーーーーーーーーーー
// effect_3d.h
//ーーーーーーーーーーーーーーーーーーーー
#ifndef EFFECT_3D_H
#define EFFECT_3D_H

#include "object_3d.h"
#include "manager_effect.h"

typedef enum {
	EFFECT3DTYPE_NONE = -1,
	EFFECT3DTYPE_NORMAL,		//一回再生
	EFFECT3DTYPE_LOOP,		//ループ再生

	EFFECT3DTYPE_MAX,

}Effect3DType; //アニメーション再生のタイプ

class Effect3D : public Object3D
{
private:
	//int m_Texture = -1;
	int m_Life = 0;
	int m_Frame = 0;
	Effect3DType m_Effect3DType = EFFECT3DTYPE_NONE;
	EffectType m_EffectType = EFFECTTYPE_NONE;
public:
	Effect3D() = delete;
	/*Effect3D(int texture, XMFLOAT3 pos, XMFLOAT3 size , int life);*/
	Effect3D(EffectType texture, XMFLOAT3 pos, XMFLOAT3 size, int life, Effect3DType type);
	virtual ~Effect3D();

	virtual void Update() override;
	virtual void Draw() const override;

	int GetFrame();
	//int GetTexture();
	EffectType GetEffectType();
};

#endif // EFFECT_H