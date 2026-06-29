//********************************************************************************
//
// モデルマネージャー[manager_model.h] 
// 
//															Author : Ponsuke
//															Date   :2024/12/08
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/08
//********************************************************************************
		
#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "directx.h"
#include <vector>

struct MODEL;

class Camera;

typedef enum {
	MODELNAME_NONE = -1,


	MODELNAME_PLAYER,
	MODELNAME_DECKBLOCK,
	MODELNAME_CANNON,
	MODELNAME_CANNONBALL,
	MODELNAME_EXPLOSION,
	MODELNAME_GATHER,
	MODELNAME_NET,
	MODELNAME_NET_RISE,
	MODELNAME_AIM,
	MODELNAME_STORAGE_CANNON,
	MODELNAME_STORAGE_WOOD,
	MODELNAME_FLOTSAM_WOOD,
	MODELNAME_FLOTSAM_BALL,
	MODELNAME_FLOTSAM_ENHANCE,
	MODELNAME_ENHANCE,

	MODELNAME_SHIP,
	MODELNAME_SKYDOME,
	MODELNAME_SHADOW,
	MODELNAME_FLATOCEAN,
	MODELNAME_OCEAN,

	MODELNAME_MAX,
}ModelName;

typedef enum {
	PLAYERTYPE_NONE = -1,


	PLAYERTYPE_TAKO,			// 0
	PLAYERTYPE_KAJIKI,			// 1
	PLAYERTYPE_OUMU,			// 2
	PLAYERTYPE_CAT,				// 3

	PLAYERTYPE_MAX,
}PlayerType;

typedef enum {
	MODELPLAYER_NONE = -1,		


	MODELPLAYER_IDLE,			// 0
	MODELPLAYER_RUN,			// 1
	MODELPLAYER_CARRYBALL,		// 2
	MODELPLAYER_IDLE_CARRYBALL,	// 3
	MODELPLAYER_CARRYWOOD,		// 4
	MODELPLAYER_IDLE_CARRYWOOD,	// 5
	MODELPLAYER_SHOT,			// 6
	MODELPLAYER_REPAIR,			// 7
	
	MODELPLAYER_MAX,

	MODELPLAYER_HOLD,			//プレイヤーモデルのロードの関係上、MODELPLAYER_MAXの後に記載
	MODELPLAYER_BLOWOFF,

}ModelPlayer;

void	ModelManagerInitialize();
void	ModelManagerFinalize();

void	ModelDraw(ModelName name, XMFLOAT3 pos, XMFLOAT3 rot = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 size = XMFLOAT3(1.0f, 1.0f, 1.0f));

void	ModelDrawNoLight(ModelName name, XMFLOAT3 pos, XMFLOAT3 rot = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 size = XMFLOAT3(1.0f, 1.0f, 1.0f));


//プレイヤー用
void	ModelDraw(PlayerType type, ModelPlayer name, int AnimationNum, XMFLOAT3 pos, XMFLOAT3 rot = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 size = XMFLOAT3(1.0f, 1.0f, 1.0f));


MODEL* GetModel(ModelName name);


void SetView(XMMATRIX view);
void SetProjection(XMMATRIX projection);

#endif // !MODEL_MANAGER_H
