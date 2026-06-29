/*==============================================================================

   プレイヤー [player.cpp]
														 Author : Souta Fukamizu
														 Date   : 2024/11/19
--------------------------------------------------------------------------------

==============================================================================*/
#include "directx.h"
#include <vector>
#include "math.h"
#include "camera_player.h"
#include "shader.h"
#include "texture.h"
#include "keyboard.h"
#include "controller.h"
#include "collision.h"

#include "mode.h"
#include "deckblock.h"
#include "deck.h"
#include "cannon.h"
#include "gather.h"
#include "storage.h"
#include "enhance.h"
#include "action_gather.h"
#include "action_cannon.h"
#include "action_repair.h"
#include "action_enhance.h"
#include "ship.h"
#include "manager_ui.h"
#include "manager_model.h"
#include "manager_animation.h"
#include "manager_effect.h"
#include "ui_resource.h"
#include "ui_minimap.h"
#include "entry.h"
#include "client.h"
#include "cri.h"

#include "player.h"


#define GRAVITY_SCALE (-9.8f)
#define AIR_TIME (2.0f) 

Player::Player(Ship* pship, XMFLOAT3 position, ManagerUI* uimanager, Controller* controller):Object3D(pship, OBJECTTYPE_PLAYER,position)
{
	m_Id = 0;
	m_oldposition = position;
	m_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fov = position;

	m_itemstate = ITEMSTATE_NONE;
	m_action = ACTIONSTATE_IDLE;
	m_direction = DIRECTIONSTATE_NONE;
	
	m_pManagerUI = uimanager;
	
	m_CannonAction = new CannonAction(this);
	m_GatherAction = new GatherAction(this);
	m_pRepairAction = new RepairAction(this, GetShip()->GetEnhance());
	m_EnhanceAction = new EnhanceAction(this);
	m_pAnimationManager = new AnimationManager(m_Id);
	m_pController = controller;
	
	uimanager->GetResource()->SetPlayerResource(this);
}

Player::Player(Ship* pship, XMFLOAT3 position, Controller* controller) :Object3D(pship, OBJECTTYPE_PLAYER, position)
{
	//m_Id = 0;
	m_oldposition = position;
	m_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fov = position;
	m_itemstate = ITEMSTATE_NONE;
	m_action = ACTIONSTATE_IDLE;
	m_direction = DIRECTIONSTATE_NONE;
	/*
	m_pManagerUI = uimanager;*/

	m_pRepairAction = new RepairAction(this, GetShip()->GetEnhance());
	m_CannonAction = new CannonAction(this);
	m_GatherAction = new GatherAction(this);

	
	m_pAnimationManager = new AnimationManager(m_Id);
	m_pController = controller;
	//uimanager->GetResource()->SetPlayerResource(this);
}

Player::Player(Ship* pship, XMFLOAT3 position, Controller* controller, int ID) : Object3D(pship, OBJECTTYPE_PLAYER, position)
{
	m_Id = ID;
	m_oldposition = position;
	m_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fov = position;
	
	m_itemstate = ITEMSTATE_NONE;
	m_action = ACTIONSTATE_IDLE;
	m_direction = DIRECTIONSTATE_NONE;

	m_pRepairAction = new RepairAction(this, GetShip()->GetEnhance());
	m_CannonAction = new CannonAction(this);
	m_GatherAction = new GatherAction(this);

	m_pAnimationManager = new AnimationManager(m_Id);
	m_pController = controller;
}

Player::Player(Ship* pship, XMFLOAT3 position, XMFLOAT3 respawnposition, Controller* controller, int ID) : Object3D(pship, OBJECTTYPE_PLAYER, position)
{
	m_Id = ID;
	m_oldposition = position;
	m_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fov = position;
	m_RespawnPosition = respawnposition;
	m_RespawnFlag = false;
	m_RespawnCount = 0;
	m_RespawnInterval = PLAYER_DEFAULT_RESPAWNTIME;
	m_DrawShadow = false;

	m_itemstate = ITEMSTATE_NONE;
	m_action = ACTIONSTATE_IDLE;
	m_direction = DIRECTIONSTATE_NONE;
	m_pController = controller;
	
	m_pRepairAction = new RepairAction(this, GetShip()->GetEnhance());
	m_CannonAction = new CannonAction(this);
	m_GatherAction = new GatherAction(this);
	
	m_pAnimationManager = new AnimationManager(m_Id);
	
}

Player::Player(Ship* pship, Controller* controller, int ID) : Object3D(pship, OBJECTTYPE_PLAYER)
{
	m_Id = ID;

	XMFLOAT3 pos = GetShip()->GetCenterPosition();

	pos.y += 1.45f;
	pos.z += MAP_BLOCK_SIZE * 1.5f - MAP_BLOCK_SIZE * ((float)MAP_HEIGHT_NUM * 0.5f);

	switch (m_Id / 2)
	{
	case 0:
		pos.x -= MAP_BLOCK_SIZE * 3.0f;
		break;

	case 1:
		pos.x += MAP_BLOCK_SIZE * 3.0f;
		break;

	default:
		break;
	}

	SetPosition(pos);

	m_oldposition = pos;
	m_velocity = { 0.0f, 0.0f, 0.0f };
	m_fov = { 0.0f, 0.0f, 0.0f };
	m_RespawnPosition = GetPosition();
	m_RespawnFlag = false;
	m_RespawnCount = 0;
	m_RespawnInterval = PLAYER_DEFAULT_RESPAWNTIME;
	m_DrawShadow = false;

	m_itemstate = ITEMSTATE_NONE;
	m_action = ACTIONSTATE_STANDBY;
	m_direction = DIRECTIONSTATE_NONE;
	m_pController = controller;

	m_pRepairAction = new RepairAction(this, GetShip()->GetEnhance());
	m_CannonAction = new CannonAction(this);
	m_GatherAction = new GatherAction(this);

	m_pAnimationManager = new AnimationManager(m_Id);

	//プレイヤーのレベルによってスピード値計算
	m_PlayerSpeedLevel = m_pRepairAction->GetEnhance()->GetRepairLevel();
	m_PlayerSpeed = PLAYER_SPEED;
	if (GetGameMode() == GAMEMODE_SOLO)
	{
		m_PlayerSpeed = PLAYER_SPEED + 3.0f;
	}
}

Player::Player(Ship* ship, int id, AnimationManager* animmanager) : Object3D(ship, OBJECTTYPE_PLAYER),m_pAnimationManager(animmanager), m_Id(id)
{
	m_itemstate = ITEMSTATE_NONE;
	m_action = ACTIONSTATE_IDLE;

	XMFLOAT3 pos = GetShip()->GetCenterPosition();

	pos.y += 1.45f;
	pos.z += MAP_BLOCK_SIZE * 1.5f - MAP_BLOCK_SIZE * ((float)MAP_HEIGHT_NUM * 0.5f);

	switch (m_Id / 2)
	{
	case 0:
		pos.x -= MAP_BLOCK_SIZE * 3.0f;
		break;

	case 1:
		pos.x += MAP_BLOCK_SIZE * 3.0f;
		break;

	default:
		break;
	}

	SetPosition(pos);
}

Player::~Player()
{
	if (m_CannonAction) { delete m_CannonAction; }
	if (m_GatherAction) { delete m_GatherAction; }
	if (m_pRepairAction) { delete m_pRepairAction; }
	if (m_EnhanceAction) { delete m_EnhanceAction; }
	if (m_pAnimationManager) { delete m_pAnimationManager; }
}



void Player::Update(void)
{
	switch (m_action)
	{
	case ACTIONSTATE_IDLE:
		Move();
		Action();
		ItemDrop();//アイテムを落とすかどうか
		break;

	case ACTIONSTATE_RUN:
		Move();
		Action();
		ItemDrop();//アイテムを落とすかどうか
		break;

	case ACTIONSTATE_CARRY:
		Move();
		Action();
		ItemDrop();//アイテムを落とすかどうか
		break;

	case ACTIONSTATE_GATHER:
		m_GatherAction->Update();
		break;

	case ACTIONSTATE_REPAIR:
		m_pRepairAction->Update();
		break;
	case ACTIONSTATE_CANNON:
		m_CannonAction->Update();
		break;
		
	case ACTIONSTATE_ENHANCE:
		m_EnhanceAction->Update();
		break;

	case ACTIONSTATE_BLOWOFF:
		BlowOff();
		break;

	case ACTIONSTATE_RESPAWN_WAIT:
		WaitRespawn();
		break;

	case ACTIONSTATE_RESPAWN:
		Respawn();
		break;

	case ACTIONSTATE_STANDBY:

		break;

	default:
		break;
	}

	CheckShadowOnDeck();

	SetAnimation();
	m_pAnimationManager->Update();

	if (GetGameMode() == GAMEMODE_ONLINE) {
		PlayerData data;
		data.playerid = m_Id;
		data.teamid = GetShip()->GetTeamId();
		data.position = GetPosition();
		data.rotation = GetRotation();
		data.anim = GetAnimationManager()->GetAnimation();
		GetClient()->SetMyPlayerData(data);
		GetClient()->SendMyPlayerData();

		SwitchListenerPosition(GetApp3dSE(), GetPosition());
	}
	else
	{
		SwitchListenerPosition(GetApp3dSE(m_pController->GetPlayerIndex()), GetPosition());
	}

	
	

	

}

void Player::Draw(void) const
{
	if (!m_RespawnFlag)
		m_pAnimationManager->Draw(GetPosition(), GetRotation());

	if (m_DrawShadow)
	{
		XMFLOAT3 ShadowPosition = { GetPosition().x, GetShip()->GetCenterPosition().y + MAP_BLOCK_SIZE * 0.5f + 0.01f, GetPosition().z }; //y座標は甲板とかぶらないように0.01上げる
		XMFLOAT3 ShadowSize = { 1.5f, 1.5f, 1.5f };
		ModelDraw(MODELNAME_SHADOW, ShadowPosition, {0.0f, 0.0f, 0.0f}, ShadowSize);
	}


	//ModelDraw(MODELNAME_FLOTSAM_ENHANCE, GetPosition(), GetRotation());

	m_GatherAction->Draw();
}

void Player::SetId(int Id)
{
	m_Id = Id;
}

int Player::GetId()
{
	return m_Id;
}

XMFLOAT3 Player::GetOldPosition()
{
	return m_oldposition;
}

void Player::SetVelocity(XMFLOAT3 velocity)
{
	m_velocity = velocity;
}

XMFLOAT3 Player::GetVelocity()
{
	return m_velocity;
}

void Player::SetItemState(ItemState item)
{
	m_itemstate = item;
}

ItemState Player::GetItemState()
{
	return m_itemstate;
}

void Player::SetAction(ActionState action)
{
	m_action = action;
}

ActionState Player::GetAction()
{
	return m_action;
}

void Player::SetDirectionState(DirectionState direction)
{
	m_direction = direction;
}

DirectionState Player::GetDirectionState()
{
	return m_direction;
}

void Player::SetRespawnTime(int time)
{
	m_RespawnInterval = time;
}

void Player::SetFov(XMFLOAT3 fov)
{
	m_fov = fov;
}

void Player::PositionInterpolation(Object3D* object)
{
	float ObjectSize = 1.0f;
	float ObjectLength = 0.0f;

	XMFLOAT3 direction;
	direction.x = object->GetPosition().x - GetPosition().x;
	direction.y = 0;
	direction.z = object->GetPosition().z - GetPosition().z;

	float length = sqrtf(direction.x * direction.x + direction.z * direction.z);

	direction.x /= length;
	direction.z /= length;

	if(object->GetObjectType() != OBJECTTYPE_ENHANCE)
	{
		ObjectLength = ObjectSize + (PLAYER_SIZE * 0.5f);
		if (m_Id == 1)	//かじきのくちが長いので調整
			ObjectLength += PLAYER_SIZE * 0.3f;

		SetPosition(XMFLOAT3(object->GetPosition().x - (ObjectLength * direction.x), GetPosition().y, object->GetPosition().z));
		m_oldposition = XMFLOAT3(object->GetPosition().x + (1.5f * -direction.x), GetPosition().y, GetPosition().z);
	}
	else
	{
		ObjectLength = ObjectSize + PLAYER_SIZE;
		if (m_Id == 1) //かじきのくちが長いので調整
			ObjectLength += PLAYER_SIZE * 0.3f;

		SetPosition(XMFLOAT3(object->GetPosition().x, GetPosition().y, object->GetPosition().z - (ObjectLength * direction.z)));
		m_oldposition = GetPosition();
	}

	direction.x = object->GetPosition().x - GetPosition().x;
	direction.y = 0;
	direction.z = object->GetPosition().z - GetPosition().z;

	length = sqrtf(direction.x * direction.x + direction.z * direction.z);

	direction.x /= length;
	direction.z /= length;
	float angle = atan2f(direction.x, direction.z);

	SetRotation(XMFLOAT3(0.0f, angle, 0.0f));

}

void Player::SetAnimation(void)
{
	if (m_action == ACTIONSTATE_RUN)
	{
		if (m_pAnimationManager->GetAnimation() != ANIMATIONSTATE_RUN && m_itemstate == ITEMSTATE_NONE)
		{
			m_pAnimationManager->SetAnimation(ANIMATIONSTATE_RUN);
		}

		if (m_pAnimationManager->GetAnimation() != ANIMATIONSTATE_CARRY_CANNON && m_itemstate == ITEMSTATE_CANNON)
		{
			m_pAnimationManager->SetAnimation(ANIMATIONSTATE_CARRY_CANNON);
		}

		if (m_pAnimationManager->GetAnimation() != ANIMATIONSTATE_CARRY_WOOD && m_itemstate == ITEMSTATE_WOOD)
		{
			m_pAnimationManager->SetAnimation(ANIMATIONSTATE_CARRY_WOOD);
		}

	}
	else 
	{
		if ((m_action == ACTIONSTATE_IDLE))
		{
			if (m_pAnimationManager->GetAnimation() != ANIMATIONSTATE_IDLE && m_itemstate == ITEMSTATE_NONE)
			{
				m_pAnimationManager->SetAnimation(ANIMATIONSTATE_IDLE);
			}

			if ((m_pAnimationManager->GetAnimation() != ANIMATIONSTATE_CARRY_CANNON_IDLE && m_itemstate == ITEMSTATE_CANNON))
			{
				m_pAnimationManager->SetAnimation(ANIMATIONSTATE_CARRY_CANNON_IDLE);
			}
			
			if (m_pAnimationManager->GetAnimation() != ANIMATIONSTATE_CARRY_WOOD_IDLE && m_itemstate == ITEMSTATE_WOOD)
			{
				m_pAnimationManager->SetAnimation(ANIMATIONSTATE_CARRY_WOOD_IDLE);
			}

		}
		
	}
}

XMFLOAT3 Player::GetFov()
{
	return m_fov;
}

Controller* Player::GetController()
{
	return m_pController;
}

ManagerUI* Player::GetManagerUI(void)
{
	return m_pManagerUI;
}

AnimationManager* Player::GetAnimationManager(void) const
{
	return m_pAnimationManager;
}



void Player::Move(void)
{
	XMFLOAT3 movedirection = { 0.0f, 0.0f, 0.0f };

	float lx = 0.0f;
	float ly = 0.0f;

	if (m_pController->IsConnected())
	{
		lx = m_pController->GetLeftStickX();
		ly = m_pController->GetLeftStickY();
	}
	if (!m_pController->IsConnected()) {
		//加速
		if (Keyboard_IsKeyDown(KK_A))
		{
			lx += -1.0f;
		}
		if (Keyboard_IsKeyDown(KK_D))
		{
			lx += 1.0f;
		}
		if (Keyboard_IsKeyDown(KK_W))
		{
			ly += 1.0f;
		}
		if (Keyboard_IsKeyDown(KK_S))
		{
			ly += -1.0f;
		}
	}

	if (lx != 0 || ly != 0)
	{
		XMFLOAT3 forwardDir = m_pCamera->GetFrontDirection();
		XMFLOAT3 RightDir = m_pCamera->GetRightDirection();

		movedirection.x = forwardDir.x * ly + RightDir.x * lx;
		movedirection.y = 0;
		movedirection.z = forwardDir.z * ly + RightDir.z * lx;

		float x = sqrtf(movedirection.x * movedirection.x + movedirection.z * movedirection.z);

		movedirection.x /= x;
		movedirection.z /= x;

		float angle = atan2f(movedirection.x, movedirection.z);

		SetRotation(XMFLOAT3(0.0f, angle, 0.0f));

	}

	if (m_PlayerSpeedLevel != m_pRepairAction->GetEnhance()->GetRepairLevel())
	{
		switch (m_pRepairAction->GetEnhance()->GetRepairLevel())
		{
		case 0:
			m_PlayerSpeedLevel = m_pRepairAction->GetEnhance()->GetRepairLevel();
			m_PlayerSpeed = PLAYER_SPEED;
			break;
		case 1:
			m_PlayerSpeedLevel = m_pRepairAction->GetEnhance()->GetRepairLevel();
			m_PlayerSpeed = PLAYER_SPEED + 0.5f;
			break;
		case 2:
			m_PlayerSpeedLevel = m_pRepairAction->GetEnhance()->GetRepairLevel();
			m_PlayerSpeed = PLAYER_SPEED + 1.0f;
			break;
		case 3:
			m_PlayerSpeedLevel = m_pRepairAction->GetEnhance()->GetRepairLevel();
			m_PlayerSpeed = PLAYER_SPEED + 1.5f;
			break;
		case 4:
			m_PlayerSpeedLevel = m_pRepairAction->GetEnhance()->GetRepairLevel();
			m_PlayerSpeed = PLAYER_SPEED + 2.0f;
			break;
		case 5:
			m_PlayerSpeedLevel = m_pRepairAction->GetEnhance()->GetRepairLevel();
			m_PlayerSpeed = PLAYER_SPEED + 3.0f;
			break;
		}
	}

	//経過時間
	float dt = 1.0f / 60.0f;

	//加速
	m_velocity.x = movedirection.x * m_PlayerSpeed * dt;
	m_velocity.y = 0.0f;
	m_velocity.z = movedirection.z * m_PlayerSpeed * dt;
	
	//移動
	SetPosition(XMFLOAT3(GetPosition().x + m_velocity.x, GetPosition().y + m_velocity.y, GetPosition().z + m_velocity.z));
	CheckMap();
	m_action = ACTIONSTATE_IDLE;

	if (/*GetPosition().x != m_oldposition.x || GetPosition().z != m_oldposition.z*/movedirection.x != 0.0f || movedirection.z != 0.0f )
	{
		m_fov.x = GetPosition().x - m_oldposition.x;
		m_fov.y = GetPosition().y - m_oldposition.y;
		m_fov.z = GetPosition().z - m_oldposition.z;
				
		m_action = ACTIONSTATE_RUN;
		m_oldposition = GetPosition();

		m_pAnimationManager->SetRunEffect(GetPosition(), movedirection);
	}

	//もし海に沈んだままなら
	if (GetPosition().y < m_RespawnPosition.y)
	{
		Respawn();
	}

}

void Player::WaitRespawn(void)
{
	if (m_RespawnCount >= m_RespawnInterval)
		m_action = ACTIONSTATE_RESPAWN;
	
	m_RespawnCount++;
}

void Player::Respawn(void)
{
	//std::vector<DeckBlock*> pDeckBlocks = GetShip()->GetDeckBlocks();
	////float x = pDeckBlocks[0]->GetPosition().x + MAP_BLOCK_SIZE * 14.5f;
	////float x2 = GetPosition().x + PLAYER_SIZE * 0.5f;
	//bool RespawnFlag = false;
	//if (pDeckBlocks[0]->GetPosition().z + MAP_BLOCK_SIZE * 0.5f <= GetPosition().z + PLAYER_SIZE * 0.5f || pDeckBlocks[0]->GetPosition().z - MAP_BLOCK_SIZE * 26.5f >= GetPosition().z - PLAYER_SIZE * 0.5f) {
	//	RespawnFlag = true;
	//}
	//if (pDeckBlocks[0]->GetPosition().x + MAP_BLOCK_SIZE * 14.5f <= GetPosition().x + PLAYER_SIZE * 0.5f || pDeckBlocks[0]->GetPosition().x - MAP_BLOCK_SIZE * 0.5f >= GetPosition().x - PLAYER_SIZE * 0.5f) {
	//	RespawnFlag = true;
	//}


	//for (int MapCnt = 0; MapCnt < pDeckBlocks.size(); MapCnt++) {

	//	if (!pDeckBlocks[MapCnt]->GetUse()){
	//		if (CheckBoxCollider(XMFLOAT2(GetPosition().x, GetPosition().z), XMFLOAT2(pDeckBlocks[MapCnt]->GetPosition().x, pDeckBlocks[MapCnt]->GetPosition().z), XMFLOAT2(PLAYER_SIZE, PLAYER_SIZE), XMFLOAT2(MAP_BLOCK_SIZE, MAP_BLOCK_SIZE))) {
	//			RespawnFlag = true;
	//		}
	//	}
	//}


	//if (RespawnFlag) {
	//	pDeckBlocks = GetShip()->GetDeckBlocks();
	//	XMFLOAT3 RespawnPosition = pDeckBlocks[363]->GetPosition();
	//	RespawnPosition.y += MAP_BLOCK_SIZE * 0.5f + PLAYER_SIZE * 0.5f;
	//	SetPosition(RespawnPosition);
	//	m_action = ACTIONSTATE_IDLE;
	//}

	SetPosition(m_RespawnPosition);
	SetRotation({ 0.0f, 0.0f, 0.0f });

	m_action = ACTIONSTATE_IDLE;
	m_itemstate = ITEMSTATE_NONE;
	m_pAnimationManager->SetAnimation(ANIMATIONSTATE_IDLE);

	m_RespawnFlag = false;

	m_RespawnCount = 0;

	m_GatherAction->AimCancel();
	m_pCamera->SetCameraState(CAMERA_STATE_RESET);
	m_pManagerUI->SetUIState(UI_STATE_NORMAL);

}

void Player::CheckMap()
{
	std::vector<DeckBlock*> pDeckBlocks = GetShip()->GetDeckBlocks();

	//float x = pDeckBlocks[0]->GetPosition().x + MAP_BLOCK_SIZE * 14.5f;
	//float x2 = GetPosition().x + PLAYER_SIZE * 0.5f;

	if (GetShip()->GetCenterPosition().z + MAP_BLOCK_SIZE * (MAP_HEIGHT_NUM * 0.5f) <= GetPosition().z + PLAYER_SIZE * 0.5f)
	{
		SetPosition(XMFLOAT3(GetPosition().x, GetPosition().y, GetShip()->GetCenterPosition().z + MAP_BLOCK_SIZE * (MAP_HEIGHT_NUM * 0.5f) - 0.5f));
	}
	else if (GetShip()->GetCenterPosition().z - MAP_BLOCK_SIZE * (MAP_HEIGHT_NUM * 0.5f) >= GetPosition().z - PLAYER_SIZE * 0.5f)
	{
		SetPosition(XMFLOAT3(GetPosition().x, GetPosition().y, GetShip()->GetCenterPosition().z - MAP_BLOCK_SIZE * (MAP_HEIGHT_NUM * 0.5f) + 0.5f));
	}
	
	if (GetShip()->GetCenterPosition().x + MAP_BLOCK_SIZE * (MAP_WIDTH_NUM * 0.5f) <= GetPosition().x + PLAYER_SIZE * 0.5f)
	{
		SetPosition(XMFLOAT3(GetShip()->GetCenterPosition().x + MAP_BLOCK_SIZE * (MAP_WIDTH_NUM * 0.5f) - 0.5f, GetPosition().y, GetPosition().z));
	}
	else if(GetShip()->GetCenterPosition().x - MAP_BLOCK_SIZE * (MAP_WIDTH_NUM * 0.5f) >= GetPosition().x - PLAYER_SIZE * 0.5f)
	{
		SetPosition(XMFLOAT3(GetShip()->GetCenterPosition().x - MAP_BLOCK_SIZE * (MAP_WIDTH_NUM * 0.5f) + 0.5f, GetPosition().y, GetPosition().z));
	}

	for (int MapCnt = 0; MapCnt < pDeckBlocks.size(); MapCnt++) {

		if (!pDeckBlocks[MapCnt]->GetUse())
		{
			if (CheckBoxCollider(XMFLOAT2(GetPosition().x, GetPosition().z), XMFLOAT2(pDeckBlocks[MapCnt]->GetPosition().x, pDeckBlocks[MapCnt]->GetPosition().z), XMFLOAT2(PLAYER_SIZE * 0.5f, PLAYER_SIZE * 0.5f), XMFLOAT2(MAP_BLOCK_SIZE, MAP_BLOCK_SIZE))) {
				SetPosition(m_oldposition);
				return;
			}
		}

	}

}



void Player::Action(void)
{
	if ((!m_pController->IsConnected() && Keyboard_IsKeyTrigger(KK_SPACE)) || (m_pController->IsConnected() && m_pController->Controller_IsJustPressed(m_pController->GetButtonForTrigger(XINPUT_GAMEPAD_B))))
	{
		//大砲
		if (m_itemstate == ITEMSTATE_CANNON)
		{
			std::vector<Cannon*> pCannons = GetShip()->GetCannons();

			for (int i = 0; i < pCannons.size(); i++)
			{
				XMFLOAT3 objectvector = { pCannons[i]->GetPosition().x - GetPosition().x, 0.0f, pCannons[i]->GetPosition().z - GetPosition().z };
				float inner = m_fov.x * objectvector.x + m_fov.z * objectvector.z;

				if (pCannons[i]->GetCannonActionFlag())
				{
					if (CheckCircleCollider(XMFLOAT2(GetPosition().x, GetPosition().z), XMFLOAT2(pCannons[i]->GetPosition().x, pCannons[i]->GetPosition().z), 0.0f, 2.5f))
					{
						if (inner >= 0)
						{
							if (GetGameMode() == GAMEMODE_ONLINE) {
								UseReqest data;
								data.object = USEOBJECT_CANNON;
								data.useid = i;
								data.teamid = GetClient()->GetMyTeamId();
								data.type = REQUEST_ASK;

								GetClient()->SendUseReqest(data);
							}
							else {
								m_CannonAction->UseCannonObject(pCannons[i]);
								m_action = ACTIONSTATE_CANNON;
								m_pCamera->SetCameraState(CAMERA_STATE_ACTION_CANNON); //仮追加
								m_pAnimationManager->SetAnimation(ANIMATIONSTATE_HOLD);
								PositionInterpolation(pCannons[i]);
							}
							


						}
					}
				}
			}
		}

		//修復
		if (m_itemstate == ITEMSTATE_WOOD)
		{
			if (m_pRepairAction->GetRepairCount() <= 0)
			{
				m_action = ACTIONSTATE_IDLE;
				m_itemstate = ITEMSTATE_NONE;
				return;
			}

			std::vector<DeckBlock*> pDeckBlocks = GetShip()->GetDeckBlocks();

			for (int i = 0; i < pDeckBlocks.size(); i++)
			{


				XMFLOAT3 objectvector = { pDeckBlocks[i]->GetPosition().x - GetPosition().x, 0.0f, pDeckBlocks[i]->GetPosition().z - GetPosition().z };
				float inner = m_fov.x * objectvector.x + m_fov.z * objectvector.z;

				if (inner >= 0)
				{
					if (pDeckBlocks[i]->GetDeckType() == DECKTYPE_WOOD && !pDeckBlocks[i]->GetUse())
					{
						if (CheckCircleCollider(XMFLOAT2(GetPosition().x, GetPosition().z),
							XMFLOAT2(pDeckBlocks[i]->GetPosition().x, pDeckBlocks[i]->GetPosition().z), 0.0f, 1.5f))
						{
							if (GetGameMode() == GAMEMODE_ONLINE) {
								UseReqest data;
								data.object = USEOBJECT_REPAIR_DECKBLOCK;
								data.useid = i;
								data.teamid = GetClient()->GetMyTeamId();
								data.type = REQUEST_ASK;

								GetClient()->SendUseReqest(data);

							}
							else {
								m_pRepairAction->SetRepairBlock(pDeckBlocks[i]);
								m_action = ACTIONSTATE_REPAIR;
								m_pAnimationManager->SetAnimation(ANIMATIONSTATE_REPAIR);
							}

							return;
						}
					}
				}
			}
		}

		if (m_itemstate == ITEMSTATE_NONE)
		{
			std::vector<Gather*> pGathers = GetShip()->GetGathers();
			std::vector<Storage*> pStorages = GetShip()->GetStorages();
			Enhance* pEnhance = GetShip()->GetEnhance();

			//Enhance
			{
				XMFLOAT3 objectvector = { pEnhance->GetPosition().x - GetPosition().x, 0.0f, pEnhance->GetPosition().z - GetPosition().z };
				float inner = m_fov.x * objectvector.x + m_fov.z * objectvector.z;

				if (pEnhance->GetUsingEnhance())
				{
					if (CheckCircleCollider(XMFLOAT2(GetPosition().x, GetPosition().z), XMFLOAT2(pEnhance->GetPosition().x, pEnhance->GetPosition().z), 0.0f, 2.5f))
					{
						if (inner >= 0)
						{

							if (GetGameMode() == GAMEMODE_ONLINE) {
								UseReqest data;
								data.object = USEOBJECT_ENHANCE;
								data.teamid = GetClient()->GetMyTeamId();
								data.type = REQUEST_ASK;

								GetClient()->SendUseReqest(data);

							}
							else {
								SetAction(ACTIONSTATE_ENHANCE);
								m_pAnimationManager->SetAnimation(ANIMATIONSTATE_IDLE);
								pEnhance->SetUsingEnhance(false);
								m_pManagerUI->SetUIState(UI_STATE_ENHANCE);
								PositionInterpolation(pEnhance);
							}
							if (GetGameMode() != GAMEMODE_ONLINE)
							{

								for (int i = 0; i < 4; i++)
								{
									CreateSource(GetApp3dSE(i), GetPosition(), CRI_SE3D_強化画面, 3.0f);
								}
							}

						}
					}
				}
			}


			//Gather
			for (int i = 0; i < pGathers.size(); i++)
			{
				XMFLOAT3 objectvector = { pGathers[i]->GetPosition().x - GetPosition().x, 0.0f, pGathers[i]->GetPosition().z - GetPosition().z };
				float inner = m_fov.x * objectvector.x + m_fov.z * objectvector.z;

				if (pGathers[i]->GetUsingGather())
				{
					if (CheckCircleCollider(XMFLOAT2(GetPosition().x, GetPosition().z), XMFLOAT2(pGathers[i]->GetPosition().x, pGathers[i]->GetPosition().z), 0.0f, 2.5f))
					{
						if (inner >= 0)
						{
							if (GetGameMode() == GAMEMODE_ONLINE) {
								UseReqest data;
								data.object = USEOBJECT_GATHER;
								data.useid = i;
								data.teamid = GetClient()->GetMyTeamId();
								data.type = REQUEST_ASK;

								GetClient()->SendUseReqest(data);

							}
							else {
								SetAction(ACTIONSTATE_GATHER);
								m_pCamera->SetCameraState(CAMERA_STATE_ACTION_GATHER); //仮追加
								m_GatherAction->UseGatherObject(pGathers[i]);
								m_pAnimationManager->SetAnimation(ANIMATIONSTATE_HOLD);
								PositionInterpolation(pGathers[i]);
							}
						}
					}
				}
			}

			//ストレージアクセス
			for (auto storage : pStorages)
			{
				XMFLOAT3 objectvector = { storage->GetPosition().x - GetPosition().x, 0.0f, storage->GetPosition().z - GetPosition().z };
				float inner = m_fov.x * objectvector.x + m_fov.z * objectvector.z;


				if (CheckCircleCollider(XMFLOAT2(GetPosition().x, GetPosition().z),
					XMFLOAT2(storage->GetPosition().x, storage->GetPosition().z), 0.0f, 2.5f))
				{
					if (inner >= 0)
					{
						if (storage->GetType() == STORAGETYPE_REPAIR)
						{
							if (storage->GetStock() > 0)
							{
	

								m_pRepairAction->SetRepairCount();

								if (GetGameMode() == GAMEMODE_ONLINE) {
									UseReqest data;
									data.object = USEOBJECT_WOOD_STORAGE;
									data.teamid = GetClient()->GetMyTeamId();
									data.type = REQUEST_ASK;

									GetClient()->SendUseReqest(data);

								}
								else {
									//修復に必要な個数は後できめる
									storage->Decrease(1);

									m_itemstate = ITEMSTATE_WOOD;
								}

								if (GetGameMode() != GAMEMODE_ONLINE)
								{
									for (int i = 0; i < 4; i++)
									{
										CreateSource(GetApp3dSE(i), GetPosition(), CRI_SE3D_ストレージ);
									}
								}
							}
							return;
						}
					}

					if (storage->GetType() == STORAGETYPE_CANNON)
					{
						if (storage->GetStock() > 0)
						{

							if (GetGameMode() == GAMEMODE_ONLINE) {
								UseReqest data;
								data.object = USEOBJECT_CANNON_STORAGE;
								data.teamid = GetClient()->GetMyTeamId();
								data.type = REQUEST_ASK;

								GetClient()->SendUseReqest(data);

							}
							else {
								storage->Decrease(1);
								m_itemstate = ITEMSTATE_CANNON;
							}
							if (GetGameMode() != GAMEMODE_ONLINE)
							{
								for (int i = 0; i < 4; i++)
								{
									CreateSource(GetApp3dSE(i), GetPosition(), CRI_SE3D_ストレージ);
								}
							}

						}

						return;
					}
				}
			}

		}
	}
}
	

void Player::SetBlowOff(int RespawnTime)
{
	std::vector<DeckBlock*> pDeckBlocks = GetShip()->GetDeckBlocks();

	int Id = GetShip()->GetTeamId();
	if (Id == 0)
	{
		m_velocity.x = (pDeckBlocks[0]->GetPosition().x - 10.0f) - GetPosition().x;

		if (GetGameMode() == GAMEMODE_ONLINE)
		{
			CreateSource(GetApp3dSE(), GetPosition(), CRI_SE3D_吹っ飛ぶ, 2.0f);
		}
		else
		{

			for (int i = 0; i < 4; i++)
			{
				CreateSource(GetApp3dSE(i), GetPosition(), CRI_SE3D_吹っ飛ぶ, 2.0f);
			}
		}
	}
	else
	{
		m_velocity.x = (pDeckBlocks[0]->GetPosition().x + (MAP_BLOCK_SIZE * 14.5f) + 10.0f) - GetPosition().x;

		if (GetGameMode() == GAMEMODE_ONLINE)
		{
			CreateSource(GetApp3dSE(), GetPosition(), CRI_SE3D_吹っ飛ぶ, 2.0f);
		}
		else
		{

			for (int i = 0; i < 4; i++)
			{
				CreateSource(GetApp3dSE(i), GetPosition(), CRI_SE3D_吹っ飛ぶ, 2.0f);
			}
		}
	}

	m_velocity.y = 0.0f - GetPosition().y;
	m_velocity.z = 0.0f;


	//xz軸の移動時間

	m_velocity.x = m_velocity.x / AIR_TIME;
	m_velocity.z = m_velocity.z / AIR_TIME;


	//Y軸の移動
	//Y方向の最高点
	//※Y方向の傾きが考慮されてないのでxz軸の移動時間とのズレあり
	if (m_velocity.y > 0)
	{
		m_velocity.y = (m_velocity.y / AIR_TIME) + (0.5f * GRAVITY_SCALE * AIR_TIME);//ありえないけど一応の場合分け
	}
	else
	{
		m_velocity.y = (m_velocity.y / AIR_TIME) - (0.5f * GRAVITY_SCALE * AIR_TIME);
	}
	

	m_itemstate = ITEMSTATE_NONE;
	m_action = ACTIONSTATE_BLOWOFF;
	m_RespawnInterval = RespawnTime;
	
	if (m_pAnimationManager) { m_pAnimationManager->SetAnimation(ANIMATIONSTATE_RUN); }
	if (m_pCamera) { m_pCamera->SetCameraState(CAMERA_STATE_BLOWOFF); }
	if (m_pManagerUI) { m_pManagerUI->SetUIState(UI_STATE_NORMAL); }
	SetRotation({ 0.0f, 0.0f ,0.0f });
	
}

void Player::BlowOff(void)
{
	//経過時間
	float dt = 1.0f / 60.0f;

	float angle = 1.0f / 180.0f;


	//重力
	m_velocity.y += -9.8f * dt;


	SetPosition(XMFLOAT3(GetPosition().x + (m_velocity.x * dt), GetPosition().y + (m_velocity.y * dt), GetPosition().z + (m_velocity.z * dt)));


	if (GetShip()->GetTeamId() == 0)
	{
		//SetRotation({ GetRotation().x, GetRotation().y + XM_PI * 0.25f, GetRotation().z });
		SetRotation({ (GetRotation().x),
			(GetRotation().y),
			(GetRotation().z) + 0.5f });
	}
	else
	{

		//SetRotation({ GetRotation().x - XM_PI * (angle * 45.0f * dt), GetRotation().y/* + XM_PI * (angle * 360.0f * dt)*/, GetRotation().z  /*- XM_PI * (angle * 45.0f * dt)*/ });
		SetRotation({ (GetRotation().x),
			(GetRotation().y),
			(GetRotation().z) - 0.5f });
	}

	if (-4.0f >= GetPosition().y)
	{
		EffectManagerRegister(EFFECTTYPE_SPLASH, GetPosition(), { 3.0f, 3.0f, 3.0f });
		EffectManagerRegister(EFFECTTYPE_SPLASH_MOJI, GetPosition(), { 3.0f, 3.0f, 3.0f });
		m_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
		//Respawn();
		m_action = ACTIONSTATE_RESPAWN_WAIT;
		m_RespawnFlag = true;

	}
}

void Player::ItemDrop(void)
{
	if ((!m_pController->IsConnected() && Keyboard_IsKeyTrigger(KK_TAB)) || (m_pController->IsConnected() && m_pController->Controller_IsJustPressed(m_pController->GetButtonForTrigger(XINPUT_GAMEPAD_A))))
	{
		if (m_itemstate == ITEMSTATE_NONE) {
			return; 
		}

		m_itemstate = ITEMSTATE_NONE;

	}
}

void Player::SetCameraPointer(PlayerCamera* p_camera)
{
	m_pCamera = p_camera;
}

PlayerCamera* Player::GetCamera()
{
	if (m_pCamera == nullptr)
	{
		return nullptr;
	}

	return m_pCamera;
}

void Player::SetManagerUI(ManagerUI* pManager)
{
	m_pManagerUI = pManager;
	m_EnhanceAction = new EnhanceAction(this);
	m_pManagerUI->GetResource()->SetPlayerResource(this);
	if (GetGameMode()==GAMEMODE_SEGMENTATION)
	{
		m_pManagerUI->GetMiniMap()->SetMiniMap(this->GetShip(), this->GetId());
	}
	else
	{
		m_pManagerUI->GetMiniMap()->SetMiniMap(this->GetShip());
	}
}

void Player::Repair(void)
{
	std::vector<DeckBlock*> pDeckBlocks = GetShip()->GetDeckBlocks();

}

void Player::CheckShadowOnDeck(void)
{
	XMFLOAT3 ShipCenter = GetShip()->GetCenterPosition();
	float cubeRadius = 0.5f * MAP_BLOCK_SIZE;
	m_DrawShadow = true;

	if (ShipCenter.x + MAP_BLOCK_SIZE * (MAP_WIDTH_NUM * 0.5f) <= GetPosition().x + PLAYER_SIZE * 0.5f || ShipCenter.x - MAP_BLOCK_SIZE * (MAP_WIDTH_NUM * 0.5f) >= GetPosition().x - PLAYER_SIZE * 0.5f)
	{
		m_DrawShadow = false;
	}


}

void Player::OnlineAction(UseReqest data)
{
	switch (data.object)
	{
	case USEOBJECT_CANNON: {
		std::vector<Cannon*> pCannons = GetShip()->GetCannons();

		m_CannonAction->UseCannonObject(pCannons[data.useid]);
		m_action = ACTIONSTATE_CANNON;
		m_pCamera->SetCameraState(CAMERA_STATE_ACTION_CANNON); //仮追加
		m_pAnimationManager->SetAnimation(ANIMATIONSTATE_HOLD);
		PositionInterpolation(pCannons[data.useid]);
		break;
	}
	case USEOBJECT_GATHER: {
		std::vector<Gather*> pGathers = GetShip()->GetGathers();
		SetAction(ACTIONSTATE_GATHER);
		m_pCamera->SetCameraState(CAMERA_STATE_ACTION_GATHER); //仮追加
		m_GatherAction->UseGatherObject(pGathers[data.useid]);
		m_pAnimationManager->SetAnimation(ANIMATIONSTATE_HOLD);
		PositionInterpolation(pGathers[data.useid]);
		break;
	}
	case USEOBJECT_ENHANCE: {
		Enhance* pEnhance = GetShip()->GetEnhance();

		SetAction(ACTIONSTATE_ENHANCE);
		m_pAnimationManager->SetAnimation(ANIMATIONSTATE_IDLE);
		pEnhance->SetUsingEnhance(false);
		m_pManagerUI->SetUIState(UI_STATE_ENHANCE);
		PositionInterpolation(pEnhance);
		CreateSource(GetApp3dSE(), GetPosition(), CRI_SE3D_強化画面, 3.0f);
		break;
	}
	case USEOBJECT_WOOD_STORAGE: {
		//std::vector<Storage*> pStorages = GetShip()->GetStorages();
		m_itemstate = ITEMSTATE_WOOD;
		m_pRepairAction->SetRepairCount();
		break;
	}
	case USEOBJECT_CANNON_STORAGE: {
		//std::vector<Storage*> pStorages = GetShip()->GetStorages();
		m_itemstate = ITEMSTATE_CANNON;
		CreateSource(GetApp3dSE(), GetPosition(), CRI_SE3D_ストレージ);
		break;
	}
	case USEOBJECT_REPAIR_DECKBLOCK: {
		std::vector<DeckBlock*> pDeckBlocks = GetShip()->GetDeckBlocks();

		m_pRepairAction->SetRepairBlock(pDeckBlocks[data.useid]);
		m_action = ACTIONSTATE_REPAIR;
		m_pAnimationManager->SetAnimation(ANIMATIONSTATE_REPAIR);
		CreateSource(GetApp3dSE(), GetPosition(), CRI_SE3D_ストレージ);
		break;
	}
	default:
		break;
	}
	
}


