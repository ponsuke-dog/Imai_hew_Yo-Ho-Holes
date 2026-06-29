/*===============================================================


		砲弾クラス [cannonball.h]

										Author : shusuke katori
										Date   : 2024/11/22

==================================================================*/

#ifndef CANNONBALL_H
#define CANNONBALL_H

#include "object_3d.h"

class Cannon;
class Ship;
class Player;

class CannonBall : public Object3D
{

private:

	
	XMFLOAT3 m_CannonBallEndpoint = XMFLOAT3(0.0, 0.0, 0.0);
	XMFLOAT3 m_CannonBallSpeed = XMFLOAT3 (0.0 ,0.0, 0.0);

	XMFLOAT3 m_ExplosionSize = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Player* m_pPlayer = nullptr;

public:

	CannonBall() = delete;
	CannonBall(XMFLOAT3 endposition, Cannon* pCannon, Ship* pShip , Player* p_player);
	~CannonBall();

	//処理関係
	
	void Update(void) override;
	void Draw(void) const override;

	//追加
	void CalculateSpeed();

	//セッター
	void SetCannonBallEndpoint(XMFLOAT3 endpoint);
	void SetCannonBallSpeed(XMFLOAT3 speed);

	//ゲッター
	XMFLOAT3 GetCannonBallEndpoint(void);
	XMFLOAT3 GetCannonBallSpeed(void);

};

#endif //CANNONBALL_H
