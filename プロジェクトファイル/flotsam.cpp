//	flotsam.cpp
//	
//
//	2024.12.20			PON

#include "manager_model.h"
#include "flotsam.h"


Flotsam::Flotsam(Ship* pship, XMFLOAT3 pos, FlotsamType type):Object3D(pship,OBJECTTYPE_FLOTSAM,pos)
{
	// 初期化
	m_Velocity = XMFLOAT3(0.0f, 0.0f, 2.5f);
	m_Type = type;
}


Flotsam::~Flotsam()
{
}

void Flotsam::Update()
{
	// 移動処理
	float dt = 1.0f / 60.0f;
	SetPosition(XMFLOAT3(GetPosition().x, GetPosition().y, GetPosition().z - m_Velocity.z * dt));

	// deleteするポジション
	if (GetPosition().z <= -30.0f)
	{
		SetDestroyFlag(true);
	}
}

void Flotsam::Draw()const
{
	

	//モデルの描画
	switch (m_Type)
	{
	case FLOTSAM_TYPE_WOOD:		// 木材
		ModelDraw(MODELNAME_FLOTSAM_WOOD, GetPosition());
		break;

	case FLOTSAM_TYPE_CANNON:	// 大砲
		ModelDraw(MODELNAME_FLOTSAM_BALL, GetPosition());
		break;

	case FLOTSAM_TYPE_ENHANCE:	// 強化
		ModelDraw(MODELNAME_FLOTSAM_ENHANCE, GetPosition());
		break;

	default:
		break;
	}
}


XMFLOAT3 Flotsam::GetVel()
{
	return m_Velocity;
}

FlotsamType Flotsam::GetFlotsamType()
{
	return m_Type;
}

void Flotsam::SetVel(XMFLOAT3 vel)
{
	m_Velocity = vel;
}

