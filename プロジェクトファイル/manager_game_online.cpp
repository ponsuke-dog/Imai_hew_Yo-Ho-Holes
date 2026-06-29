//********************************************************************************
//
// ゲームマネージャーオンライン版[manager_game_online.h] 
// 
//															Author : Ryotaro Oyama
//															Date   :2025/02/10
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/10
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
#include "fade.h"
#include "ocean.h"
#include "skydome.h"
#include "manager_model.h"
#include "manager_field.h"
#include "manager_flotsam.h"
#include "manager_ui.h"
#include "manager_systemui.h"
#include "manager_effect.h"
#include "manager_event_local.h"
#include "manager_billboard.h"
#include "result.h"
#include "resultannounce.h"
#include "client.h"
#include "entry.h"
#include "cri.h"
#include "manager_game_online.h"
#include "player_bot.h"
#include "keyboard.h"
//=============================================================================
// ウィンドウ数４枚プレイヤー４人
//=============================================================================

OnlineGameManager::OnlineGameManager()
{
	m_Client = GetClient();

	m_pControllers.push_back(new Controller(0));


	m_GameState = GAMESTATE_STAGE_PREVIEW;
	m_DroneState = DRONE_CIRCLEVIEW;
	m_CameraType = CAMERATYPE_PLAYER;

	//船の数必ず二つ
	m_pShips.reserve(2);
	m_pShips.push_back(new Ship(0, XMFLOAT3(-25.0f, 0.0f, 0.0f), m_pControllers[0]));
	m_pShips.push_back(new Ship(1, XMFLOAT3(25.0f, 0.0f, 0.0f), nullptr));

	//UIマネージャー作成
	m_pManagerUIs.push_back(new ManagerUI(m_pShips[m_Client->GetMyTeamId()]));

	//プレイヤー登録

	m_pShips[m_Client->GetMyTeamId()]->RegisterPlayers(new Player(m_pShips[m_Client->GetMyTeamId()], m_pControllers.front(), m_Client->GetMyPlayerID()));

	//それぞれの船に敵船の登録
	m_pShips[0]->SetEnemyShip(m_pShips[1]);
	m_pShips[1]->SetEnemyShip(m_pShips[0]);

	//カメラの作成（プレイヤーの人数分）
	m_pCameras.push_back(new PlayerCamera(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-25.0f, 0.0f, 0.0f), m_pShips[m_Client->GetMyTeamId()], m_pShips[m_Client->GetMyTeamId()]->GetPlayers()[0], GAMEMODE_SOLO));
	

	//カメラポインターとプレイヤーの結びつけ
	m_pShips[m_Client->GetMyTeamId()]->GetPlayers()[0]->SetCameraPointer(m_pCameras[0]);

	m_MultiWindowUI = new SystemUI_Local_Manager(m_pShips);

	m_LocalEventManager = new LocalEventManager(m_pShips, m_pControllers);

	m_pShips[m_Client->GetMyTeamId()]->GetPlayers().front()->SetManagerUI(m_pManagerUIs.front());

	//海上旋回ドローン
	m_pDrones.push_back(new DroneCamera({ 0.0f, 0.0f , 0.0f }, DRONE_TYPE_CIRCLEVIEW));

	//描画用ボットプレイヤーの登録
	
	for (int playerid = 0; playerid < MAX_PLAYERS; playerid++) {
		if (playerid != m_Client->GetMyPlayerID()) {
			m_pShips[playerid % TEAM_COUNT]->RegisterPlayers(new BotPlayer(m_pShips[playerid % TEAM_COUNT], m_Client, playerid));
		}
	}
	

}

OnlineGameManager::~OnlineGameManager()
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
		if (m_pManagerUIs[i]) {
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

void OnlineGameManager::Update()
{
	

	

	switch (m_GameState)
	{
	case GAMESTATE_STAGE_PREVIEW:
		receive_and_update();
		stagepreviewupdate();
		break;
	case GAMESTATE_COUNTDOWN:
		receive_and_update();
		countdownupdate();
		break;
	case GAMESTATE_PLAY:
		receive_and_update();
		playupdate();
		break;
	case GAMESTATE_EVENT:
		receive_and_update();
		eventupdate();
		break;
	case GAMESTATE_RESULT_ANNOUNCE:
		//receive_and_update();
		resultannounceupdate();
		break;
	case GAMESTATE_END:
		endupdate();
		break;

	default:
		break;
	}
}

void OnlineGameManager::Draw(int windownumber)
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

void OnlineGameManager::stagepreviewupdate()
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

void OnlineGameManager::countdownupdate()
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

	//デバック用に強制終了
	if (Keyboard_IsKeyDown(KK_D0) && Keyboard_IsKeyTrigger(KK_D9))
	{
		m_GameState = GAMESTATE_END;
		SetWhoWon(false);
	}

	for (int i = 0; i < m_pManagerUIs.size(); i++)
	{
		m_pManagerUIs[i]->Update();
	}
	m_MultiWindowUI->Update();
	
}

void OnlineGameManager::playupdate()
{
	controllerupdate();


	for (int i = 0; i < m_pShips.size(); i++) {
		if (m_pShips[i]) {
			m_pShips[i]->Update();
		}
	}

	for (int i = 0; i < m_pCameras.size(); i++)
	{
		m_pCameras[i]->Update();
	}


	if (m_pShips[0]->GetShipHP() < 23100.0f - 7500.0f)// デバッグ用にHP10000以下
	{
		// バイブレーションを0に
		for (int i = 0; i < m_pControllers.size(); i++)
		{
			m_pControllers[i]->Controller_SetVibration(0, 0, 0);
		}
		m_GameState = GAMESTATE_RESULT_ANNOUNCE;
		DestroyClient();
		SetWhoWon(false);
		SEStart(GetAppSE(), CRI_SE_笛);
	}
	else if (m_pShips[1]->GetShipHP() < 23100.0f - 7500.0f)
	{
		// バイブレーションを0に
		for (int i = 0; i < m_pControllers.size(); i++)
		{
			m_pControllers[i]->Controller_SetVibration(0, 0, 0);
		}
		m_GameState = GAMESTATE_RESULT_ANNOUNCE;
		DestroyClient();
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
			DestroyClient();
			SetWhoWon(true);
		}
		else
		{
			m_GameState = GAMESTATE_RESULT_ANNOUNCE;
			DestroyClient();
			SetWhoWon(false);
		}
		//万が一引分けた場合のパターンをのちに追加

		SEStart(GetAppSE(), CRI_SE_笛);
	}

	//デバック用に強制終了
	if (Keyboard_IsKeyDown(KK_D0) && Keyboard_IsKeyTrigger(KK_D9))
	{
		m_GameState = GAMESTATE_END;
		SetWhoWon(false);
	}

	//デバックように強制ステート変更
	if (Keyboard_IsKeyDown(KK_D8) && Keyboard_IsKeyTrigger(KK_D7))
	{
		m_GameState = GAMESTATE_RESULT_ANNOUNCE;
	}


	if (Keyboard_IsKeyTrigger(KK_T))// デバッグ用キーボード操作
	{
		m_LocalEventManager->SetEvent();
		m_GameState = GAMESTATE_EVENT;
	}

	m_MultiWindowUI->Update();
	m_MultiWindowUI->SetTimer(m_Timer);
	
}

void OnlineGameManager::eventupdate()
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

void OnlineGameManager::resultannounceupdate()
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

void OnlineGameManager::endupdate()
{
	if (GetFadeState() == FADE_STATE_FIXED)
	{
		FadeOut();

		SceneChange(SCENE_RESULT);
	}
}

void OnlineGameManager::stagepreviewdraw(int windownumber)
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

void OnlineGameManager::countdowndraw(int windownumber)
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

void OnlineGameManager::playdraw(int windownumber)
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

void OnlineGameManager::eventdraw(int windownumber)
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

void OnlineGameManager::resultannouncedraw(int windownumber)
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

void OnlineGameManager::enddraw(int windownumber)
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

void OnlineGameManager::controllerupdate()
{
	m_pControllers[0]->Controller_Update();

	m_pControllers[0]->Controller_UpdateVibration();
}

void OnlineGameManager::receive_and_update()
{
	auto client = GetClient();

	m_FrameCount = client->GetGameScene().frame;
	m_Timer = client->GetGameScene().time;

	for (int i = 0; i < TEAM_COUNT; i++) {
		m_pShips[i]->SetShipHp(client->GetGameScene().ship_hp[i]);
	}

	//プレイヤーアクション
	if ((m_Client->GetUseReqest().object == m_Client->GetSaveReqest().object) && (m_Client->GetUseReqest().useid == m_Client->GetSaveReqest().useid)) {
		if (!m_Client->GetActionFlag() && m_Client->GetUseReqest().type == REQUEST_OK) {
			m_pShips[m_Client->GetMyTeamId()]->GetPlayers().front()->OnlineAction(m_Client->GetSaveReqest());
			m_Client->SetActionFlag(true);
			m_Client->ResetSaveReqest();
		}
		else if (!m_Client->GetActionFlag() && m_Client->GetUseReqest().type == REQUEST_NO) {
			m_Client->SetActionFlag(true);
			m_Client->ResetSaveReqest();
		}
	}

	//船上オブジェクト更新
	for (int teamid = 0; teamid < TEAM_NUM; teamid++) {
		auto actionobject = client->GetActionObjectData()[teamid];

		//Cannon使用フラグ切替
		for (int cannonnum = 0; cannonnum < 3; cannonnum++) {
			if (actionobject.cannon[cannonnum].use == USEFLAG_TRUE && !m_pShips[teamid]->GetCannons()[cannonnum]->GetCannonActionFlag()) {
				m_pShips[teamid]->GetCannons()[cannonnum]->SetCannonActionFlag(true);
			}
			else if (actionobject.cannon[cannonnum].use != USEFLAG_TRUE && m_pShips[teamid]->GetCannons()[cannonnum]->GetCannonActionFlag()) {
				m_pShips[teamid]->GetCannons()[cannonnum]->SetCannonActionFlag(false);
			}
		}

		//Gather使用フラグ切替
		for (int gathernum = 0; gathernum < 3; gathernum++) {
			if (actionobject.gather[gathernum].use == USEFLAG_TRUE && !m_pShips[teamid]->GetGathers()[gathernum]->GetUsingGather()) {
				m_pShips[teamid]->GetGathers()[gathernum]->SetUsingGather(true);
			}
			else if (actionobject.gather[gathernum].use != USEFLAG_TRUE && m_pShips[teamid]->GetGathers()[gathernum]->GetUsingGather()) {
				m_pShips[teamid]->GetGathers()[gathernum]->SetUsingGather(false);
			}
		}

		//Enhance使用フラグ切替
		if (actionobject.enhance.use == USEFLAG_TRUE && !m_pShips[teamid]->GetEnhance()->GetUsingEnhance()) {
			m_pShips[teamid]->GetEnhance()->SetUsingEnhance(true);
		}
		else if (actionobject.enhance.use == USEFLAG_FALSE && m_pShips[teamid]->GetEnhance()->GetUsingEnhance()) {
			m_pShips[teamid]->GetEnhance()->SetUsingEnhance(false);
		}

		//Enhanceレベル反映
		m_pShips[teamid]->GetEnhance()->SetCannonLevel(actionobject.enhance.cannonlevel);
		m_pShips[teamid]->GetEnhance()->SetGatherLevel(actionobject.enhance.gatherlevel);
		m_pShips[teamid]->GetEnhance()->SetRepairLevel(actionobject.enhance.repairlevel);

		//ストック反映
		for (auto storage : m_pShips[teamid]->GetStorages()) {
			if (storage->GetType() == STORAGETYPE_REPAIR) {
				storage->SetStock(actionobject.storage[STOCKTYPE_WOOD].stock);
			}
			else if (storage->GetType() == STORAGETYPE_CANNON) {
				storage->SetStock(actionobject.storage[STOCKTYPE_CANNON].stock);
			}
		}
		m_pShips[teamid]->GetEnhance()->SetStock(actionobject.enhance.stock);
	}

	//大砲発射
	if (!client->GetCannnonData().empty()) {

		auto cannnonball = client->GetCannnonData();
		for (int i = 0; i < cannnonball.size(); i++) {

			if (cannnonball[i].playerid != client->GetMyPlayerID()) {
				m_pShips[cannnonball[i].teamid]->GetCannons()[cannnonball[i].cannonid]->CreateCannonBall(cannnonball[i].endpoint);
			}
			else {
				m_pShips[cannnonball[i].teamid]->GetCannons()[cannnonball[i].cannonid]->CreateCannonBall(cannnonball[i].endpoint, m_pShips[client->GetMyTeamId()]->GetPlayers().front());
			}
		}

		client->GetCannnonData().clear();
	}

	//デッキブロック情報
	if (!client->GetBlockData().empty()) {
		//参照型autoじゃないとコピー扱いになる
		auto& blockdata = client->GetBlockData();

		for (int i = 0; i < blockdata.size(); i++) {
			if (blockdata[i].state == BLOCKSTATE_TRUE) {
				m_pShips[blockdata[i].teamid]->GetDeckBlocks()[blockdata[i].blocknumber]->Repair(blockdata[i].hp);
			}
			else if (blockdata[i].state == BLOCKSTATE_BREAK) {
				m_pShips[blockdata[i].teamid]->GetDeckBlocks()[blockdata[i].blocknumber]->Damage(blockdata[i]);
			}
		}

		blockdata.clear();
	}

	//イベント開始
	if (m_GameState == GAMESTATE_PLAY && client->GetGameScene().servergame == SERVERGAME_EVENT && client->GetEventData().scene != EVENTSCENE_END) {
		m_GameState = GAMESTATE_EVENT;
		m_LocalEventManager->SetEvent(client->GetEventData().type);
		m_Client->SetActionFlag(true);
	}

}
