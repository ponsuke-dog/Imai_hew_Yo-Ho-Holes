
#include "directx.h"
#include <vector>

#include "manager_game.h"
#include "manager_game_solo.h"
#include "manager_game_local.h"
#include "manager_game_local_splitscreen.h"
#include "manager_game_multisplitwindow.h"
#include "manager_game_multiwindow.h"
#include "manager_game_online.h"
#include "manager_model.h"
#include "scene.h"
#include "mode.h"
#include "manager_effect.h"
#include "manager_billboard.h"
#include "fade.h"
#include "ocean.h"
#include "skydome.h"
#include "resultannounce.h"
#include "countdown.h"
#include "client.h"
#include "keyboard.h"
#include "ship.h"
#include "player.h"

#include "cri.h"

#include "game.h"


static GameManager* g_GameManager = nullptr;

static int g_TimeCount = 0;

void GameInitialize(void)
{
	
	
	//ModelManagerInitialize();
	
	
	EffectManagerInitialize();
	BillBoradManagerInitialize();

	switch (GetGameMode())
	{
	case GAMEMODE_SOLO:
	{
		g_GameManager = new GameManagerSolo;
	}
		break;
	case GAMEMODE_LOCAL_SPLITSCREEN:
	{
		g_GameManager = new LocalSplitScreenGameManager;
	}
		break;
	case GAMEMODE_LOCAL_MULTIWINDOW:
	{
		g_GameManager = new LocalGameManager;
	}
		break;
	case GAMEMODE_SEGMENTATION:
	{
		g_GameManager = new MultiSplitWindowGameManager;
	}
		break;
	case GAMEMODE_MULTIWINDOW:
	{
		g_GameManager = new MultiWindowGameManager;
	}
		break;
	case GAMEMODE_ONLINE:
	{
		g_GameManager = new OnlineGameManager;
	}
	break;
	}


	SkydomeInitialize(g_GameManager);
	OceanInitialize(g_GameManager);
	//g_pGameManager = new GameManager;
	
	CountDownInitialize();
	
	ResultAnnounceInitialize();
	
	g_TimeCount = 0;
}

void GameFinalize(void)
{
	OceanFinalize();
	SkydomeFinalize();

	ResultAnnounceFinalize();

	CountDownFinalize();

	delete g_GameManager;
	g_GameManager = nullptr;

	/*delete g_pGameManager;
	g_pGameManager = nullptr;*/
	

	//CameraFinalize();
	BillBoradManagerFinalize();
	EffectManagerFinalize();

	//ModelManagerFinalize();

}

void GameUpdate(void)
{
	g_GameManager->Update();
	

	EffectManagerUpdate();
	BillBoradManagerUpdate();

	if (g_TimeCount / (60 * 30) == 1)
	{
	//	SceneChange(SCENE_RESULT);
	}

	if (g_GameManager->GetCountdown())
	{
		if (!GetCountFlag() && GetFadeState() == FADE_STATE_FIXED)
		{
			StartCountDown(5.0f);

			SEStart(GetAppSE(), CRI_SE_COUNTDOWN);

			g_GameManager->SetCountdown(false);
		}
	}
	

	CountDownUpdate();

	g_TimeCount++;

	if (g_GameManager->GetGameState() == GAMESTATE_COUNTDOWN)
	{
		if (!g_GameManager->GetCountdown() && !GetCountFlag())
		{
			std::vector<Ship*> pShips = g_GameManager->GetShips();

			if (GetGameMode() != GAMEMODE_ONLINE) {
				g_GameManager->SetGameState(GAMESTATE_PLAY);
				for (auto ship : pShips)
				{
					std::vector<Player*> pPlayers = ship->GetPlayers();
					for (auto player : pPlayers)
					{
						player->SetAction(ACTIONSTATE_IDLE);
					}
				}
			}
			else {
				if (GetClient()->GetGameScene().servergame == SERVERGAME_PLAY) {
					g_GameManager->SetGameState(GAMESTATE_PLAY);
					pShips[GetClient()->GetMyTeamId()]->GetPlayers().front()->SetAction(ACTIONSTATE_IDLE);
				}
				else {
					GameStart data;
					data.flag = PLAYERREADY_OK;
					GetClient()->SendMyGameStart(data);
				}
			}
		}
	}

	OceanUpdate();
	SkydomeUpdate();
}

void GameDraw(int windownumber)
{	
	g_GameManager->Draw(windownumber);

	CountDownDraw();
}
