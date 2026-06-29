//********************************************************************************
//
// ゲームマネージャーマルチウィンドウ版[manager_game_multiwindow.h] 
// 
//															Author : Riugo Honda
//															Date   :2025/01/30
// -------------------------------------------------------------------------------
//															Last Edited:2025/01/30
//********************************************************************************
#ifndef MANAGER_GAME_MULTIWINDOW_H
#define MANAGER_GAME_MULTIWINDOW_H

#include "directx.h"
#include <list>
#include <vector>

class SystemUI_Local_Manager;
class LocalEventManager;

class MultiWindowGameManager:public GameManager
{
private:
	SystemUI_Local_Manager* m_MultiWindowUI = nullptr;
	LocalEventManager* m_LocalEventManager = nullptr;
public:
	MultiWindowGameManager();
	~MultiWindowGameManager();

	void Update() override;
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

#endif //MANAGER_GAME_MULTIWINDOW_H
