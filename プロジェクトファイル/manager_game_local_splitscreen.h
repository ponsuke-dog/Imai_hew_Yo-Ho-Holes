//********************************************************************************
//
// ゲームマネージャーローカルスプリットスクリーン版[manager_game_local_splitscreen.h] 
// 
//															Author :Riugo Honda
//															Date   :2024/02/02
// -------------------------------------------------------------------------------
//															Last Edited:2024/02/02
//********************************************************************************
#ifndef MANAGER_GAME_LOCAL_SPLITSCREEN_H
#define MANAGER_GAME_LOCAL_SPLITSCREEN_H

#include "directx.h"
#include <list>
#include <vector>

//typedef enum
//{
//	GAMESTATE_NONE = -1,
//
//	GAMESTATE_PLAY,
//	GAMESTATE_EVENT,
//	GAMESTATE_END,
//
//	GAMESTATE_MAX,
//
//}GameState;

class SystemUI_Local_Manager;
class LocalEventManager;

class LocalSplitScreenGameManager:public GameManager
{
private:
	SystemUI_Local_Manager* m_LocalUI = nullptr;
	LocalEventManager* m_LocalEventManager = nullptr;
public:
	LocalSplitScreenGameManager();
	~LocalSplitScreenGameManager();

	void Update()override;
	void Draw(int) override;
private:
	void stagepreviewupdate() override;
	void countdownupdate() override;
	void playupdate() override;
	void eventupdate() override;
	void resultannounceupdate() override;
	void endupdate() override;

	void stagepreviewdraw(int) override;
	void countdowndraw(int) override;
	void playdraw(int) override;
	void eventdraw(int) override;
	void resultannouncedraw(int) override;
	void enddraw(int) override;

	void controllerupdate() override;
};

#endif //MANAGER_GAME_LOCAL_SPLITSCREEN_H
