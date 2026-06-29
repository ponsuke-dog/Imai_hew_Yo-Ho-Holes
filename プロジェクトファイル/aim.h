//********************************************************************************
//
// 網のクラス[net.h] 
// 
//															Author : Riugo Honda
//															Date   :2024/12/11
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/11
//********************************************************************************

#ifndef _AIM_H_
#define _AIM_H_

#include "object_3d.h"

class Gather;
class Net;
class Player;

class Aim :public Object3D
{
private:
	XMFLOAT3 m_StartPosition; //エイムの初期位置（ネット用）
	XMFLOAT3 m_Velocity; //エイムの速度
	Gather* m_pGather = nullptr;
public:
	Aim() = delete;
	Aim(XMFLOAT3 start,  Gather* gather,Ship* p_ship);

	~Aim();

	void Update() override;
	void Draw() const override;

	void AimMove(XMFLOAT3 vector);


};


#endif // !_AIM_H

