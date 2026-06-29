//********************************************************************************
//
// 収集のクラス[gather.h] 
// 
//															Author : Riugo Honda
//															Date   :2024/11/22
// -------------------------------------------------------------------------------
//															Last Edited:2024/11/24
//********************************************************************************

#ifndef _GATHER_H_
#define _GATHER_H_

#include "object_3d.h"

class Player;
class Ship;
class BillboardObject;

class Gather: public Object3D
{
private:
	int m_id = -1;
	
	bool m_IsUsingGather = false;	//ギャザー使用フラグ
	bool m_CoolTimeFlag = false;

	int m_CoolTime;
	int m_CoolTimeMax = 600;

	BillboardObject* m_BillboardObject = nullptr;

public:
	Gather() = delete;
	Gather(XMFLOAT3 startpos, Ship* p_ship,int id);

	~Gather();

	void Update() override;		//アップデート
	void Draw() const override;	//ドロー
	
	//gather使用フラグ
	void SetUsingGather(bool buse);
	bool GetUsingGather();

	//idゲッター
	int GetId();

	//クールタイムのセットフラグ
	void SetCoolTimeFlag(bool buse);

	void SetNet(XMFLOAT3 endposition , XMFLOAT3 size , Player* p_player);

	//レベルによってクールタイムの変更可能
	void SetCoolTimeMax(int level);

	void Reset();
};

#endif // !_GATHER_H