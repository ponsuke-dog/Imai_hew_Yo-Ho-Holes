/*===============================================================


		爆発処理 [explosion.h]

										Author : shusuke katori
										Date   : 2024/12/1

==================================================================*/

#ifndef EXPLOSION_H

#define EXPLOSION_H

#include "object_3d.h"

class Ship;

class Player;

class Explosion : public Object3D
{

private:

	float m_Radius = 0.0f;		//爆発半径
	int m_FrameCount = 0;			//フレーム数
	int m_EndCount = 0;				//終了地点のフレーム数
	float m_damage = 0.0f;
	

	//追加
	Player* m_pPlayer = nullptr;

public:

	Explosion() = delete;
	Explosion(XMFLOAT3 position, Ship* pShip , Player* p_player);
	~Explosion();


	void Update() override;
	void Draw() const override;

};




#endif //EXPLOSION_H
