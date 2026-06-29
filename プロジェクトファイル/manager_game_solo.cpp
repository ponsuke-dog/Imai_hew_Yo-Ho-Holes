//********************************************************************************
//
// ゲームマネージャー[manager_game.cpp] 
// 
//															Author : Ponsuke
//															Date   :2024/12/08
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/08
//********************************************************************************
#include "object_3d.h"
#include "player.h"
#include "deckblock.h"
#include "effect_3d.h"
#include "controller.h"
#include "ship.h"
#include "camera_drone.h"
#include "camera_player.h"
#include "fade.h"
#include "scene.h"
#include "result.h"
#include "resultannounce.h"
#include "ocean.h"
#include "skydome.h"
#include "keyboard.h"
#include "manager_model.h"
#include "manager_field.h"
#include "manager_flotsam.h"
#include "manager_ui.h"
#include "manager_systemui.h"
#include "manager_effect.h"
#include "manager_billboard.h"

#include "cri.h"

#include "manager_game_solo.h"


GameManagerSolo::GameManagerSolo()
{
	m_pControllers.push_back(new Controller(0));

	//m_pCamera = new PlayerCamera(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-25.0f, 0.0f, 0.0f));

	m_GameState = GAMESTATE_STAGE_PREVIEW;
	m_DroneState = DRONE_CIRCLEVIEW;
	m_CameraType = CAMERATYPE_PLAYER;


	m_pShips.reserve(2);
	m_pShips.push_back(new Ship(0, XMFLOAT3(-25.0f, 0.0f, 0.0f), m_pControllers[0]));
	m_pShips.push_back(new Ship(1, XMFLOAT3(25.0f, 0.0f, 0.0f), nullptr));

	m_pManagerUIs.push_back(new ManagerUI(m_pShips[0]));

	m_pShips[0]->RegisterPlayers(new Player(m_pShips[0], m_pControllers[0], 0));

	m_pShips[0]->SetEnemyShip(m_pShips[1]);
	m_pShips[1]->SetEnemyShip(m_pShips[0]);

	m_pCameras.push_back(new PlayerCamera(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-25.0f, 0.0f, 0.0f), m_pShips[0], m_pShips[0]->GetPlayers()[0], GAMEMODE_SOLO));

	m_pShips[0]->GetPlayers()[0]->SetCameraPointer(m_pCameras[0]);

	m_pShips[0]->GetPlayers()[0]->SetManagerUI(m_pManagerUIs[0]);


	m_MultiWindowUI = new SystemUI_Local_Manager(m_pShips);
	//海上旋回ドローン
	m_pDrones.push_back(new DroneCamera({ 0.0f, 0.0f , 0.0f }, DRONE_TYPE_CIRCLEVIEW));
}

GameManagerSolo::~GameManagerSolo()
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

}

void GameManagerSolo::Update()
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

void GameManagerSolo::Draw(int windownumber)
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

int GameManagerSolo::GetScore()
{
	return m_Score;
}

void GameManagerSolo::stagepreviewupdate()
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

void GameManagerSolo::countdownupdate()
{
	for (int i = 0; i < m_pShips.size(); i++) {
		if (m_pShips[i]) {
			m_pShips[i]->Update();
		}
	}
	m_pCameras[0]->Update();

	//デバック用に強制終了
	if (Keyboard_IsKeyDown(KK_D0) && Keyboard_IsKeyTrigger(KK_D9))
	{
		m_GameState = GAMESTATE_END;
		SetWhoWon(true);
	}

	m_pManagerUIs[0]->Update();

	m_MultiWindowUI->Update();
	
}

void GameManagerSolo::playupdate()
{
	controllerupdate();

	if (m_Timer >= 60)
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
			SEStart(GetAppSE(), CRI_SE_笛);
		}
		else
		{
			m_GameState = GAMESTATE_RESULT_ANNOUNCE;
			SetWhoWon(false);
			SEStart(GetAppSE(), CRI_SE_笛);
		}
		//万が一引分けた場合のパターンをのちに追加

		SEStart(GetAppSE(), CRI_SE_笛);
	}

	for (int i = 0; i < m_pShips.size(); i++) {
		if (m_pShips[i]) {
			m_pShips[i]->Update();
		}
	}

	m_pCameras[0]->Update();

	//デバック用に強制終了
	if (Keyboard_IsKeyDown(KK_D0) && Keyboard_IsKeyTrigger(KK_D9))
	{
		m_GameState = GAMESTATE_END;
		SetWhoWon(true);
	}

	m_pManagerUIs[0]->Update();

	m_MultiWindowUI->Update();
	m_MultiWindowUI->SetTimer(m_Timer);
	m_Timer = m_FrameCount / 60;
	m_FrameCount++;
}

void GameManagerSolo::resultannounceupdate()
{
	controllerupdate();

	m_pCameras[0]->Update();

	m_pManagerUIs[0]->Update();
	m_MultiWindowUI->Update();


	ResultAnnounceUpdate(m_pControllers);

	if (GetResultAnnouncePushFlags()) {
		m_GameState = GAMESTATE_END;
	}
}

void GameManagerSolo::endupdate()
{
	if (GetFadeState() == FADE_STATE_FIXED)
	{
		// 念のため、自分の船にとって相手の船の甲板のサイズ分ループ
		for (int i = 0; i < m_pShips[0]->GetEnemyShip()->GetDeckBlocks().size(); i++)
		{
			if (m_pShips[0]->GetEnemyShip()->GetDeckBlocks()[i]->GetDeckType() == DECKTYPE_WOOD &&
				m_pShips[0]->GetEnemyShip()->GetDeckBlocks()[i]->GetUse() == false)
			{
				m_Score++;
			}
		}

		// リザルトにスコアのデータを格納
		SetScore(m_Score);


		FadeOut();

		SceneChange(SCENE_RESULT);
	}
}

void GameManagerSolo::stagepreviewdraw(int windownumber)
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

void GameManagerSolo::countdowndraw(int windownumber)
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

void GameManagerSolo::playdraw(int window)
{
	m_pCameras[window]->Draw();
	SetView(m_pCameras[window]->GetCameraView());
	SetProjection(m_pCameras[window]->GetCameraProjection());
	SetEffectProjection(m_pCameras[window]->GetCameraProjection());
	SetEffectView(m_pCameras[window]->GetCameraView());
	SetBillboardProjection(m_pCameras[window]->GetCameraProjection());
	SetBillboardView(m_pCameras[window]->GetCameraView());

	OceanDraw();
	SkydomeDraw();

	for (int i = 0; i < m_pShips.size(); i++) {
		if (m_pShips[i]) {
			m_pShips[i]->Draw();
		}
	}
	EffectManagerDraw();
	BillBoradManagerDraw();

	m_pManagerUIs[window]->Draw();

	m_MultiWindowUI->Draw();
}

void GameManagerSolo::resultannouncedraw(int windownumber)
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

void GameManagerSolo::enddraw(int windownumber)
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

void GameManagerSolo::controllerupdate()
{
	m_pControllers[0]->Controller_Update();

	m_pControllers[0]->Controller_UpdateVibration();
}

