/*===============================================================


		大砲関係のアクション [action_cannon.h]

										Author : shusuke katori
										Date   : 2024/12/11

==================================================================*/

#ifndef ACTION_CANNON_H

#define ACTION_CANNON_H


class Ship;
class Player;
class Cannon;

typedef enum
{

	CANNON_ACTION_STATE_NONE = -1,
	CANNON_ACTION_STATE_IDOL,
	CANNON_ACTION_STATE_DIRECTION_GAUGE,	//横方向の範囲指定
	CANNON_ACTION_STATE_SHOT_GAUGE,		//発射角度指定
	CANNON_ACTION_STATE_SHOT,				//砲弾の発射
	CANNON_ACTION_STATE_MAX,

}CannonActionState;


class CannonAction
{

private:

	Player* m_Player = nullptr;

	Cannon* m_Cannon = nullptr;			//発射元の大砲を保存

	CannonActionState m_State;			//アクションの状態管理

	float m_FrameCount;						//フレーム数
	float m_GaugeCount;					//ゲージの数値

	int m_EndPoint = -1;

public:

	CannonAction() = delete;
	CannonAction(Player* pPlayer);
	~CannonAction() {}

	//更新処理（後で変わるかも）
	void Update();
	void Draw();

	//Cannonの登録
	void UseCannonObject(Cannon* p_cannon);

private:
	//アップデート内で使う関数
	void DecideEndPoint();
	void GaugeAction();
	void ShotAction();
	void ActionReset();
};


#endif //ACTION_CANNON_H
