/*===============================================================


		砲弾クラス [cannonball.cpp]

										Author : shusuke katori
										Date   : 2024/11/22

==================================================================*/

#include "manager_model.h"
#include "cannon.h"
#include "ship.h"
#include "explosion.h"
#include "manager_effect.h"
#include "player.h"
#include "camera_player.h"
#include "cri.h"
#include "controller.h"
#include "enhance.h"
#include "mode.h"
#include "cannonball.h"

//定数--------------------------------
#define GRAVITY_SCALE (-9.8f)
#define AIR_TIME (2.0f) 

//----------------------------------------------


CannonBall::CannonBall(XMFLOAT3 endposition, Cannon* pCannon, Ship* pShip , Player* p_player) : Object3D(pShip, OBJECTTYPE_CANNONBALL, pCannon->GetPosition())
, m_CannonBallEndpoint(endposition) , m_pPlayer(p_player)
{

	switch (GetShip()->GetTeamId())
	{

	case 0:
		//砲弾の射出位置微調整する
		SetPosition(XMFLOAT3(GetPosition().x + 1.0f, GetPosition().y + 1.0f, GetPosition().z));
		EffectManagerRegister(EFFECTTYPE_SHOTSMOKE, { GetPosition().x + 2.0f, GetPosition().y + 2.0f, GetPosition().z }, XMFLOAT3{ 2.0f, 2.0f, 2.0f });
		EffectManagerRegister(EFFECTTYPE_SHOCKWAVE, { GetPosition().x + 2.0f, GetPosition().y + 1.5f, GetPosition().z }, XMFLOAT3{ 2.5f, 2.5f, 2.5f });
		EffectManagerRegister(EFFECTTYPE_BANG_MOJI, { GetPosition().x + 2.0f, GetPosition().y + 2.5f, GetPosition().z - 1.0f }, XMFLOAT3{ 2.5f, 2.5f, 2.5f });

		break;

	case 1:
		//砲弾の射出位置微調整する
		SetPosition(XMFLOAT3(GetPosition().x - 1.0f, GetPosition().y + 1.0f, GetPosition().z));
		EffectManagerRegister(EFFECTTYPE_SHOTSMOKE, { GetPosition().x - 2.0f, GetPosition().y + 2.0f, GetPosition().z }, XMFLOAT3{ 2.0f, 2.0f, 2.0f });
		EffectManagerRegister(EFFECTTYPE_SHOCKWAVE, { GetPosition().x - 2.0f, GetPosition().y + 1.5f, GetPosition().z }, XMFLOAT3{ 2.5f, 2.5f, 2.5f });
		EffectManagerRegister(EFFECTTYPE_BANG_MOJI, { GetPosition().x - 2.0f, GetPosition().y + 2.5f, GetPosition().z - 1.0f }, XMFLOAT3{ 2.5f, 2.5f, 2.5f });

		break;

	default:
		break;
	}

	switch (GetShip()->GetEnhance()->GetCannonLevel()) {
	case 0://*1
		m_ExplosionSize = XMFLOAT3(4.0f, 4.0f, 4.0f);
		break;

	case 1://+
		m_ExplosionSize = XMFLOAT3(5.0f, 5.0f, 5.0f);
		break;

	case 2://+
		m_ExplosionSize = XMFLOAT3(6.5f, 6.5f, 6.5f);
		break;

	case 3://*3
		m_ExplosionSize = XMFLOAT3(7.5f, 7.5f, 7.5f);
		break;

	case 4://*3
		m_ExplosionSize = XMFLOAT3(8.5f, 8.5f, 8.5f);
		break;

	case 5://*3,+
		m_ExplosionSize = XMFLOAT3(10.0f, 10.0f, 10.0f);
		break;

	default:
		break;

	}

	CalculateSpeed();

}



CannonBall::~CannonBall()
{
}

//処理関係
void CannonBall::Update(void)
{

	float dt = 1.0f / 60.0f;

	//重力
	m_CannonBallSpeed.y += GRAVITY_SCALE * dt;


	SetPosition(XMFLOAT3(GetPosition().x + (m_CannonBallSpeed.x * dt) , GetPosition().y + (m_CannonBallSpeed.y * dt), GetPosition().z + (m_CannonBallSpeed.z * dt)));

	
	//着弾点に到達したときの処理↓(ここでexplosionの生成)        制作途中
	if (GetPosition().y < m_CannonBallEndpoint.y)
	{
		GetShip()->RegisterExplosions(new Explosion(m_CannonBallEndpoint, GetShip(), m_pPlayer));
		//GetShip()->RegisterEffect3D(new EffectExplostion3D(m_CannonBallEndpoint, XMFLOAT3{ 3.0f, 3.0f, 3.0f }));//(これが原因で映らない理由はわからん
		//仮
		EffectManagerRegister(EFFECTTYPE_EXPLOSION, { m_CannonBallEndpoint.x, m_CannonBallEndpoint.y + 2.0f, m_CannonBallEndpoint.z }, m_ExplosionSize);
		EffectManagerRegister(EFFECTTYPE_EXPLOSION_MOJI, { m_CannonBallEndpoint.x, m_CannonBallEndpoint.y + 3.0f, m_CannonBallEndpoint.z -1.0f }, { m_ExplosionSize .x*0.5f,m_ExplosionSize .y*0.5f,m_ExplosionSize .z*0.5f});

		if (GetGameMode() != GAMEMODE_ONLINE) {
			for (auto itr : GetShip()->GetEnemyShip()->GetPlayers())
			{
				itr->GetCamera()->SetCameraShake(1.0f);

				itr->GetController()->Controller_SetVibration(8000, 8000, 2000);
			}
		}
		else {
			
		}

		if (GetGameMode() == GAMEMODE_ONLINE)
		{
			CreateSource(GetApp3dSE(), GetPosition(), CRI_SE3D_爆発, 1.5f);
		}
		else
		{

			for (int i = 0; i < 4; i++)
			{
				CreateSource(GetApp3dSE(i), GetPosition(), CRI_SE3D_爆発, 1.5f);
			}
		}

		SetDestroyFlag(true);

		
	}
	

}


void CannonBall::Draw(void) const
{
	ModelDraw(MODELNAME_CANNONBALL , GetPosition());
}

void CannonBall::CalculateSpeed()
{

	m_CannonBallSpeed.x = m_CannonBallEndpoint.x - GetPosition().x;
	m_CannonBallSpeed.y = m_CannonBallEndpoint.y - GetPosition().y;
	m_CannonBallSpeed.z = m_CannonBallEndpoint.z - GetPosition().z;


	//xz軸の移動時間

	m_CannonBallSpeed.x = m_CannonBallSpeed.x / AIR_TIME;
	m_CannonBallSpeed.z = m_CannonBallSpeed.z / AIR_TIME;


	//Y軸のお移動
	//Y方向の最高点
	//※Y方向の傾きが考慮されてないのでxz軸の移動時間とのズレあり
	if (m_CannonBallSpeed.y > 0)
	{
		m_CannonBallSpeed.y = (m_CannonBallSpeed.y / AIR_TIME) + (0.5f * GRAVITY_SCALE * AIR_TIME);//ありえないけど一応の場合分け
	}
	else
	{
		m_CannonBallSpeed.y = (m_CannonBallSpeed.y / AIR_TIME) - (0.5f * GRAVITY_SCALE * AIR_TIME);
	}
	

}


//セッター

void CannonBall::SetCannonBallEndpoint(XMFLOAT3 endpoint)
{
	m_CannonBallEndpoint = endpoint;
}

void CannonBall::SetCannonBallSpeed(XMFLOAT3 speed)
{
	m_CannonBallSpeed = speed;
}


//ゲッター

XMFLOAT3 CannonBall::GetCannonBallEndpoint(void)
{
	return m_CannonBallEndpoint;
}

XMFLOAT3 CannonBall::GetCannonBallSpeed(void)
{
	return m_CannonBallSpeed;
}



