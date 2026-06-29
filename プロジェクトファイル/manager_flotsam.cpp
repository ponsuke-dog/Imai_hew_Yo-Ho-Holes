//	manager_flotsam.h
//	
//
//	2024.12.20			PON

#include <random>
#include <math.h>
#include "ship.h"
#include "manager_flotsam.h"

#define FLOTSAM_START_POS_DEPTHS (32.0f)

FlotsamManager::FlotsamManager(Ship* ship):m_pShip(ship),m_Count(0)
{
}

FlotsamManager::~FlotsamManager()
{
}

void FlotsamManager::Update()
{
	// 流す間隔　60フレーム * 秒数
	if (m_Count % ( 45 ) == 0)
	{
		CreateFlotsam();
	}

	// カウントのリセット
	if (m_Count >= 600)
	{
		m_Count = 1;
	}
	// カウントの増加
	m_Count++;
}

void FlotsamManager::CreateFlotsam()
{
	// 完全乱数
	std::random_device rd;

	// x座標の乱数取得							// * 流す幅
	float RandPosx = sinf(3.14159f * (float)(rd())) * 6.5f;

	// タイプの乱数取得
	int RandType = rd() % 10;

	// センターポジション
	XMFLOAT3 StartPosition = m_pShip->GetCenterPosition();

	// チーム判別
	if (m_pShip->GetTeamId() == 0)
	{
		//StartPosition.x *= -1;
		StartPosition.x += -21;
	}
	else if(m_pShip->GetTeamId() == 1)
	{
		//StartPosition.x *= +1;
		StartPosition.x += 21;
	}


	// 新しい漂流物の生成
	if (RandType < 4)
	{
		// 木材
		m_pShip->RegisterFlotsams(new Flotsam(m_pShip,XMFLOAT3((RandPosx + StartPosition.x ), StartPosition.y, StartPosition.z + FLOTSAM_START_POS_DEPTHS), FLOTSAM_TYPE_WOOD));
		// return;
	}
	else if (RandType < 8)
	{
		// 大砲
		m_pShip->RegisterFlotsams(new Flotsam(m_pShip, XMFLOAT3((RandPosx + StartPosition.x ), StartPosition.y, StartPosition.z + FLOTSAM_START_POS_DEPTHS), FLOTSAM_TYPE_CANNON));
		// return;
	}
	else
	{
		// 強化
		m_pShip->RegisterFlotsams(new Flotsam(m_pShip, XMFLOAT3((RandPosx + StartPosition.x ), StartPosition.y, StartPosition.z + FLOTSAM_START_POS_DEPTHS), FLOTSAM_TYPE_ENHANCE));
		// return;
	}

}
