//********************************************************************************
//
// ネットのクラス[net.h]
//
//															Author : Riugo Honda
//															Date   :2024/12/03
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/15
//********************************************************************************
#include "player.h"
#include <list>
#include "mode.h"
#include "cri.h"
#include "collision.h"
#include "flotsam.h"
#include "ship.h"
#include "manager_model.h"
#include "storage.h"
#include "enhance.h"
#include "gather.h"
#include "camera_player.h"
#include "manager_effect.h"
#include "easing.h"
#include "client.h"
#include "mode.h"
#include "net.h"

#define NET_SPEED (2.38f)
#define NET_DURATION (60)
#define NET_GATHERING_TIME (20)
static float angle = 0.0f;

Net::Net(XMFLOAT3 start, XMFLOAT3 end, XMFLOAT3 size, Player* p_player, Ship* p_ship, Gather* p_gather)
	:m_StartPosition(start), m_EndPosition(end.x, end.y - 1.0f, end.z), Object3D(p_ship, OBJECTTYPE_NET, start, size), m_pPlayer(p_player), m_pGather(p_gather)
{//エンドポジションを下げないと当たり判定が当たらなくなる
	if (p_ship->GetTeamId() == 0)
	{
		SetPosition(XMFLOAT3(start.x - 4.5f, start.y, start.z));

		m_StartPosition.x -= 4.5f;
	}
	else if (p_ship->GetTeamId() == 1)
	{
		SetPosition(XMFLOAT3(start.x + 4.5f, start.y, start.z));

		m_StartPosition.x += 4.5f;
	}


	//回転の初期化
	SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));

	//ネットのサイズ設定
	switch (GetShip()->GetEnhance()->GetGatherLevel())
	{
	case 0:
		m_ModelSize = XMFLOAT3(4.0f, 4.0f, 4.0f);
		m_EffectSize = XMFLOAT3(2.0f, 2.0f, 2.0f);
		break;
	case 1:
		m_ModelSize = XMFLOAT3(6.0f, 6.0f, 6.0f);
		m_EffectSize = XMFLOAT3(2.5f, 2.5f, 2.5f);
		break;
	case 2:
		m_ModelSize = XMFLOAT3(8.0f, 8.0f, 8.0f);
		m_EffectSize = XMFLOAT3(3.0f, 3.0f, 3.0f);
		break;
	case 3:
		m_ModelSize = XMFLOAT3(10.0f, 10.0f, 10.0f);
		m_EffectSize = XMFLOAT3(3.5f, 3.5f, 3.5f);
		break;
	case 4:
		m_ModelSize = XMFLOAT3(12.0f, 12.0f, 12.0f);
		m_EffectSize = XMFLOAT3(4.0f, 4.0f, 4.0f);
		break;
	case 5:
		m_ModelSize = XMFLOAT3(14.0f, 14.0f, 14.0f);//バランス調整
		m_EffectSize = XMFLOAT3(5.0f, 5.0f, 5.0f);
		break;
	default:
		break;
	}
	m_FrameCount = 0;

	m_Time = 0.0f;
	m_EndTime = 0.0f;

	m_State = NET_ACTION_THROWING;

	angle = 0.0f;

	//射出位置をバリスタに調整
	m_StartPosition.y = 2.5f;

	SetPosition(XMFLOAT3((GetPosition().x), (m_StartPosition.y), (GetPosition().z)));

	ThorwNet();
};



void Net::Update()
{
	switch (m_State)
	{
	case NET_ACTION_THROWING:
		ThrowingNet();
		break;
	case NET_ACTION_GATHERING:
		Gathering();
		break;
	case NET_ACTION_RETRIEVING:
		RetrievingNet();
		break;
	}
}

void Net::Draw() const
{
	switch (m_State)
	{
	case NET_ACTION_THROWING:
		ModelDraw(MODELNAME_NET, GetPosition(), GetRotation(), m_ModelSize);
		break;
	case NET_ACTION_GATHERING:
		ModelDraw(MODELNAME_NET, GetPosition(), { 0.0f,angle ,0.0f }, m_ModelSize);
		break;
	case NET_ACTION_RETRIEVING:
		ModelDraw(MODELNAME_NET_RISE, GetPosition(), { 0.0f,0.0f ,0.0f }, m_ModelSize);
		break;
	}
	//ModelDraw(MODELNAME_NET, GetPosition(), { 0.0f,angle ,0.0f }, m_ModelSize);
	angle += 0.1f;
}

XMFLOAT3 Net::GetNetCurrentPosition()
{
	return GetPosition();
}

bool Net::GetThrowingNet()
{
	return m_IsThrowingNet;
}

void Net::ThrowingNet()
{
	XMFLOAT3 Old_position = GetPosition();

	float interpolation = m_Time / m_EndTime;
	if (interpolation > 1.0f)
	{
		interpolation = 1.0f;
	}

	XMFLOAT3 rate;

	rate.x = easeOutQuad(interpolation);
	if (rate.x > 1.0)
	{
		rate.x = 1.0;
	}
	rate.y = easeOutQuad(interpolation);
	if (rate.y > 1.0)
	{
		rate.y = 1.0;
	}
	rate.z = easeOutQuad(interpolation);
	if (rate.z > 1.0)
	{
		rate.z = 1.0;
	}

	XMFLOAT3 setting_position = GetPosition();

	setting_position.x = Old_position.x * (1.0f - interpolation) + m_EndPosition.x * interpolation;//Old_position.x * (1.0f - interpolation) + m_EndPosition.x * interpolation //Old_position.x * (1.0f - rate.x) + m_EndPosition.x * rate.x
	setting_position.y = Old_position.y * (1.0f - interpolation) + m_EndPosition.y * interpolation;//Old_position.y * (1.0f - interpolation) + m_EndPosition.y * interpolation //Old_position.y * (1.0f - rate.y) + m_EndPosition.y * rate.y
	setting_position.z = Old_position.z * (1.0f - interpolation) + m_EndPosition.z * interpolation;//Old_position.z * (1.0f - interpolation) + m_EndPosition.z * interpolation //Old_position.z * (1.0f - rate.z) + m_EndPosition.z * rate.z

	if ((setting_position.x - Old_position.x <= 0.01 && setting_position.x - Old_position.x > 0) || (setting_position.x - Old_position.x >= -0.01 && setting_position.x - Old_position.x < 0))
	{
		setting_position = m_EndPosition;
	}

	SetPosition(setting_position);

	m_Time += 1.0f / 60.0f;

	if (/*m_Time >= m_EndTime */ interpolation >= 1.0f || (m_EndPosition.x == GetPosition().x  && m_EndPosition.z == GetPosition().z))
	{
		m_State = NET_ACTION_GATHERING;

		SetPosition(m_EndPosition);

		m_StopFrame = true;

		if (GetGameMode() == GAMEMODE_ONLINE)
		{
			CreateSource(GetApp3dSE(), m_EndPosition, CRI_SE3D_ネット着水);
		}
		else
		{

			for (int i = 0; i < 4; i++)
			{
				CreateSource(GetApp3dSE(i), m_EndPosition, CRI_SE3D_ネット着水, 2.0f);
			}
		}

		EffectManagerRegister(EFFECTTYPE_SPLASH, { m_EndPosition.x, m_EndPosition.y + 2.0f, m_EndPosition.z },m_EffectSize);
		if (GetShip()->GetTeamId() == 0) {
			EffectManagerRegister(EFFECTTYPE_SPLASH_MOJI, { m_EndPosition.x + (m_EffectSize.x * 0.5f), m_EndPosition.y + 2.0f, m_EndPosition.z}, {m_EffectSize.x * 2.0f,m_EffectSize.y * 2.0f,m_EffectSize.z * 2.0f});
		}
		else {
			EffectManagerRegister(EFFECTTYPE_SPLASH_MOJI, { m_EndPosition.x - (m_EffectSize.x / 2.0f) , m_EndPosition.y + 2.0f, m_EndPosition.z}, { m_EffectSize.x * 2.0f,m_EffectSize.y * 2.0f,m_EffectSize.z * 2.0f });
		}
	}
}

void Net::RetrievingNet()
{
	XMFLOAT3 setting_position = XMFLOAT3((GetPosition().x + m_RetrievingVelocity.x), (GetPosition().y + m_RetrievingVelocity.y), (GetPosition().z + m_RetrievingVelocity.z));

	//ネットのX座標が目的地を超えてないかのチェック
	if (setting_position.x > m_StartPosition.x && m_RetrievingVelocity.x > 0)
	{
		setting_position.x = m_StartPosition.x;

		m_RetrievingVelocity.x = 0;
	}

	if (setting_position.x < m_StartPosition.x && m_RetrievingVelocity.x < 0)
	{
		setting_position.x = m_StartPosition.x;

		m_RetrievingVelocity.x = 0;
	}

	//ネットのZ座標が目的地を超えてないかのチェック
	if (setting_position.z > m_StartPosition.z && m_RetrievingVelocity.z > 0)
	{
		setting_position.z = m_StartPosition.z;

		m_RetrievingVelocity.z = 0;
	}

	if (setting_position.z < m_StartPosition.z && m_RetrievingVelocity.z < 0)
	{
		setting_position.z = m_StartPosition.z;

		m_RetrievingVelocity.z = 0;
	}

	//ネットのY座標が目的地を超えてないかのチェック
	if (setting_position.y < m_StartPosition.y)
	{
		setting_position.y = m_StartPosition.y;

		m_RetrievingVelocity.y = 0;
	}

	SetPosition({GetPosition().x , GetPosition().y + 1.0f , GetPosition().z});

	if (GetPosition().y >= 5.0f)
	{
		m_pPlayer->GetCamera()->SetCameraState(CAMERA_STATE_RESET);
	}


	if (GetPosition().y >= 10.0f)
	{
		SetDestroyFlag(true);
	}

}

void Net::Gathering()
{
	

	if (m_FrameCount >= NET_GATHERING_TIME)//着弾してからの待機時間
	{
		m_State = NET_ACTION_RETRIEVING;

		m_FrameCount = 0;

		SetStorage();

		m_pGather->SetCoolTimeFlag(true);

		if (GetGameMode() == GAMEMODE_ONLINE) {
			GatherObject data;
			data.teamid = m_pGather->GetShip()->GetTeamId();
			data.gatherid = m_pGather->GetId();
			data.use = USEFLAG_END;
			GetClient()->SendGatherObjectData(data);
		}

		/*m_pPlayer->GetCamera()->SetCameraState(CAMERA_STATE_RESET);*/


		return;
	}


	std::list<Flotsam*>p_flotsams = GetShip()->GetFlotsams();

	for (auto itr = p_flotsams.begin(); itr != p_flotsams.end(); itr++)
	{
		if (CheckBoxCollider(XMFLOAT2(GetPosition().x, GetPosition().z), XMFLOAT2((*itr)->GetPosition().x, (*itr)->GetPosition().z), XMFLOAT2(GetSize().x, GetSize().z), XMFLOAT2((*itr)->GetSize().x, (*itr)->GetSize().z)))
		{
			//後でここに＋＋追加
			switch ((*itr)->GetFlotsamType())
			{
			case FLOTSAM_TYPE_WOOD:
				m_countwood++;
				break;
			case FLOTSAM_TYPE_CANNON:
				m_countcannonball++;
				break;
			case FLOTSAM_TYPE_ENHANCE:
				m_countenhance += 5;
				break;
			default:
				break;
			}

			(*itr)->SetDestroyFlag(true);



		}
	}


	m_FrameCount++;	
}

void Net::ThorwNet()//ネットのスピードなどの決定
{
	

	XMFLOAT3 distance = XMFLOAT3(((m_EndPosition.x - m_StartPosition.x)), ((m_EndPosition.y - m_StartPosition.y)), ((m_EndPosition.z - m_StartPosition.z)));

	float length = sqrtf((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z));

	m_EndTime = (NET_DURATION * length) / 17.5f;

	if (m_EndTime >= NET_DURATION)
	{
		m_EndTime = NET_DURATION;
	}

	m_EndTime /= 60.0f;

	m_IsThrowingNet = true;

}


void Net::SetStorage()
{
	if (GetGameMode() != GAMEMODE_ONLINE) {
		auto storages = GetShip()->GetStorages();

		for (auto check : storages)
		{
			switch (check->GetType())
			{
			case STORAGETYPE_REPAIR:
				check->Increase(m_countwood);

				m_countwood = 0;
				break;
			case STORAGETYPE_CANNON:
				check->Increase(m_countcannonball);

				m_countcannonball = 0;
				break;
			default:
				break;
			}
		}

		if (m_countenhance > 0)
		{
			GetShip()->GetEnhance()->AddStock(m_countenhance);

			m_countenhance = 0;
		}
	}
	else {
		AddStockData data;
		data.teamid = GetShip()->GetTeamId();
		data.add_stock[STOCKTYPE_WOOD] = m_countwood;
		data.add_stock[STOCKTYPE_CANNON] = m_countcannonball;
		data.add_stock[STOCKTYPE_ENHANCE] = m_countenhance;
		
		GetClient()->SendAddStockObjectData(data);

		m_countwood = 0;
		m_countcannonball = 0;
		m_countenhance = 0;
	}


}



