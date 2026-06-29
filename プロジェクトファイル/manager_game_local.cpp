//********************************************************************************
//
// ゲームマネージャーローカル版[manager_game_local.cpp] 
// 
//															Author : Ponsuke
//															Date   :2024/12/26
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/26
//********************************************************************************
#include "object_3d.h"
#include "player.h"
#include "deckblock.h"
#include "deck.h"
#include "cannon.h"
#include "gather.h"
#include "storage.h"
#include "enhance.h"
#include "flotsam.h"
#include "cannonball.h"
#include "explosion.h"
#include "net.h"
#include "effect_3d.h"
#include "controller.h"
#include "ship.h"
#include "camera_player.h"
#include "camera_drone.h"
#include "scene.h"
#include "ocean.h"
#include "skydome.h"
#include "fade.h"
#include "manager_game.h"
#include "manager_model.h"
#include "manager_field.h"
#include "manager_flotsam.h"
#include "manager_ui.h"
#include "manager_systemui.h"
#include "manager_effect.h"
#include "manager_event_local.h"
#include "manager_billboard.h"
#include "resultannounce.h"
#include "result.h"

#include "cri.h"

#include "manager_game_local.h"

#include "keyboard.h"

LocalGameManager::LocalGameManager()
{
	m_pControllers.push_back(new Controller(0));
	m_pControllers.push_back(new Controller(1));

	//m_pManagerUIs = new ManagerUI();
	m_GameState = GAMESTATE_STAGE_PREVIEW;
	m_DroneState = DRONE_CIRCLEVIEW;

	m_pShips.reserve(2);
	m_pShips.push_back(new Ship(0, XMFLOAT3(-25.0f, 0.0f, 0.0f), m_pControllers[0]));
	m_pShips.push_back(new Ship(1, XMFLOAT3(25.0f, 0.0f, 0.0f), m_pControllers[1]));
	
	m_pManagerUIs.push_back(new ManagerUI(m_pShips[0]));
	m_pManagerUIs.push_back(new ManagerUI(m_pShips[1]));

	m_pShips[0]->RegisterPlayers(new Player(m_pShips[0], m_pControllers[0],0));
	m_pShips[1]->RegisterPlayers(new Player(m_pShips[1], m_pControllers[1],1));

	m_pShips[0]->SetEnemyShip(m_pShips[1]);
	m_pShips[1]->SetEnemyShip(m_pShips[0]);

	m_pCameras.push_back (new PlayerCamera(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-25.0f, 0.0f, 0.0f), m_pShips[0], m_pShips[0]->GetPlayers()[0] , GAMEMODE_LOCAL_MULTIWINDOW));
	m_pCameras.push_back(new PlayerCamera(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(25.0f, 0.0f, 0.0f), m_pShips[1], m_pShips[1]->GetPlayers()[0] , GAMEMODE_LOCAL_MULTIWINDOW));

	m_pShips[0]->GetPlayers()[0]->SetCameraPointer(m_pCameras[0]);
	m_pShips[1]->GetPlayers()[0]->SetCameraPointer(m_pCameras[1]);

	m_pShips[0]->SetEnemyShip(m_pShips[1]);
	m_pShips[1]->SetEnemyShip(m_pShips[0]);

	m_LocalUI = new SystemUI_Local_Manager(m_pShips);

	m_LocalEventManager = new LocalEventManager(m_pShips,m_pControllers);

	m_pShips[0]->GetPlayers()[0]->SetManagerUI(m_pManagerUIs[0]);
	m_pShips[1]->GetPlayers()[0]->SetManagerUI(m_pManagerUIs[1]);

	m_Timer = 0;
	m_FrameCount = 0;

	//海上旋回ドローン
	m_pDrones.push_back(new DroneCamera({ 0.0f, 0.0f , 0.0f }, DRONE_TYPE_CIRCLEVIEW));
}

LocalGameManager::~LocalGameManager()
{
	for (int i = 0; i < m_pShips.size(); i++) {
		if (m_pShips[i]) {
			delete m_pShips[i];
			m_pShips[i] = nullptr;
		}
	}

	for (auto pCamera : m_pCameras)
	{
		if (pCamera)
		{
			delete pCamera;
			pCamera = nullptr;
		}
	}
	for (auto pController : m_pControllers)
	{
		if (pController)
		{
			delete pController;
			pController = nullptr;
		}
	}
	for (auto pManagerUI : m_pManagerUIs)
	{
		if (pManagerUI)
		{
			delete pManagerUI;
			pManagerUI = nullptr;
		}
	}

	m_pDrones.clear();

	delete m_LocalUI;
	m_LocalUI = nullptr;

	delete m_LocalEventManager;
	m_LocalEventManager = nullptr;
}

void LocalGameManager::Update()
{
	switch (m_GameState)
	{
	case GAMESTATE_STAGE_PREVIEW:
		stagepreviewupdate();
	break;
	case GAMESTATE_COUNTDOWN:
		countdownupdate();
	break;
	case GAMESTATE_PLAY:
		playupdate();
		break;
	case GAMESTATE_EVENT:
		eventupdate();
		break;
	case GAMESTATE_RESULT_ANNOUNCE:
		resultannounceupdate();
		break;
	case GAMESTATE_END:
		endupdate();
		break;
	default:
		break;
	}
	
}

void LocalGameManager::Draw(int windownumber)
{
	switch (m_GameState)
	{
	case GAMESTATE_STAGE_PREVIEW:
		stagepreviewdraw(windownumber);
	break;
	case GAMESTATE_COUNTDOWN:
		countdowndraw(windownumber);
	break;
	case GAMESTATE_PLAY:
		playdraw(windownumber);
	break;
	case GAMESTATE_EVENT:
		eventdraw(windownumber);
	break;
	case GAMESTATE_RESULT_ANNOUNCE:
		resultannouncedraw(windownumber);
		break;
	case GAMESTATE_END:
		enddraw(windownumber);
	break;
	default:
		break;
	}

}

void LocalGameManager::stagepreviewupdate()
{
	m_pDrones[0]->Update();

	for (int i = 0; i < m_pShips.size(); i++) {
		if (m_pShips[i])
		{
			m_pShips[i]->FlotsamUpdate();
		}
	}

	if (m_pDrones[0]->IsOneLapFinished() && GetFadeState() == FADE_STATE_FIXED)
	{
		FadeOut();
	}

	if (m_pDrones[0]->IsOneLapFinished() && GetFadeState() == FADE_STATE_IN)
	{
		m_GameState = GAMESTATE_COUNTDOWN;

		SetCountdown(true);
	}

}

void LocalGameManager::countdownupdate()
{
	for (int i = 0; i < m_pShips.size(); i++) {
		if (m_pShips[i]) {
			m_pShips[i]->Update();
		}
	}
	for (int i = 0; i < 2; i++)
	{
		m_pCameras[i]->Update();
	}

	////デバック用に強制終了
	//if (Keyboard_IsKeyDown(KK_D0) && Keyboard_IsKeyTrigger(KK_D9))
	//{
	//	m_GameState = GAMESTATE_END;
	//	SetWhoWon(false);
	//}

	for (int i = 0; i < m_pManagerUIs.size(); i++)
	{
		m_pManagerUIs[i]->Update();
	}
	m_LocalUI->Update();
}

void LocalGameManager::playupdate()
{
	controllerupdate();

	//イベント突入フラグ
	if (m_FrameCount == FIRST_EVENT * FRAMECOUNT_TO_MIN)
	{
		m_LocalEventManager->SetEvent();
		m_GameState = GAMESTATE_EVENT;
	}

	if (m_FrameCount == SECOND_EVENT * FRAMECOUNT_TO_MIN)
	{
		m_LocalEventManager->SetEvent();
		m_GameState = GAMESTATE_EVENT;
	}

	if (m_FrameCount == THIRD_EVENT * FRAMECOUNT_TO_MIN)
	{
		m_LocalEventManager->SetEvent();
		m_GameState = GAMESTATE_EVENT;
	}

	//if (Keyboard_IsKeyTrigger(KK_T))// デバッグ用キーボード操作
	//{
	//	m_LocalEventManager->SetEvent();
	//	m_GameState = GAMESTATE_EVENT;
	//}

	for (int i = 0; i < m_pShips.size(); i++) {
		if (m_pShips[i]) {
			m_pShips[i]->Update();
		}
	}
	for (int i = 0; i < 2; i++)
	{
		m_pCameras[i]->Update();
	}
	// デバッグ用
	//m_pCameras[0]->SetTarget(m_pShips[0]->GetPlayerPos(0));

	//船の総体力23100
	if (m_pShips[0]->GetShipHP() < 13100.0f/*2100.0f*/)// デバッグ用にHP10000以下
	{
		// バイブレーションを0に
		for (int i = 0; i < m_pControllers.size(); i++)
		{
			m_pControllers[i]->Controller_SetVibration(0, 0, 0);
		}
		m_GameState = GAMESTATE_END;
		SetWhoWon(false);
		SEStart(GetAppSE(), CRI_SE_笛);
	}
	else if (m_pShips[1]->GetShipHP() < 13100.0f/*2100.0f*/)
	{
		// バイブレーションを0に
		for (int i = 0; i < m_pControllers.size(); i++)
		{
			m_pControllers[i]->Controller_SetVibration(0, 0, 0);
		}
		m_GameState = GAMESTATE_END;
		SetWhoWon(true);
		SEStart(GetAppSE(), CRI_SE_笛);
	}

	if (m_Timer >= 180)
	{
		// バイブレーションを0に
		for (int i = 0; i < m_pControllers.size(); i++)
		{
			m_pControllers[i]->Controller_SetVibration(0, 0, 0);
		}
		if (m_pShips[0]->GetShipHP() > m_pShips[1]->GetShipHP())
		{
			m_GameState = GAMESTATE_END;
			SetWhoWon(true);
		}
		else
		{
			m_GameState = GAMESTATE_END;
			SetWhoWon(false);
		}
		//万が一引分けた場合のパターンをのちに追加

		SEStart(GetAppSE(), CRI_SE_笛);
	}

	//デバック用に強制終了
	//if (Keyboard_IsKeyDown(KK_D0) && Keyboard_IsKeyTrigger(KK_D9))
	//{
	//	m_GameState = GAMESTATE_END;
	//	SetWhoWon(false);
	//}

	////デバックように強制ステート変更
	//if (Keyboard_IsKeyDown(KK_D8) && Keyboard_IsKeyTrigger(KK_D7))
	//{
	//	m_GameState = GAMESTATE_RESULT_ANNOUNCE;
	//}

	for (int i = 0; i < m_pManagerUIs.size(); i++)
	{
		m_pManagerUIs[i]->Update();
	}
	m_LocalUI->Update();


	m_LocalUI->SetTimer(m_Timer);
	m_Timer = m_FrameCount / 60;
	m_FrameCount++;
}

void LocalGameManager::eventupdate()
{
	controllerupdate();

	m_LocalEventManager->Update();

	if (m_LocalEventManager->GetEndFlag())
	{
		m_GameState = GAMESTATE_PLAY;
		m_LocalEventManager->DeleteEvent();
	}
}

void LocalGameManager::resultannounceupdate()
{
	controllerupdate();

	for (int i = 0; i < 2; i++)
	{
		m_pCameras[i]->Update();
	}

	for (int i = 0; i < m_pManagerUIs.size(); i++)
	{
		m_pManagerUIs[i]->Update();
	}
	m_LocalUI->Update();


	ResultAnnounceUpdate(m_pControllers);

	if (GetResultAnnouncePushFlags()) {
		m_GameState = GAMESTATE_END;
	}
}

void LocalGameManager::endupdate()
{
	if (GetFadeState() == FADE_STATE_FIXED)
	{
		FadeOut();

		SceneChange(SCENE_RESULT);
	}
}

void LocalGameManager::stagepreviewdraw(int windownumber)
{
	for (auto itr : m_pDrones)
	{
		if (itr->GetDroneView() == DRONE_TYPE_CIRCLEVIEW)
		{
			GetSwapChains()[windownumber]->SetViewport();
			itr->Draw();
			SetView(itr->GetCameraView());
			SetProjection(itr->GetCameraProjection());

			OceanDraw();
			SkydomeDraw();

			for (int j = 0; j < m_pShips.size(); j++)
			{
				if (m_pShips[j])
				{
					m_pShips[j]->StandByDraw();
				}
			}

			GetSwapChains()[windownumber]->SetViewport();
		}
	}
}

void LocalGameManager::countdowndraw(int windownumber)
{
	m_pCameras[windownumber]->Draw();
	SetView(m_pCameras[windownumber]->GetCameraView());
	SetProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetEffectProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetEffectView(m_pCameras[windownumber]->GetCameraView());
	SetBillboardProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetBillboardView(m_pCameras[windownumber]->GetCameraView());

	OceanDraw();
	SkydomeDraw();

	for (int i = 0; i < m_pShips.size(); i++)
	{
		if (m_pShips[i])
		{
			m_pShips[i]->Draw();
		}
	}
	EffectManagerDraw();
	BillBoradManagerDraw();

	m_pManagerUIs[windownumber]->Draw();

	m_LocalUI->Draw();
}



void LocalGameManager::playdraw(int windownumber)
{
	
	m_pCameras[windownumber]->Draw();
	SetView(m_pCameras[windownumber]->GetCameraView());
	SetProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetEffectProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetEffectView(m_pCameras[windownumber]->GetCameraView());
	SetBillboardProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetBillboardView(m_pCameras[windownumber]->GetCameraView());

	OceanDraw();
	SkydomeDraw();

	for (int i = 0; i < m_pShips.size(); i++)
	{
		if (m_pShips[i])
		{
			m_pShips[i]->Draw();
		}
	}
	EffectManagerDraw();
	BillBoradManagerDraw();

	m_pManagerUIs[windownumber]->Draw();

	m_LocalUI->Draw();
	
}

void LocalGameManager::eventdraw(int windownumber)
{
	m_pCameras[windownumber]->Draw();
	SetView(m_pCameras[windownumber]->GetCameraView());
	SetProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetEffectProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetEffectView(m_pCameras[windownumber]->GetCameraView());
	SetBillboardProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetBillboardView(m_pCameras[windownumber]->GetCameraView());

	OceanDraw();
	SkydomeDraw();

	for (int i = 0; i < m_pShips.size(); i++)
	{
		if (m_pShips[i])
		{
			m_pShips[i]->Draw();
		}
	}
	EffectManagerDraw();
	BillBoradManagerDraw();

	m_pManagerUIs[windownumber]->Draw();

	m_LocalUI->Draw();

	GetSwapChains()[0]->SetViewport();
	// イベントのDraw();
	m_LocalEventManager->Draw(windownumber);
	
}

void LocalGameManager::resultannouncedraw(int windownumber)
{
	m_pCameras[windownumber]->Draw();
	SetView(m_pCameras[windownumber]->GetCameraView());
	SetProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetEffectProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetEffectView(m_pCameras[windownumber]->GetCameraView());
	SetBillboardProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetBillboardView(m_pCameras[windownumber]->GetCameraView());

	OceanDraw();
	SkydomeDraw();

	for (int i = 0; i < m_pShips.size(); i++)
	{
		if (m_pShips[i])
		{
			m_pShips[i]->Draw();
		}
	}
	EffectManagerDraw();
	BillBoradManagerDraw();

	m_pManagerUIs[windownumber]->Draw();

	m_LocalUI->Draw();

	ResultAnnounceDraw();
}

void LocalGameManager::enddraw(int windownumber)
{
	m_pCameras[windownumber]->Draw();
	SetView(m_pCameras[windownumber]->GetCameraView());
	SetProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetEffectProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetEffectView(m_pCameras[windownumber]->GetCameraView());
	SetBillboardProjection(m_pCameras[windownumber]->GetCameraProjection());
	SetBillboardView(m_pCameras[windownumber]->GetCameraView());

	OceanDraw();
	SkydomeDraw();

	for (int i = 0; i < m_pShips.size(); i++)
	{
		if (m_pShips[i])
		{
			m_pShips[i]->Draw();
		}
	}
	EffectManagerDraw();
	BillBoradManagerDraw();

	m_pManagerUIs[windownumber]->Draw();

	m_LocalUI->Draw();

	ResultAnnounceDraw();
}

void LocalGameManager::controllerupdate()
{
	for (auto itr : m_pControllers)
	{
		itr->Controller_Update();

		itr->Controller_UpdateVibration();
	}
}

