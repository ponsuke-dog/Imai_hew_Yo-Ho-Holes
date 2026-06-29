//********************************************************************************
//
// ゲームマネージャーローカル版[manager_game_local.h] 
// 
//															Author : Ponsuke
//															Date   :2024/12/26
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/26
//********************************************************************************
#ifndef MANAGER_GAME_LOCAL_H
#define MANAGER_GAME_LOCAL_H

#include "directx.h"
#include <list>
#include <vector>

class SystemUI_Local_Manager;
class LocalEventManager;

class LocalGameManager:public GameManager
{
private:
	SystemUI_Local_Manager* m_LocalUI = nullptr;
	LocalEventManager* m_LocalEventManager = nullptr;
public:
	LocalGameManager();
	~LocalGameManager();

	void Update()override;
	void Draw(int)override;
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

#endif //MANAGER_GAME_LOCAL_H