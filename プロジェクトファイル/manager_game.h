//********************************************************************************
//
// ゲームマネージャー[manager_game.h] 
// 
//															Author : Ponsuke
//															Date   :2024/12/08
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/08
//********************************************************************************
#ifndef MANAGER_GAME_H
#define MANAGER_GAME_H

#include "directx.h"
#include <list>
#include <vector>

#define FRAMECOUNT_TO_MIN (60)		//フレームカウントにかけて分に直す定数
#define FIRST_EVENT (50)		//最初のイベントの時間
#define SECOND_EVENT (100)		//二番目のイベントの時間
#define THIRD_EVENT (150)	//三番目のイベントの時間


typedef enum
{
	GAMESTATE_NONE = -1,

	GAMESTATE_STAGE_PREVIEW,
	GAMESTATE_COUNTDOWN,
	GAMESTATE_PLAY,
	GAMESTATE_EVENT,
	GAMESTATE_RESULT_ANNOUNCE,
	GAMESTATE_END,

	GAMESTATE_MAX,

}GameState;

//カメラのどのタイプを表示するかのステート
typedef enum
{
	CAMERATYPE_NONE = -1,

	CAMERATYPE_PLAYER,
	CAMERATYPE_DRONE,
	CAMERATYPE_FIXEDPOINT,

	CAMERATYPE_MAX
}CameraType;

//ドローンのどのタイプのドローンのステート
typedef enum
{
	DRONE_NONE = -1,

	DRONE_RISNG,
	DRONE_CHASING,
	DRONE_CIRCLEVIEW,
}DroneState;

//定点カメラのどの点のステートか
typedef enum
{
	FIXEDPOINT_NONE = -1,

	FIXEDPOINT_TOP,
	FIXEDPOINT_FRONT,
	FIXEDPOINT_REAR,
	FIXEDPOINT_SIDE_RIGHT,
	FIXEDPOINT_SIDE_LEFT,

	FIXEDPOINT_MAX,
}FixedpointState;

class Ship;

class Controller;

class PlayerCamera;
class DroneCamera;
class FixedpointCamera;

class ManagerUI;

class GameManager
{
private:

protected:
	std::vector<Ship*> m_pShips;
	std::vector<PlayerCamera*>m_pCameras;
	std::vector<DroneCamera*>m_pDrones;
	std::vector<FixedpointCamera*>m_pFixedpointCameras;
	std::vector<Controller*>m_pControllers;
	std::vector<ManagerUI*> m_pManagerUIs;

	int m_Timer = 0;
	int m_FrameCount = 0;
	bool m_IsCountdown = false;

	GameState m_GameState = GAMESTATE_NONE;
	DroneState m_DroneState = DRONE_NONE;
	FixedpointState m_FixedpointState = FIXEDPOINT_NONE;
	CameraType m_CameraType = CAMERATYPE_NONE;

	//Controller* m_pController = nullptr;
	//Camera* m_pCamera = nullptr;
	//ManagerUI* m_pManagerUI = nullptr;
	//捨てpと別のアップデート
	virtual void stagepreviewupdate() = 0;
	virtual void countdownupdate() = 0;
	virtual void playupdate() = 0;
	virtual void eventupdate() = 0;
	virtual void resultannounceupdate() = 0;
	virtual void endupdate() = 0;

	virtual void stagepreviewdraw(int) = 0;
	virtual void countdowndraw(int) = 0;
	virtual void playdraw(int) = 0;
	virtual void eventdraw(int) = 0;
	virtual void resultannouncedraw(int) = 0;
	virtual void enddraw(int) = 0;

	virtual void controllerupdate() = 0;

public:
	GameManager(){}
	virtual ~GameManager(){}

	virtual void Update() = 0;
	virtual void Draw(int) = 0;

	void SetGameState(GameState Gamestate) { m_GameState = Gamestate; }
	GameState GetGameState() { return m_GameState; }

	//カウントダウン開始したかのチェックフラグ
	bool GetCountdown() { return m_IsCountdown; }
	void SetCountdown(bool flag) { m_IsCountdown = flag; }

	std::vector<Ship*>& GetShips() { return m_pShips; }
};

#endif //MANAGER_GAME_H