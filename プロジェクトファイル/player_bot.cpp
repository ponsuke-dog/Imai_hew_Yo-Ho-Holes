//	player_bot.cpp
//	
//
//	2025.02.14			‘åŽR—½‘¾˜N

#include "manager_animation.h"
#include "client.h"
#include "player_bot.h"

BotPlayer::BotPlayer(Ship* ship, Client* client, int id):Player(ship,id,new AnimationManager(id)),m_Client(client)
{
}

BotPlayer::~BotPlayer()
{
}

void BotPlayer::Update()
{
	PlayerData data = m_Client->GetAllPlayerData()[GetId()];

	if (data.playerid < 0 || data.teamid < 0) {
		return;
	}

	XMFLOAT3 oldpos = GetPosition();

	
	SetPosition(data.position);
	SetRotation(data.rotation);
	if (data.anim != GetAnimationManager()->GetAnimation()) {
		GetAnimationManager()->SetAnimation(data.anim);
	}

	if(GetAnimationManager()->GetAnimation()==ANIMATIONSTATE_RUN|| GetAnimationManager()->GetAnimation() == ANIMATIONSTATE_CARRY_WOOD|| GetAnimationManager()->GetAnimation() == ANIMATIONSTATE_CARRY_CANNON){
		XMFLOAT3 movedirection;

		movedirection.x = GetPosition().x - oldpos.x;
		movedirection.y = 0;
		movedirection.z = GetPosition().z - oldpos.z;

		float x = sqrtf(movedirection.x * movedirection.x + movedirection.z * movedirection.z);

		movedirection.x /= x;
		movedirection.z /= x;

		GetAnimationManager()->SetRunEffect(GetPosition(), movedirection);
	}

	GetAnimationManager()->Update();
}

void BotPlayer::Draw() const
{
	if (GetPosition().y > -4.0f) {
		GetAnimationManager()->Draw(GetPosition(), GetRotation());
	}
}
