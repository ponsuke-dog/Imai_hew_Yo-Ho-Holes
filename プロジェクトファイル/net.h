//********************************************************************************
//
// 網のクラス[net.h] 
// 
//															Author : Riugo Honda
//															Date   :2024/11/22
// -------------------------------------------------------------------------------
//															Last Edited:2024/11/24
//********************************************************************************

#ifndef _NET_H_
#define _NET_H_

#include "object_3d.h"

class Gather;

class Player;
class Ship;

typedef enum
{
	NET_ACTION_NONE = -1,
	NET_ACTION_THROWING,
	NET_ACTION_GATHERING,
	NET_ACTION_RETRIEVING,
	NET_ACTION_MAX,
}NET_ACTION;

class Net :public Object3D
{
private:
	XMFLOAT3 m_StartPosition;//バリスタの前の位置
	XMFLOAT3 m_EndPosition;//ギャザーのエイムでネットを投げると決めた位置
	XMFLOAT3 m_ThrowingVelocity; //ネットを投げる速度
	XMFLOAT3 m_RetrievingVelocity; //ネットを回収する速度
	XMFLOAT3 m_ModelSize; //ネットのモデル描画サイズ（判定とは別である）

	XMFLOAT3 m_EffectSize;

	int ObjectCount;
	bool m_IsThrowingNet;//ネットを投げているかどうか
	int m_FrameCount;
	bool m_StopFrame;
	float m_Time;
	float m_EndTime;

	//ネットのステート管理
	NET_ACTION m_State;

	//ストレージに入れるためのカウント
	int m_countcannonball = 0;
	int m_countwood = 0;
	int m_countenhance = 0;

	//後で3つ配列でFlotsamを保存のメンバ変数追加

	Player* m_pPlayer = nullptr;

	Gather* m_pGather = nullptr;



public:
	Net() = delete;
	Net(XMFLOAT3 start, XMFLOAT3 end, XMFLOAT3 size, Player* p_player, Ship* p_ship, Gather* p_gather);
	~Net() {};

	void Update() override;
	void Draw() const override;

	XMFLOAT3 GetNetCurrentPosition();
	bool GetThrowingNet();

private:
	//追加関数（update内を見やすくするために）
	void ThrowingNet();		//ネットを投げる関数
	void RetrievingNet();	//ネットを引っ張る関数
	void Gathering();
	void ThorwNet();//ネットのスピードなどの決定
	void SetStorage();
};


#endif // !_NET_H
