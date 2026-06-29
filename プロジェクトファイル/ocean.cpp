//********************************************************************************
//
//	海の作成ヘッダー[ocean.h] 
// 
//															Author : Riugo Honda
//															Date   :2025/02/21
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/21
//********************************************************************************
#include "directx.h"
#include "configuration.h"
#include "scene.h"
#include "shader.h"
#include "camera.h"
#include "keyboard.h"
#include "manager_model.h"
#include "manager_game.h"

#include "ocean.h"

//********************************************************************************
//グローバル変数
//********************************************************************************
static XMFLOAT3 g_Position;//位置
static XMFLOAT3 g_Rotation;//回転
static XMFLOAT3 g_Scale;//拡縮

static GameManager* g_GameManager;

static MODEL* g_Model;


//********************************************************************************
//関数
//********************************************************************************

void OceanInitialize(GameManager* gamemanager)
{
	g_Position = { 0.0f, ( - 675.0f * 5), 0.0f};

	g_Rotation = { 0.0f, 0.0f , 0.0f };

	g_Scale = { 10.0f, 1.0f , 1.0f } ;

	g_GameManager = gamemanager;
}

void OceanFinalize(void)
{
}

void OceanUpdate(void)
{
	//g_Rotation.x = sinf(g_Rotation.x);

	if (g_GameManager->GetGameState() == GAMESTATE_STAGE_PREVIEW)
	{
		g_Position.z -= 2.0f;
	}

	if (g_GameManager->GetGameState() != GAMESTATE_STAGE_PREVIEW)
	{
		g_Position = { 0.0f, (-675.5f), 0.0f };

		g_Scale = { 10.0f, 1.0f , 1.0f };

		g_Rotation.x -= 0.0005f;
	}
	
}

void OceanDraw(void)
{
	if (g_GameManager->GetGameState() != GAMESTATE_STAGE_PREVIEW)
	{
		//ホイールモデルの海
		ModelDraw(MODELNAME_OCEAN, g_Position, g_Rotation, g_Scale);
	}
	else
	{
		//ドローン用のほぼ平面海
		ModelDraw(MODELNAME_FLATOCEAN, {0.0f , -8.0f ,g_Position.z}, g_Rotation, {g_Scale.x , g_Scale.y , g_Scale.z * 10.0f});
	}

}
