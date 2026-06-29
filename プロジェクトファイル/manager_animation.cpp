#include "directx.h"
#include "texture.h"
#include "manager_effect.h"

#include "manager_animation.h"

const int g_EffectCreateSpan = 15;

AnimationManager::AnimationManager(int id)
{
	m_AnimationState = ANIMATIONSTATE_IDLE;
	m_FrameCount = 0;
	m_AnimationNum = 0;
	m_EffectTimeCount = -1;
	m_IsCreateEffect = false;
	m_PlayerType = PLAYERTYPE_NONE;
	m_Direction = { 0.0f, 0.0f, 0.0f };

	if (id == PLAYERTYPE_TAKO)
		m_PlayerType = PLAYERTYPE_TAKO;
	if (id == PLAYERTYPE_KAJIKI)
		m_PlayerType = PLAYERTYPE_KAJIKI;
	if (id == PLAYERTYPE_OUMU)
		m_PlayerType = PLAYERTYPE_OUMU;
	if (id == PLAYERTYPE_CAT)
		m_PlayerType = PLAYERTYPE_CAT;
}

AnimationManager::~AnimationManager(void)
{
}

void AnimationManager::Update(void)
{
	switch (m_AnimationState)
	{
	case ANIMATIONSTATE_IDLE:
		if (m_AnimationNum >= 7)
		{
			m_AnimationNum = 0;
		}

		if (m_FrameCount % 4 == 0)
		{
			m_AnimationNum++;
		}

		break;

	case ANIMATIONSTATE_RUN:
		if (m_AnimationNum >= 7)
		{
			m_AnimationNum = 0;
		}

		if (m_FrameCount % 4 == 0)
		{
			m_AnimationNum++;
		}

		break;

	case ANIMATIONSTATE_CARRY_WOOD_IDLE:
		if (m_AnimationNum >= 7)
		{
			m_AnimationNum = 0;
		}

		if (m_FrameCount % 4 == 0)
		{
			m_AnimationNum++;
		}
		
		break;
	
	case ANIMATIONSTATE_CARRY_WOOD:
		if (m_AnimationNum >= 7)
		{
			m_AnimationNum = 0;
		}

		if (m_FrameCount % 4 == 0)
		{
			m_AnimationNum++;
		}

		break;

	case ANIMATIONSTATE_CARRY_CANNON_IDLE:
		if (m_AnimationNum >= 7)
		{
			m_AnimationNum = 0;
		}

		if (m_FrameCount % 4 == 0)
		{
			m_AnimationNum++;
		}
		break;

	case ANIMATIONSTATE_CARRY_CANNON:
		if (m_AnimationNum >= 7)
		{
			m_AnimationNum = 0;
		}

		if (m_FrameCount % 4 == 0)
		{
			m_AnimationNum++;
		}

		break;

	case ANIMATIONSTATE_HOLD:

		break;

	case ANIMATIONSTATE_SHOT:
		if (m_AnimationNum >= 7)
		{
			m_AnimationNum = 7;
		}

		if (m_FrameCount % 4 == 0 && m_AnimationNum < 7)
		{
			m_AnimationNum++;
		}

		break;
	case ANIMATIONSTATE_REPAIR:
		if (m_AnimationNum >= 7)
		{
			m_AnimationNum = 0;
		}

		if (m_FrameCount % 4 == 0 && m_AnimationNum < 7)
		{
			m_AnimationNum++;
		}
		break;

	case ANIMATIONSTATE_BLOWOFF:
		if (m_AnimationNum >= 7)
		{
			m_AnimationNum = 0;
		}

		if (m_FrameCount % 4 == 0 && m_AnimationNum < 7)
		{
			m_AnimationNum++;
		}
		break;

	default:
		break;
	}

	if (m_FrameCount > 32)
		m_FrameCount = 0;


	if (m_EffectTimeCount == g_EffectCreateSpan)
	{
		m_EffectTimeCount = -1;
		m_IsCreateEffect = false;
	}
	
	if (m_IsCreateEffect == true)
		m_EffectTimeCount++;

	m_FrameCount++;
}

void AnimationManager::Draw(XMFLOAT3 pos, XMFLOAT3 angle)
{
	switch (m_AnimationState)
	{
	case ANIMATIONSTATE_IDLE:
		ModelDraw(m_PlayerType, MODELPLAYER_IDLE, m_AnimationNum, pos, angle);

		break;

	case ANIMATIONSTATE_RUN:
		ModelDraw(m_PlayerType, MODELPLAYER_RUN, m_AnimationNum, pos, angle);

		break;

	case ANIMATIONSTATE_CARRY_WOOD_IDLE:
		ModelDraw(m_PlayerType, MODELPLAYER_IDLE_CARRYWOOD, m_AnimationNum, pos, angle);
		break;

	case ANIMATIONSTATE_CARRY_WOOD:
		ModelDraw(m_PlayerType, MODELPLAYER_CARRYWOOD, m_AnimationNum, pos, angle);

		break;

	case ANIMATIONSTATE_CARRY_CANNON_IDLE:
		ModelDraw(m_PlayerType, MODELPLAYER_IDLE_CARRYBALL, m_AnimationNum, pos, angle);

		break;

	case ANIMATIONSTATE_CARRY_CANNON:
		ModelDraw(m_PlayerType, MODELPLAYER_CARRYBALL, m_AnimationNum, pos, angle);

		break;

	case ANIMATIONSTATE_HOLD:
		ModelDraw(m_PlayerType, MODELPLAYER_HOLD, m_AnimationNum, pos, angle);
		break;

	case ANIMATIONSTATE_SHOT:
		ModelDraw(m_PlayerType, MODELPLAYER_SHOT, m_AnimationNum, pos, angle);

		break;

	case ANIMATIONSTATE_REPAIR:
		ModelDraw(m_PlayerType, MODELPLAYER_REPAIR, m_AnimationNum, pos, angle);

		break;

	case ANIMATIONSTATE_BLOWOFF:
		ModelDraw(m_PlayerType, MODELPLAYER_BLOWOFF, m_AnimationNum, pos, angle);

		break;

	default:
		break;
	}
}

void AnimationManager::SetAnimation(AnimationState state)
{
	m_AnimationState = state;
	m_FrameCount = 0;
	m_AnimationNum = 0;
}

AnimationState AnimationManager::GetAnimation(void)
{
	return m_AnimationState;
}

void AnimationManager::SetDirection(XMFLOAT3 direction)
{
	m_Direction = direction;
}

void AnimationManager::SetRunEffect(XMFLOAT3 position, XMFLOAT3 direction)
{
	if (m_IsCreateEffect)
		return;

	m_IsCreateEffect = true;	//生成クールタイム
	m_FrameCount = 0;			//タイマー起動

	float Length = 0.5f;
	XMFLOAT3 EffectPosition = position;
	
	EffectPosition.x += Length * -direction.x;
	EffectPosition.y += 0.5f;
	EffectPosition.z += Length * -direction.z;

	EffectManagerRegister(EFFECTTYPE_RUN, EffectPosition, { 1.0f, 1.0f, 1.0f });

}


