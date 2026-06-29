//********************************************************************************
//
// ゲームマネージャーマルチウィンドウ版[manager_game_multiwindow.h] 
// 
//															Author : Riugo Honda
//															Date   :2025/01/30
// -------------------------------------------------------------------------------
//															Last Edited:2025/01/30
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

#include "manager_game_multiwindow.h"

#include "keyboard.h"
//=============================================================================
// ウィンドウ数４枚プレイヤー４人
//=============================================================================

MultiWindowGameManager::MultiWindowGameManager()
{
	m_pControllers.push_back(new Controller(0));
	m_pControllers.push_back(new Controller(1));
	m_pControllers.push_back(new Controller(2));
	m_pControllers.push_back(new Controller(3));

	m_GameState = GAMESTATE_STAGE_PREVIEW;
	m_DroneState = DRONE_CIRCLEVIEW;
	

	//船の数必ず二つ
	m_pShips.reserve(2);
	m_pShips.push_back(new Ship(0, XMFLOAT3(-25.0f, 0.0f, 0.0f), nullptr));
	m_pShips.push_back(new Ship(1, XMFLOAT3(25.0f, 0.0f, 0.0f), nullptr));

	//各プレイヤーに一つ
	m_pManagerUIs.push_back(new ManagerUI(m_pShips[0]));
	m_pManagerUIs.push_back(new ManagerUI(m_pShips[0]));
	m_pManagerUIs.push_back(new ManagerUI(m_pShips[1]));
	m_pManagerUIs.push_back(new ManagerUI(m_pShips[1]));

	//プレイヤーの数・・・マルチウィンドウは４人登録
	m_pShips[0]->RegisterPlayers(new Player(m_pShips[0], m_pControllers[0]	,0));//後々ここのプレイヤーID をコントローラー番号と統一する
	m_pShips[0]->RegisterPlayers(new Player(m_pShips[0], m_pControllers[1]  ,2));
	m_pShips[1]->RegisterPlayers(new Player(m_pShips[1], m_pControllers[2]  ,1));
	m_pShips[1]->RegisterPlayers(new Player(m_pShips[1], m_pControllers[3]  ,3));

	//それぞれの船に敵船の登録
	m_pShips[0]->SetEnemyShip(m_pShips[1]);
	m_pShips[1]->SetEnemyShip(m_pShips[0]);

	//カメラの作成（プレイヤーの人数分）
	m_pCameras.push_back(new PlayerCamera(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-25.0f, 0.0f, 0.0f), m_pShips[0], m_pShips[0]->GetPlayers()[0], GAMEMODE_MULTIWINDOW));
	m_pCameras.push_back(new PlayerCamera(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-25.0f, 0.0f, 10.0f), m_pShips[0], m_pShips[0]->GetPlayers()[1], GAMEMODE_MULTIWINDOW));
	m_pCameras.push_back(new PlayerCamera(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(25.0f, 0.0f, 0.0f), m_pShips[1], m_pShips[1]->GetPlayers()[0], GAMEMODE_MULTIWINDOW));
	m_pCameras.push_back(new PlayerCamera(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(25.0f, 0.0f, 10.0f), m_pShips[1], m_pShips[1]->GetPlayers()[1], GAMEMODE_MULTIWINDOW));

	//カメラポインターとプレイヤーの結びつけ
	m_pShips[0]->GetPlayers()[0]->SetCameraPointer(m_pCameras[0]);
	m_pShips[0]->GetPlayers()[1]->SetCameraPointer(m_pCameras[1]);
	m_pShips[1]->GetPlayers()[0]->SetCameraPointer(m_pCameras[2]);
	m_pShips[1]->GetPlayers()[1]->SetCameraPointer(m_pCameras[3]);

	m_MultiWindowUI = new SystemUI_Local_Manager(m_pShips);

	m_LocalEventManager = new LocalEventManager(m_pShips, m_pControllers);

	m_pShips[0]->GetPlayers()[0]->SetManagerUI(m_pManagerUIs[0]);
	m_pShips[0]->GetPlayers()[1]->SetManagerUI(m_pManagerUIs[1]);
	m_pShips[1]->GetPlayers()[0]->SetManagerUI(m_pManagerUIs[2]);
	m_pShips[1]->GetPlayers()[1]->SetManagerUI(m_pManagerUIs[3]);

	//海上旋回ドローン
	m_pDrones.push_back(new DroneCamera({ 0.0f, 0.0f , 0.0f }, DRONE_TYPE_CIRCLEVIEW));
}

MultiWindowGameManager::~MultiWindowGameManager()
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

	delete m_MultiWindowUI;
	m_MultiWindowUI = nullptr;

	delete m_LocalEventManager;
	m_LocalEventManager = nullptr;
}

void MultiWindowGameManager::Update()
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

void MultiWindowGameManager::Draw(int windownumber)
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

void MultiWindowGameManager::stagepreviewupdate()
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

void MultiWindowGameManager::countdownupdate()
{
	for (int i = 0; i < m_pShips.size(); i++) {
		if (m_pShips[i]) {
			m_pShips[i]->Update();
		}
	}
	for (int i = 0; i < m_pCameras.size(); i++)
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
	m_MultiWindowUI->Update();
}

void MultiWindowGameManager::playupdate()
{
	controllerupdate();

	//if (Keyboard_IsKeyTrigger(KK_T))// デバッグ用キーボード操作
	//{
	//	m_LocalEventManager->SetEvent();
	//	m_GameState = GAMESTATE_EVENT;
	//}

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


	for (int i = 0; i < m_pShips.size(); i++) {
		if (m_pShips[i]) {
			m_pShips[i]->Update();
		}
	}
	for (int i = 0; i < m_pCameras.size(); i++)
	{
		m_pCameras[i]->Update();
	}


	//船の総体力23100
	if (m_pShips[0]->GetShipHP() < 13100.0f/*2100.0f*/)// デバッグ用にHP10000以下
	{
		m_GameState = GAMESTATE_RESULT_ANNOUNCE;
		SetWhoWon(false);
		SEStart(GetAppSE(), CRI_SE_笛);
	}
	else if (m_pShips[1]->GetShipHP() < 13100.0f/*2100.0f*/)
	{
		m_GameState = GAMESTATE_RESULT_ANNOUNCE;
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
			m_GameState = GAMESTATE_RESULT_ANNOUNCE;
			SetWhoWon(true);
		}
		else
		{
			m_GameState = GAMESTATE_RESULT_ANNOUNCE;
			SetWhoWon(false);
		}
		//万が一引分けた場合のパターンをのちに追加

		SEStart(GetAppSE(), CRI_SE_笛);
	}

	////デバック用に強制終了
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
	m_MultiWindowUI->Update();
	m_MultiWindowUI->SetTimer(m_Timer);
	m_Timer = m_FrameCount / 60;
	m_FrameCount++;
}

void MultiWindowGameManager::eventupdate()
{
	controllerupdate();

	m_LocalEventManager->Update();

	if (m_LocalEventManager->GetEndFlag())
	{
		m_GameState = GAMESTATE_PLAY;
		m_LocalEventManager->DeleteEvent();
	}
}

void MultiWindowGameManager::resultannounceupdate()
{
	controllerupdate();

	for (int i = 0; i < m_pCameras.size(); i++)
	{
		m_pCameras[i]->Update();
	}


	

	for (int i = 0; i < m_pManagerUIs.size(); i++)
	{
		m_pManagerUIs[i]->Update();
	}

	m_MultiWindowUI->Update();

	ResultAnnounceUpdate(m_pControllers);

	if (GetResultAnnouncePushFlags()) {
		m_GameState = GAMESTATE_END;
	}
	
}

void MultiWindowGameManager::endupdate()
{
	if (GetFadeState() == FADE_STATE_FIXED)
	{
		FadeOut();

		SceneChange(SCENE_RESULT);
	}
}

void MultiWindowGameManager::stagepreviewdraw(int windownumber)
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

void MultiWindowGameManager::countdowndraw(int windownumber)
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

	m_MultiWindowUI->Draw();
}

void MultiWindowGameManager::playdraw(int windownumber)
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

	m_MultiWindowUI->Draw();
}

void MultiWindowGameManager::eventdraw(int windownumber)
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

	m_MultiWindowUI->Draw();

	GetSwapChains()[0]->SetViewport();
	// イベントのDraw();
	m_LocalEventManager->Draw(windownumber);
}

void MultiWindowGameManager::resultannouncedraw(int windownumber)
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

	m_MultiWindowUI->Draw();

	ResultAnnounceDraw();
}

void MultiWindowGameManager::enddraw(int windownumber)
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

	m_MultiWindowUI->Draw();

	ResultAnnounceDraw();
}

void MultiWindowGameManager::controllerupdate()
{
	for (auto itr : m_pControllers)
	{
		itr->Controller_Update();

		itr->Controller_UpdateVibration();
	}
}
