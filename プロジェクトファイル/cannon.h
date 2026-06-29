/*===============================================================


		大砲クラス [cannon.h]

										Author : shusuke katori
										Date   : 2024/11/19

==================================================================*/

#ifndef CANNON_H
#define CANNON_H

#include "object_3d.h"

class Player;
class BillboardObject;

class Cannon : public Object3D
{
private:

	//アクション中フラグ
	bool m_ActionFlag = true;

	//弾の着弾点の記憶
	XMFLOAT3 m_Endpoint;
	
	int m_Id = -1;

	BillboardObject* m_BillboardObject = nullptr;

public:
	Cannon() = delete;
	Cannon(XMFLOAT3 pos, Ship* p_ship , int id);
	~Cannon();


	//大砲の処理関係
	void Update(void) override;
	void Draw(void)const override;


	//砲弾の生成削除
	void CreateCannonBall(int id , Player* p_player = nullptr);

	//セッター
	void SetCannonActionFlag(bool flag);

	//ゲッター
	bool GetCannonActionFlag(void);
	
	int GetId();
	void SetId(int id);

	void Reset();
};

#endif //CANNON_H
