/*==============================================================================

　アニメーションマネージャー　[manager_animation.h]
														 Author : Souta Fukamizu
														 Date   : 2025/01/07
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef MANAGER_ANIMATION
#define MANAGER_ANIMATION

#include "directx.h"
#include "manager_model.h"
#include "packet.h"

class AnimationManager
{
private:
	AnimationState m_AnimationState;
	int m_FrameCount;
	int m_AnimationNum;
	PlayerType m_PlayerType = PLAYERTYPE_NONE;
	XMFLOAT3 m_Direction;			//プレイヤーの向いている方向を保存 
	int m_EffectTimeCount;
	bool m_IsCreateEffect;

public:
	AnimationManager(int id);
	~AnimationManager(void);

	void Update(void);
	void Draw(XMFLOAT3 pos, XMFLOAT3 angle);

	void SetAnimation(AnimationState state);
	AnimationState GetAnimation(void);

	void SetDirection(XMFLOAT3 direction);
	void SetRunEffect(XMFLOAT3 position, XMFLOAT3 direction);
};

#endif // !MANAGER_ANIMATION

