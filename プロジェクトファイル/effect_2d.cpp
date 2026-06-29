//	effect_2d.h
//	
//
//	2024.12.22			‘åŽR—½‘¾˜N


#include "sprite.h"
#include "texture.h"
#include "effect_2d.h"

Effect2D::Effect2D(int texture, XMFLOAT2 pos, XMFLOAT2 size, int tx, int ty, int nextFrame, Effect2DType type, bool reverse)
	:Object2d(pos,size),m_Texture(texture),m_Tx(tx),m_Ty(ty),m_NextFrame(nextFrame), m_EffectType(type),m_ReverseFlag(reverse)
{
	m_Life = m_Tx * m_Ty;
	m_TexNumber = 0;
	m_Frame = 0;
	m_EndFlag = false;
}
 
Effect2D::~Effect2D()
{
}

void Effect2D::Update()
{
	switch (m_EffectType)
	{
	case EFFECTTYPE_NOMAL:
		if (!m_EndFlag) {
			if (m_Frame >= m_NextFrame) {
				m_TexNumber++;
				m_Frame = 0;
			}

			if (m_TexNumber >= m_Life) {
				m_TexNumber = 0;
				m_EndFlag = true;
			}

		}

		break;

	case EFFECTTYPE_LOOP:
		
		if (m_Frame >= m_NextFrame) {
			m_TexNumber++;
			m_Frame = 0;
		}

		if (m_TexNumber >= m_Life) {
			m_TexNumber = 0;
		}
		break;

	case EFFECTTYPE_STOP:

		if (m_Frame >= m_NextFrame) {
			m_TexNumber++;
			m_Frame = 0;
		}

		if (m_TexNumber >= m_Life) {
			m_TexNumber = m_Life - 1;
		}
		break;
	default:
		break;
	}
	++m_Frame;
}

void Effect2D::Draw() const
{
	if (!m_EndFlag) {
		SpriteDraw(m_Texture,
			GetPosition().x, GetPosition().y, GetSize().x, GetSize().y,
			(m_TexNumber % m_Tx) * (TextureGetWidth(m_Texture) / m_Tx), (m_TexNumber / m_Tx) * (TextureGetHeight(m_Texture) / m_Ty),
			TextureGetWidth(m_Texture) / m_Tx, TextureGetHeight(m_Texture) / m_Ty,
			0, m_ReverseFlag
		);
	}
}

void Effect2D::SetTexture(int texture)
{
	m_Texture = texture;
}

void Effect2D::SetEndFlag(bool endflag)
{
	m_EndFlag = endflag;
}
