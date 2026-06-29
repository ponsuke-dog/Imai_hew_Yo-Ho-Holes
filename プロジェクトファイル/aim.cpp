//********************************************************************************
//
// 網のクラス[net.h] 
// 
//															Author : Riugo Honda
//															Date   :2024/12/11
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/11
//********************************************************************************

#include "gather.h"
#include "net.h"
#include "ship.h"
#include "manager_model.h"
#include "enhance.h"
#include "aim.h"

#define AIM_SPEED (0.25f)
#define AIM_LIMIT_X (8.0f)

//サイズ変更やれ
Aim::Aim(XMFLOAT3 start, Gather* gather, Ship* p_ship):Object3D(p_ship, OBJECTTYPE_AIM,start) , m_pGather(gather)
{
	//4.5
	if (p_ship->GetTeamId() == 0)
	{
		SetPosition(XMFLOAT3(start.x - 4.5f, start.y + 1.0f, start.z));
	}
	else if (p_ship->GetTeamId() == 1)
	{
		SetPosition(XMFLOAT3(start.x + 4.5f, start.y + 1.0f, start.z));
	}

	//エイムのサイズ及びネットの当たり判定
	switch(GetShip()->GetEnhance()->GetGatherLevel())
	{
	case 0:
		SetSize(XMFLOAT3(2.0f, 1.0f, 2.0f));
		break;
	case 1:
		SetSize(XMFLOAT3(3.0f, 1.0f, 3.0f));
		break;
	case 2:
		SetSize(XMFLOAT3(4.0f, 1.0f, 4.0f));
		break;
	case 3:
		SetSize(XMFLOAT3(5.0f, 1.0f, 5.0f));
		break;
	case 4:
		SetSize(XMFLOAT3(6.0f, 1.0f, 6.0f));
		break;
	case 5:
		SetSize(XMFLOAT3(7.0f, 1.0f, 7.0f));//後で最大5.0fに帰る
		break;
	}
	//回転の初期化
	SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_StartPosition = start;
}
Aim::~Aim()
{
}
;

void Aim::Update()
{
	
}

void Aim::Draw() const
{
	ModelDraw(MODELNAME_AIM,GetPosition(),GetRotation(),GetSize());
}

void Aim::AimMove(XMFLOAT3 vector)//方向単位ベクトルを受け取る
{
	m_Velocity = vector;

	m_Velocity.x *= AIM_SPEED;
	m_Velocity.y *= 0.0f;
	m_Velocity.z *= AIM_SPEED;

	

	if (GetShip()->GetTeamId() == 0)
	{
		//エイムの奥行きの最大値チェック
		if ((GetPosition().x + m_Velocity.x) <= (m_pGather->GetPosition().x - 20.0f))
		{
			m_Velocity.x = 0;
		}
		//もしエイムがバリスタより船側なった場合
		if ((GetPosition().x + m_Velocity.x) >= (m_pGather->GetPosition().x - 4.0))
		{
			m_Velocity.x = 0;
		}
		//エイムの右方向の限度チェック
		if ((GetPosition().z + m_Velocity.z) >= (m_pGather->GetPosition().z + AIM_LIMIT_X))
		{
			m_Velocity.z = 0;
		}
		//エイムの右方向の限度チェック
		if ((GetPosition().z + m_Velocity.z) <= (m_pGather->GetPosition().z - AIM_LIMIT_X))
		{
			m_Velocity.z = 0;
		}
	}
	else if (GetShip()->GetTeamId() == 1)
	{
		//エイムの奥行きの最大値チェック
		if ((GetPosition().x + m_Velocity.x) >= (m_pGather->GetPosition().x + 20.0f))
		{
			m_Velocity.x = 0;
		}
		//もしエイムがバリスタより船側なった場合
		if ((GetPosition().x + m_Velocity.x) <= (m_pGather->GetPosition().x + 4.0f))
		{
			m_Velocity.x = 0;
		}
		//エイムの右方向の限度チェック
		if ((GetPosition().z + m_Velocity.z) >= (m_pGather->GetPosition().z + AIM_LIMIT_X))
		{
			m_Velocity.z = 0;
		}
		//エイムの右方向の限度チェック
		if ((GetPosition().z + m_Velocity.z) <= (m_pGather->GetPosition().z - AIM_LIMIT_X))
		{
			m_Velocity.z = 0;
		}
	}

	//移動
	SetPosition(XMFLOAT3(GetPosition().x + m_Velocity.x, GetPosition().y + m_Velocity.y, GetPosition().z + m_Velocity.z));//SetPosition(XMFLOAT3(GetPosition().x + m_Velocity.x * dt, GetPosition().y + m_Velocity.y * dt, GetPosition().z + m_Velocity.z * dt));
}
