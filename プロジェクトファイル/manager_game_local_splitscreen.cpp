//********************************************************************************
//
// ゲームマネージャーローカルスプリットスクリーン版[manager_game_local_splitscreen.h] 
// 
//															Author :Riugo Honda
//															Date   :2024/02/02
// -------------------------------------------------------------------------------
//															Last Edited:2024/02/02
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
#include "camera_fixedpoint.h"
#include "ocean.h"
#include "skydome.h"
#include "scene.h"
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
#include "fade.h"

#include "cri.h"

#include "manager_game_local_splitscreen.h"

#include "keyboard.h"
//=============================================================================
// ウィンドウ数1枚プレイヤー2人
//=============================================================================

LocalSplitScreenGameManager::LocalSplitScreenGameManager()
{
	m_pControllers.push_back(new Controller(0));
	m_pControllers.push_back(new Controller(1));

	m_GameState = GAMESTATE_STAGE_PREVIEW;
	m_DroneState = DRONE_CIRCLEVIEW;
	m_FixedpointState = FIXEDPOINT_TOP;
	m_CameraType = CAMERATYPE_PLAYER;

	m_pShips.reserve(2);
	m_pShips.push_back(new Ship(0, XMFLOAT3(-25.0f, 0.0f, 0.0f), m_pControllers[0]));
	m_pShips.push_back(new Ship(1, XMFLOAT3(25.0f, 0.0f, 0.0f), m_pControllers[1]));

	m_pManagerUIs.push_back(new ManagerUI(m_pShips[0]));
	m_pManagerUIs.push_back(new ManagerUI(m_pShips[1]));

	m_pShips[0]->RegisterPlayers(new Player(m_pShips[0], /*m_pManagerUIs[0],*/ m_pControllers[0], 0));
	m_pShips[1]->RegisterPlayers(new Player(m_pShips[1], /*m_pManagerUIs[1],*/ m_pControllers[1], 1));

	m_pShips[0]->SetEnemyShip(m_pShips[1]);
	m_pShips[1]->SetEnemyShip(m_pShips[0]);

	m_pCameras.push_back(new PlayerCamera(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-25.0f, 0.0f, 0.0f), m_pShips[0], m_pShips[0]->GetPlayers()[0], GAMEMODE_LOCAL_SPLITSCREEN));
	m_pCameras.push_back(new PlayerCamera(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(25.0f, 0.0f, 0.0f), m_pShips[1], m_pShips[1]->GetPlayers()[0], GAMEMODE_LOCAL_SPLITSCREEN));

	m_pShips[0]->GetPlayers()[0]->SetCameraPointer(m_pCameras[0]);
	m_pShips[1]->GetPlayers()[0]->SetCameraPointer(m_pCameras[1]);

	m_LocalUI = new SystemUI_Local_Manager(m_pShips);
	m_LocalEventManager = new LocalEventManager(m_pShips, m_pControllers);

	m_pShips[0]->GetPlayers()[0]->SetManagerUI(m_pManagerUIs[0]);
	m_pShips[1]->GetPlayers()[0]->SetManagerUI(m_pManagerUIs[1]);

	//海上旋回ドローン
	m_pDrones.push_back(new DroneCamera({ 0.0f, 0.0f , 0.0f }, DRONE_TYPE_CIRCLEVIEW));

	//定点カメラドローン
	//上からの視界
	m_pFixedpointCameras.push_back(new FixedpointCamera({ m_pShips[0]->GetCenterPosition().x , m_pShips[0]->GetCenterPosition().y , m_pShips[0]->GetCenterPosition().z -4.0f }, {m_pShips[0]->GetCenterPosition().x , m_pShips[0]->GetCenterPosition().y + 34.0f , m_pShips[0]->GetCenterPosition().z}, FIXEDPOINT_TOPVIEW, GAMEMODE_LOCAL_SPLITSCREEN));
	m_pFixedpointCameras.push_back(new FixedpointCamera({ m_pShips[1]->GetCenterPosition().x , m_pShips[1]->GetCenterPosition().y , m_pShips[1]->GetCenterPosition().z - 4.0f }, { m_pShips[1]->GetCenterPosition().x , m_pShips[1]->GetCenterPosition().y + 34.0f , m_pShips[1]->GetCenterPosition().z }, FIXEDPOINT_TOPVIEW, GAMEMODE_LOCAL_SPLITSCREEN));

	//船後方からの視界
	m_pFixedpointCameras.push_back(new FixedpointCamera(m_pShips[0]->GetCenterPosition(), { m_pShips[0]->GetCenterPosition().x , m_pShips[0]->GetCenterPosition().y + 3.0f , m_pShips[0]->GetCenterPosition().z - 18.0f}, FIXEDPOINT_REARVIEW, GAMEMODE_LOCAL_SPLITSCREEN));
	m_pFixedpointCameras.push_back(new FixedpointCamera(m_pShips[1]->GetCenterPosition(), { m_pShips[1]->GetCenterPosition().x , m_pShips[1]->GetCenterPosition().y + 3.0f , m_pShips[1]->GetCenterPosition().z - 18.0f}, FIXEDPOINT_REARVIEW, GAMEMODE_LOCAL_SPLITSCREEN));

	//船の右からの視点
	m_pFixedpointCameras.push_back(new FixedpointCamera(m_pShips[0]->GetCenterPosition(), { m_pShips[0]->GetCenterPosition().x -25.0f, m_pShips[0]->GetCenterPosition().y + 4.0f , m_pShips[0]->GetCenterPosition().z }, FIXEDPOINT_SIDEVIEW_RIGHT, GAMEMODE_LOCAL_SPLITSCREEN));
	m_pFixedpointCameras.push_back(new FixedpointCamera(m_pShips[1]->GetCenterPosition(), { m_pShips[1]->GetCenterPosition().x +25.0f, m_pShips[1]->GetCenterPosition().y + 4.0f , m_pShips[1]->GetCenterPosition().z }, FIXEDPOINT_SIDEVIEW_RIGHT, GAMEMODE_LOCAL_SPLITSCREEN));

	//船の左からの視点
	m_pFixedpointCameras.push_back(new FixedpointCamera(m_pShips[0]->GetCenterPosition(), { m_pShips[0]->GetCenterPosition().x - 25.0f, m_pShips[0]->GetCenterPosition().y + 4.0f , m_pShips[0]->GetCenterPosition().z }, FIXEDPOINT_SIDEVIEW_LEFT, GAMEMODE_LOCAL_SPLITSCREEN));
	m_pFixedpointCameras.push_back(new FixedpointCamera(m_pShips[1]->GetCenterPosition(), { m_pShips[1]->GetCenterPosition().x + 25.0f, m_pShips[1]->GetCenterPosition().y + 4.0f , m_pShips[1]->GetCenterPosition().z }, FIXEDPOINT_SIDEVIEW_LEFT, GAMEMODE_LOCAL_SPLITSCREEN));

	m_Timer = 0.0f;
}

LocalSplitScreenGameManager::~LocalSplitScreenGameManager()
{
	for (int i = 0; i < m_pShips.size(); i++) {
		if (m_pShips[i]) {
			delete m_pShips[i];
			m_pShips[i] = nullptr;
		}
	}

	for (int i = 0; i < m_pCameras.size(); i++) {
		if (m_pCameras[i]) {
			delete m_pCameras[i];
			m_pCameras[i] = nullptr;
		}
	}
	for (int i = 0; i < m_pManagerUIs.size(); i++) {
		if (m_pManagerUIs[i]) {
			delete m_pManagerUIs[i];
			m_pManagerUIs[i] = nullptr;
		}
	}
	for (int i = 0; i < m_pControllers.size(); i++) {
		if (m_pControllers[i]) {
			delete m_pControllers[i];
			m_pControllers[i] = nullptr;
		}
	}

	m_pDrones.clear();

	m_pFixedpointCameras.clear();

	delete m_LocalUI;
	m_LocalUI = nullptr;

	delete m_LocalEventManager;
	m_LocalEventManager = nullptr;
}

void LocalSplitScreenGameManager::Update()
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

void LocalSplitScreenGameManager::Draw(int windownumber)
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

void LocalSplitScreenGameManager::stagepreviewupdate()
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

void LocalSplitScreenGameManager::countdownupdate()
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

void LocalSplitScreenGameManager::playupdate()
{
	controllerupdate();

	//if (Keyboard_IsKeyTrigger(KK_T))// デバッグ用キーボード操作
	//{
	//	m_LocalEventManager->SetEvent();
	//	m_GameState = GAMESTATE_EVENT;
	//}

	//イベントの突入フラグ
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

	for (int i = 0; i < m_pShips.size(); i++) {
		if (m_pShips[i]) {
			m_pShips[i]->Update();
		}
	}
	for (int i = 0; i < 2; i++)
	{
		m_pCameras[i]->Update();
	}
	//定点カメラのアップデート
	for (auto itr : m_pFixedpointCameras)
	{
		itr->Update();
	}


	//船の総体力23100
	if (m_pShips[0]->GetShipHP() < 13100.0f/*2100.0f*/)// デバッグ用にHP10000以下
	{
		// バイブレーションを0に
		for (int i = 0; i < m_pControllers.size(); i++)
		{
			m_pControllers[i]->Controller_SetVibration(0, 0, 0);
		}
		m_GameState = GAMESTATE_RESULT_ANNOUNCE;
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
		m_GameState = GAMESTATE_RESULT_ANNOUNCE;
		SetWhoWon(true);
		SEStart(GetAppSE(), CRI_SE_笛);
	}

	if (m_Timer >= 180)
	{
		if (m_pShips[0]->GetShipHP() > m_pShips[1]->GetShipHP())
		{
			// バイブレーションを0に
			for (int i = 0; i < m_pControllers.size(); i++)
			{
				m_pControllers[i]->Controller_SetVibration(0, 0, 0);
			}
			m_GameState = GAMESTATE_RESULT_ANNOUNCE;
			SetWhoWon(true);
		}
		else
		{
			// バイブレーションを0に
			for (int i = 0; i < m_pControllers.size(); i++)
			{
				m_pControllers[i]->Controller_SetVibration(0, 0, 0);
			}
			m_GameState = GAMESTATE_RESULT_ANNOUNCE;
			SetWhoWon(false);
		}
		//万が一引分けた場合のパターンをのちに追加

		SEStart(GetAppSE(), CRI_SE_笛);
	}

	////定点カメラの切り替え
	//if (Keyboard_IsKeyTrigger(KK_M))
	//{
	//	m_CameraType = CAMERATYPE_FIXEDPOINT;
	//}
	//if (Keyboard_IsKeyTrigger(KK_N))
	//{
	//	m_CameraType = CAMERATYPE_PLAYER;
	//}
	//if (Keyboard_IsKeyTrigger(KK_D1))
	//{
	//	m_FixedpointState = FIXEDPOINT_TOP;
	//}
	//if (Keyboard_IsKeyTrigger(KK_D2))
	//{
	//	m_FixedpointState = FIXEDPOINT_REAR;
	//}
	//if (Keyboard_IsKeyTrigger(KK_D3))
	//{
	//	m_FixedpointState = FIXEDPOINT_SIDE_RIGHT;
	//}
	//if (Keyboard_IsKeyTrigger(KK_D4))
	//{
	//	m_FixedpointState = FIXEDPOINT_SIDE_LEFT;
	//}


	//デバック用に強制終了
	/*if (Keyboard_IsKeyDown(KK_D0) && Keyboard_IsKeyTrigger(KK_D9))
	{
		m_GameState = GAMESTATE_END;
		SetWhoWon(false);
	}*/

	////デバックように強制ステート変更
	//if (Keyboard_IsKeyDown(KK_D8) && Keyboard_IsKeyTrigger(KK_D7))
	//{
	//	m_GameState = GAMESTATE_RESULT_ANNOUNCE;

	//	if (m_pShips[0]->GetShipHP() > m_pShips[1]->GetShipHP())
	//	{
	//		m_GameState = GAMESTATE_RESULT_ANNOUNCE;
	//		SetWhoWon(true);
	//	}
	//	else
	//	{
	//		m_GameState = GAMESTATE_RESULT_ANNOUNCE;
	//		SetWhoWon(false);
	//	}
	//}

	



	for (int i = 0; i < m_pManagerUIs.size(); i++)
	{
		m_pManagerUIs[i]->Update();
	}
	m_LocalUI->SetTimer(m_Timer);
	m_Timer = m_FrameCount / 60;
	m_FrameCount++;
	m_LocalUI->Update();
}

void LocalSplitScreenGameManager::eventupdate()
{
	controllerupdate();

	for (int i = 0; i < m_pCameras.size(); i++)
	{
		m_pCameras[i]->Update();
	}

	m_LocalEventManager->Update();

	if (m_LocalEventManager->GetEndFlag())
	{
		m_GameState = GAMESTATE_PLAY;
		m_LocalEventManager->DeleteEvent();
	}
}

void LocalSplitScreenGameManager::resultannounceupdate()
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

void LocalSplitScreenGameManager::endupdate()
{
	if (GetFadeState() == FADE_STATE_FIXED)
	{
		FadeOut();

		SceneChange(SCENE_RESULT);
	}
}

void LocalSplitScreenGameManager::stagepreviewdraw(int windownumber)
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

void LocalSplitScreenGameManager::countdowndraw(int windownumber)
{
	for (int i = 0; i < 2; i++)
	{

		GetSwapChains()[windownumber]->SetMultiViewport(i);
		m_pCameras[i]->Draw();
		SetView(m_pCameras[i]->GetCameraView());
		SetProjection(m_pCameras[i]->GetCameraProjection());
		SetEffectProjection(m_pCameras[i]->GetCameraProjection());
		SetEffectView(m_pCameras[i]->GetCameraView());
		SetBillboardProjection(m_pCameras[i]->GetCameraProjection());
		SetBillboardView(m_pCameras[i]->GetCameraView());

		SkydomeDraw();
		OceanDraw();
		

		for (int j = 0; j < m_pShips.size(); j++)
		{
			if (m_pShips[j])
			{
				m_pShips[j]->Draw();
			}
		}
		EffectManagerDraw();
		BillBoradManagerDraw();

		m_pManagerUIs[i]->Draw();

	}

	GetSwapChains()[windownumber]->SetViewport();

	m_LocalUI->Draw();
}

void LocalSplitScreenGameManager::playdraw(int windownumber)
{
	switch (m_CameraType)
	{
	case CAMERATYPE_PLAYER:
	{
		for (int i = 0; i < 2; i++)
		{
			GetSwapChains()[windownumber]->SetMultiViewport(i);
			m_pCameras[i]->Draw();
			SetView(m_pCameras[i]->GetCameraView());
			SetProjection(m_pCameras[i]->GetCameraProjection());
			SetEffectProjection(m_pCameras[i]->GetCameraProjection());
			SetEffectView(m_pCameras[i]->GetCameraView());
			SetBillboardProjection(m_pCameras[i]->GetCameraProjection());
			SetBillboardView(m_pCameras[i]->GetCameraView());

			SkydomeDraw();
			OceanDraw();
			


			for (int j = 0; j < m_pShips.size(); j++)
			{
				if (m_pShips[j])
				{
					m_pShips[j]->Draw();
				}
			}
			EffectManagerDraw();
			BillBoradManagerDraw();

			m_pManagerUIs[i]->Draw();

		}

		GetSwapChains()[windownumber]->SetViewport();

		m_LocalUI->Draw();
	}
	break;
	case CAMERATYPE_FIXEDPOINT:
	{
		switch (m_FixedpointState)
		{
		case FIXEDPOINT_TOP:
		{
			static int viewportnumber = 0;
			for (auto itr : m_pFixedpointCameras)
			{
				if (itr->GetFixedpointView() == FIXEDPOINT_TOPVIEW)
				{

					GetSwapChains()[windownumber]->SetMultiViewport(viewportnumber);
					itr->Draw();
					SetView(itr->GetCameraView());
					SetProjection(itr->GetCameraProjection());
					SetEffectProjection(itr->GetCameraProjection());
					SetEffectView(itr->GetCameraView());

					SkydomeDraw();
					OceanDraw();
					

					for (int j = 0; j < m_pShips.size(); j++)
					{
						if (m_pShips[j])
						{
							m_pShips[j]->Draw();
						}
					}
					EffectManagerDraw();

					viewportnumber++;
				}
			}
			viewportnumber = 0;

			GetSwapChains()[windownumber]->SetViewport();
		}
		break;
		case FIXEDPOINT_REAR:
		{
			static int viewportnumber = 0;
			for (auto itr : m_pFixedpointCameras)
			{
				if (itr->GetFixedpointView() == FIXEDPOINT_REARVIEW)
				{

					GetSwapChains()[windownumber]->SetMultiViewport(viewportnumber);
					itr->Draw();
					SetView(itr->GetCameraView());
					SetProjection(itr->GetCameraProjection());
					SetEffectProjection(itr->GetCameraProjection());
					SetEffectView(itr->GetCameraView());

					SkydomeDraw();
					OceanDraw();
					

					for (int j = 0; j < m_pShips.size(); j++)
					{
						if (m_pShips[j])
						{
							m_pShips[j]->Draw();
						}
					}
					EffectManagerDraw();

					viewportnumber++;
				}
			}
			viewportnumber = 0;

			GetSwapChains()[windownumber]->SetViewport();
		}
		break;
		case FIXEDPOINT_SIDE_RIGHT:
		{
			static int viewportnumber = 0;
			for (auto itr : m_pFixedpointCameras)
			{
				if (itr->GetFixedpointView() == FIXEDPOINT_SIDEVIEW_RIGHT)
				{

					GetSwapChains()[windownumber]->SetMultiViewport(viewportnumber);
					itr->Draw();
					SetView(itr->GetCameraView());
					SetProjection(itr->GetCameraProjection());
					SetEffectProjection(itr->GetCameraProjection());
					SetEffectView(itr->GetCameraView());

					SkydomeDraw();
					OceanDraw();
				

					for (int j = 0; j < m_pShips.size(); j++)
					{
						if (m_pShips[j])
						{
							m_pShips[j]->Draw();
						}
					}
					EffectManagerDraw();

					viewportnumber++;
				}
			}
			viewportnumber = 0;

			GetSwapChains()[windownumber]->SetViewport();
		}
		break;
		case FIXEDPOINT_SIDE_LEFT:
		{
			static int viewportnumber = 0;
			for (auto itr : m_pFixedpointCameras)
			{
				if (itr->GetFixedpointView() == FIXEDPOINT_SIDEVIEW_LEFT)
				{

					GetSwapChains()[windownumber]->SetMultiViewport(viewportnumber);
					itr->Draw();
					SetView(itr->GetCameraView());
					SetProjection(itr->GetCameraProjection());
					SetEffectProjection(itr->GetCameraProjection());
					SetEffectView(itr->GetCameraView());

					SkydomeDraw();
					OceanDraw();
					

					for (int j = 0; j < m_pShips.size(); j++)
					{
						if (m_pShips[j])
						{
							m_pShips[j]->Draw();
						}
					}
					EffectManagerDraw();

					viewportnumber++;
				}
			}
			viewportnumber = 0;

			GetSwapChains()[windownumber]->SetViewport();
		}
		break;
		}
	}
	break;
	}
}

void LocalSplitScreenGameManager::eventdraw(int windownumber)
{
	for (int i = 0; i < 2; i++)
	{
		GetSwapChains()[windownumber]->SetMultiViewport(i);
		m_pCameras[i]->Draw();
		SetView(m_pCameras[i]->GetCameraView());
		SetProjection(m_pCameras[i]->GetCameraProjection());
		SetEffectProjection(m_pCameras[i]->GetCameraProjection());
		SetEffectView(m_pCameras[i]->GetCameraView());
		SetBillboardProjection(m_pCameras[i]->GetCameraProjection());
		SetBillboardView(m_pCameras[i]->GetCameraView());

		SkydomeDraw();
		OceanDraw();
		

		for (int j = 0; j < m_pShips.size(); j++)
		{
			if (m_pShips[j])
			{
				m_pShips[j]->Draw();
			}
		}
		EffectManagerDraw();
		BillBoradManagerDraw();

		m_pManagerUIs[i]->Draw();

	}

	GetSwapChains()[windownumber]->SetViewport();

	m_LocalUI->Draw();

	GetSwapChains()[0]->SetViewport();
	// イベントのDraw();
	m_LocalEventManager->Draw(windownumber);
}

void LocalSplitScreenGameManager::resultannouncedraw(int windownumber)
{
	for (int i = 0; i < 2; i++)
	{
		GetSwapChains()[windownumber]->SetMultiViewport(i);
		m_pCameras[i]->Draw();
		SetView(m_pCameras[i]->GetCameraView());
		SetProjection(m_pCameras[i]->GetCameraProjection());
		SetEffectProjection(m_pCameras[i]->GetCameraProjection());
		SetEffectView(m_pCameras[i]->GetCameraView());
		SetBillboardProjection(m_pCameras[i]->GetCameraProjection());
		SetBillboardView(m_pCameras[i]->GetCameraView());

		SkydomeDraw();
		OceanDraw();



		for (int j = 0; j < m_pShips.size(); j++)
		{
			if (m_pShips[j])
			{
				m_pShips[j]->Draw();
			}
		}
		EffectManagerDraw();
		BillBoradManagerDraw();

		m_pManagerUIs[i]->Draw();

	}

	GetSwapChains()[windownumber]->SetViewport();

	m_LocalUI->Draw();


	ResultAnnounceDraw();
}

void LocalSplitScreenGameManager::enddraw(int windownumber)
{
	for (int i = 0; i < 2; i++)
	{
		GetSwapChains()[windownumber]->SetMultiViewport(i);
		m_pCameras[i]->Draw();
		SetView(m_pCameras[i]->GetCameraView());
		SetProjection(m_pCameras[i]->GetCameraProjection());
		SetEffectProjection(m_pCameras[i]->GetCameraProjection());
		SetEffectView(m_pCameras[i]->GetCameraView());
		SetBillboardProjection(m_pCameras[i]->GetCameraProjection());
		SetBillboardView(m_pCameras[i]->GetCameraView());

		SkydomeDraw();
		OceanDraw();
		

		for (int j = 0; j < m_pShips.size(); j++)
		{
			if (m_pShips[j])
			{
				m_pShips[j]->Draw();
			}
		}
		EffectManagerDraw();
		BillBoradManagerDraw();

		m_pManagerUIs[i]->Draw();

	}

	GetSwapChains()[windownumber]->SetViewport();

	m_LocalUI->Draw();

	ResultAnnounceDraw();
}

void LocalSplitScreenGameManager::controllerupdate()
{
	for (auto itr : m_pControllers)
	{
		itr->Controller_Update();

		itr->Controller_UpdateVibration();
	}
}
