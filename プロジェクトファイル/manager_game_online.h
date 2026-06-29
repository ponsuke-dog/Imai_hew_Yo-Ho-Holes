//********************************************************************************
//
// ゲームマネージャーオンライン版[manager_game_online.h] 
// 
//															Author : Ryotaro Oyama
//															Date   :2025/02/10
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/10
//********************************************************************************
#ifndef MANAGER_GAME_ONLINE_H
#define MANAGER_GAME_ONLINE_H

#include "directx.h"
#include "manager_game.h"
#include <list>
#include <vector>

class SystemUI_Local_Manager;
class LocalEventManager;

class Client;

class OnlineGameManager :public GameManager
{
private:
	SystemUI_Local_Manager* m_MultiWindowUI = nullptr;
	LocalEventManager* m_LocalEventManager = nullptr;
	Client* m_Client = nullptr;
public:
	OnlineGameManager();
	~OnlineGameManager();

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

private:
	void receive_and_update();

};

#endif //MANAGER_GAME_ONLINE_H
