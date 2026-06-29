/*==============================================================================

   プレイヤー [player.h]
														 Author : Souta Fukamizu
														 Date   : 2024/11/19
--------------------------------------------------------------------------------

==============================================================================*/

#ifndef PLAYER_H
#define PLAYER_H

#include "object_3d.h"
#include "packet.h"

#define PLAYER_SIZE (1.0f)
#define PLAYER_SPEED (5.0f)
#define PLAYER_DEFAULT_RESPAWNTIME (60 * 2)

class Controller;
class GatherAction;
class CannonAction;
class RepairAction;
class EnhanceAction;
class AnimationManager;
class ManagerUI;
class PlayerCamera;

//アクションステート
typedef enum
{

	ACTIONSTATE_NONE = -1,
	ACTIONSTATE_IDLE,
	ACTIONSTATE_RUN,
	ACTIONSTATE_CARRY,
	ACTIONSTATE_GATHER,
	ACTIONSTATE_REPAIR,
	ACTIONSTATE_CANNON,
	ACTIONSTATE_ENHANCE,
	ACTIONSTATE_BLOWOFF,
	ACTIONSTATE_RESPAWN_WAIT,
	ACTIONSTATE_RESPAWN,
	ACTIONSTATE_STANDBY,

	ACTIONSTATE_MAX,
}ActionState;

//プレイヤーの向いている方向
typedef enum
{

	DIRECTIONSTATE_NONE = -1,
	DIRECTIONSTATE_FRONT,
	DIRECTIONSTATE_BACK,
	DIRECTIONSTATE_LEFT,
	DIRECTIONSTATE_RIGHT,

	DIRECTIONSTATE_MAX,
}DirectionState;


//プレイヤーの持っているアイテム
typedef enum
{

	ITEMSTATE_NONE = -1,
	ITEMSTATE_WOOD,
	ITEMSTATE_CANNON,
	ITEMSTATE_BUFF,

	ITEMSTATE_MAX,
}ItemState;

class Player : public Object3D
{
private:
	int m_Id = -1;

	XMFLOAT3 m_oldposition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_fov = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_RespawnPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	bool m_RespawnFlag = false;
	int m_RespawnCount = 0;	//リスポーンの時間計測用
	int m_RespawnInterval = PLAYER_DEFAULT_RESPAWNTIME;	//リスポーン間隔
	bool m_DrawShadow = false;

	ItemState m_itemstate = ITEMSTATE_NONE;
	ActionState m_action = ACTIONSTATE_NONE;
	DirectionState m_direction = DIRECTIONSTATE_NONE;

	GatherAction* m_GatherAction = nullptr;
	CannonAction* m_CannonAction = nullptr;
	RepairAction* m_pRepairAction = nullptr;
	EnhanceAction* m_EnhanceAction = nullptr;

	AnimationManager* m_pAnimationManager = nullptr;
	ManagerUI* m_pManagerUI = nullptr;

	Controller* m_pController = nullptr;

	PlayerCamera* m_pCamera = nullptr;

	int m_PlayerSpeedLevel;
	float m_PlayerSpeed;

public:
	Player() = delete;
	Player(Ship* pship, XMFLOAT3 position, ManagerUI* uimanager,Controller* controller);
	Player(Ship* pship, XMFLOAT3 position,Controller* controller);
	Player(Ship* pship, XMFLOAT3 position,Controller* controller,int ID);
	Player(Ship* pship, XMFLOAT3 position, XMFLOAT3 respawnposition, Controller* controller, int ID);
	Player(Ship* pship, Controller* controller, int ID);

	Player(Ship* ship,int id, AnimationManager* animmanager);

	virtual ~Player();

	void Update(void) override;
	void Draw(void) const override;

	void SetId(int Id);
	int GetId(void);
	XMFLOAT3 GetOldPosition(void);
	void SetVelocity(XMFLOAT3 velocity);
	XMFLOAT3 GetVelocity(void);
	void SetItemState(ItemState item);
	ItemState GetItemState(void);
	void SetAction(ActionState action);
	ActionState GetAction(void);
	void SetDirectionState(DirectionState direction);
	
	DirectionState GetDirectionState(void);
	void SetRespawnTime(int time);
	XMFLOAT3 GetFov(void);
	Controller* GetController(void);
	ManagerUI* GetManagerUI(void);
	AnimationManager* GetAnimationManager(void)const;
	
	void SetBlowOff(int RespawnTime = PLAYER_DEFAULT_RESPAWNTIME);

	void Respawn(void);
	

	//仮追加
	void SetCameraPointer(PlayerCamera* p_camera);
	PlayerCamera* GetCamera();

	void SetManagerUI(ManagerUI* pManager);

private:
	void Move(void);
	void WaitRespawn(void);
	void CheckMap(void);
	void Action(void);
	void BlowOff(void);
	void Repair(void);
	void SetFov(XMFLOAT3 fov);
	void PositionInterpolation(Object3D* object);
	void SetAnimation(void);
	void ItemDrop(void);
	void CheckShadowOnDeck(void);

public:
	void OnlineAction(UseReqest data);
};


#endif // !PLAYER_H
