/*==============================================================================

   [.h]
														 Author : Imai Hayato
														 Date   : //
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef MANAGER_GAME_SOLO_H
#define MANAGER_GAME_SOLO_H

#include "manager_game.h"

class SystemUI_Local_Manager;

class GameManagerSolo :public GameManager
{
private:
	SystemUI_Local_Manager* m_MultiWindowUI = nullptr;
	int m_Score = 0;
public:
	GameManagerSolo();
	~GameManagerSolo();

	void Update()override;
	void Draw(int)override;

	int GetScore();
private:
	void stagepreviewupdate() override;
	void countdownupdate() override;
	void playupdate() override;
	void eventupdate() override {};
	void resultannounceupdate() override;
	void endupdate() override;

	void stagepreviewdraw(int) override;
	void countdowndraw(int) override;
	void playdraw(int) override;
	void eventdraw(int) override {};
	void resultannouncedraw(int) override;
	void enddraw(int) override;

	void controllerupdate() override;
};

#endif // !MANAGER_GAME_SOLO_H