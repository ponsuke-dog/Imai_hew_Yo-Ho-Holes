//	effect_2d.h
//	
//
//	2024.12.22			‘åŽR—½‘¾˜N

#ifndef EFFECT_2D_H
#define EFFECT_2D_H

#include  "object_2d.h"

typedef enum {
	EFFECTTYPE_NONE = -1,
	EFFECTTYPE_NOMAL,
	EFFECTTYPE_LOOP,
	EFFECTTYPE_STOP,

	EFFECTTYPE_MAX,

}Effect2DType;

class Effect2D :public Object2d
{
private:
	int m_Texture = -1;
	int m_Tx = 1;
	int m_Ty = 1;
	int m_Life = 0;
	int m_NextFrame = 0;
	int m_TexNumber = 0;
	int m_Frame = 0;
	bool m_EndFlag = false;
	bool m_ReverseFlag = false;
	Effect2DType m_EffectType = EFFECTTYPE_NONE;

public:
	Effect2D() = delete;
	Effect2D(int texture, XMFLOAT2 pos, XMFLOAT2 size, int tx, int ty,int nextFrame, Effect2DType type,bool reverse = false);
	virtual ~Effect2D();

	virtual void Update() override;
	virtual void Draw() const override;

	void SetTexture(int texture);
	void SetEndFlag(bool endflag);
};

#endif // EFFECT_2D_H
