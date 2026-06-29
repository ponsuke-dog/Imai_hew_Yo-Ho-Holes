//********************************************************************************
//
//	‹ó‚Ìì¬ƒwƒbƒ_[[skydome.cpp] 
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
#include "manager_game.h"
#include "manager_model.h"

#include "skydome.h"


//********************************************************************************
//ƒOƒ[ƒoƒ‹•Ï”
//********************************************************************************
static XMFLOAT3 g_Position;//ˆÊ’u
static XMFLOAT3 g_Rotation;//‰ñ“]
static XMFLOAT3 g_Scale;//Šgk

static GameManager* m_pGameManager;
//********************************************************************************
//ŠÖ”
//********************************************************************************

void SkydomeInitialize(GameManager* gamemanager)
{
	g_Position = XMFLOAT3(0.0f, -20.0f, 0.0f);
	g_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_pGameManager = gamemanager;

	//g_Model = ModelLoad("asset\\model\\sky.fbx", 1);
}

void SkydomeFinalize(void)
{
}

void SkydomeUpdate(void)
{
	if (m_pGameManager->GetGameState() != GAMESTATE_STAGE_PREVIEW)
	{
		g_Rotation.y += 0.0005;
	}
	
}

void SkydomeDraw(void)
{
	if (m_pGameManager->GetGameState() == GAMESTATE_STAGE_PREVIEW)
	{
		ModelDrawNoLight(MODELNAME_SKYDOME,  g_Position, g_Rotation , g_Scale);
	}
	else
	{
		ModelDrawNoLight(MODELNAME_SKYDOME, { g_Position.x + g_Scale.x - 50.0f, g_Position.y , g_Position.z }, { g_Rotation.x , g_Rotation.y , g_Rotation.z }, g_Scale);
		ModelDrawNoLight(MODELNAME_SKYDOME, { g_Position.x - g_Scale.x + 50.0f, g_Position.y , g_Position.z }, { g_Rotation.x , -g_Rotation.y , g_Rotation.z }, g_Scale);
	}
}
