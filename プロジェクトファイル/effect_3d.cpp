//ーーーーーーーーーーーーーーーーーーーー
// effect_3d.cpp
//ーーーーーーーーーーーーーーーーーーーー
#include "directx.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "object_3d.h"
#include "effect_3d.h"

//Effect3D::Effect3D(int texture, XMFLOAT3 pos, XMFLOAT3 size, int life) : Object3D(pos, size, OBJECTTYPE_EFFECT)
//{
//	m_Texture = texture;
//
//	m_Life = life;
//	m_Frame = 0;
//
//
//}

Effect3D::Effect3D(EffectType texture, XMFLOAT3 pos, XMFLOAT3 size, int life, Effect3DType type) : Object3D(pos, size, OBJECTTYPE_EFFECT)
{
	//m_Texture = texture;

	m_Life = life;
	m_Frame = 0;

	m_Effect3DType = type;
	m_EffectType = texture;

}

Effect3D::~Effect3D()
{
	
}

void Effect3D::Update()
{
	switch (m_Effect3DType)
	{
	case EFFECT3DTYPE_NORMAL:
		if (!GetDestroy()) {
			if (m_Frame >= m_Life) {
				SetDestroyFlag(true);
			}

		}
		break;

	case EFFECT3DTYPE_LOOP:

		if (m_Frame >= m_Life) {
			m_Frame = 0;
		}

		break;

	default:
		break;
	}
	++m_Frame;
	/*if (m_Frame >= m_Life)
	{
		SetDestroyFlag(true);
	}

	++m_Frame;*/
}

void Effect3D::Draw()const
{
	
}

int Effect3D::GetFrame()
{
	return m_Frame;
}

EffectType Effect3D::GetEffectType()
{
	return m_EffectType;
}

//int Effect3D::GetTexture()
//{
//	return m_Texture;
//}

