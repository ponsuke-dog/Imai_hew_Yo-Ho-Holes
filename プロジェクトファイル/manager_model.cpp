//********************************************************************************
//
// モデルマネージャー[manager_model.cpp] 
// 
//															Author : Ponsuke
//															Date   :2024/12/08
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/08
//********************************************************************************
#include "model.h"
#include "camera.h"
#include "shader.h"
#include "manager_game.h"
#include "scene.h"
#include "manager_model.h"

static MODEL* g_Model[MODELNAME_MAX] = {};
static MODEL* g_PlayerModel[PLAYERTYPE_MAX][MODELPLAYER_MAX][8];
static MODEL* g_PlayerHoldModel[PLAYERTYPE_MAX];
static MODEL* g_PlayerBlowOffModel;

// 一旦グローバル変数
XMMATRIX g_view, g_projection;

void ModelManagerInitialize()
{
	// モデルのロード
	LoadDraw();

	// プレイヤー関連モデル
	g_Model[MODELNAME_PLAYER] = ModelLoad("asset\\model\\player.fbx");

	// デッキブロック関連モデル
	g_Model[MODELNAME_DECKBLOCK] = ModelLoad("asset\\model\\Deckblock.fbx");
	
	// 大砲関連モデル
	g_Model[MODELNAME_CANNON] = ModelLoad("asset\\model\\Cannon.fbx");
	g_Model[MODELNAME_CANNONBALL] = ModelLoad("asset\\model\\cannon_ball_01.fbx");
	g_Model[MODELNAME_EXPLOSION] = ModelLoad("asset\\model\\cannon.fbx");

	// 収集関連モデル
	g_Model[MODELNAME_GATHER] = ModelLoad("asset\\model\\Gather.fbx");
	g_Model[MODELNAME_NET] = ModelLoad("asset\\model\\Ami_01.fbx");	
	g_Model[MODELNAME_NET_RISE] = ModelLoad("asset\\model\\Net_Rise.fbx");
	g_Model[MODELNAME_AIM] = ModelLoad("asset\\model\\pointer_fix_01.fbx");

	// ストレージ関連モデル
	g_Model[MODELNAME_STORAGE_WOOD] = ModelLoad("asset\\model\\Storage_Wood.fbx");
	g_Model[MODELNAME_STORAGE_CANNON] = ModelLoad("asset\\model\\Storage_Cannon.fbx");

	// 漂流物関連モデル
	g_Model[MODELNAME_FLOTSAM_WOOD] = ModelLoad("asset\\model\\Flotsam_Wood.fbx");	// 木材
	g_Model[MODELNAME_FLOTSAM_BALL] = ModelLoad("asset\\model\\Flotsam_Cannon.fbx");		// 砲弾
	g_Model[MODELNAME_FLOTSAM_ENHANCE] = ModelLoad("asset\\model\\Flotsam_Enhance.fbx");	// 強化

	// 船(飾り)関連のモデル
	g_Model[MODELNAME_SHIP] = ModelLoad("asset\\model\\ship_hata02.fbx");

	//強化施設モデル
	g_Model[MODELNAME_ENHANCE] = ModelLoad("asset\\model\\Enhance.fbx");

	//空のモデル
	g_Model[MODELNAME_SKYDOME] = ModelLoad("asset\\model\\sky_shiage_03.fbx");

	//影モデル
	g_Model[MODELNAME_SHADOW] = ModelLoad("asset\\model\\shadow.fbx");

	//海モデル
	g_Model[MODELNAME_FLATOCEAN] = ModelLoad("asset\\model\\nami_shiage_b_02.fbx");
	g_Model[MODELNAME_OCEAN] = ModelLoad("asset\\model\\nami_shiage_04.fbx");


	LoadDraw();
	//たこ関連モデル
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE][0] = ModelLoad("asset\\model\\tako\\idle\\tako_idle_a_01.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE][1] = ModelLoad("asset\\model\\tako\\idle\\tako_idle_a_02.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE][2] = ModelLoad("asset\\model\\tako\\idle\\tako_idle_a_03.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE][3] = ModelLoad("asset\\model\\tako\\idle\\tako_idle_a_04.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE][4] = ModelLoad("asset\\model\\tako\\idle\\tako_idle_a_05.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE][5] = ModelLoad("asset\\model\\tako\\idle\\tako_idle_a_06.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE][6] = ModelLoad("asset\\model\\tako\\idle\\tako_idle_a_07.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE][7] = ModelLoad("asset\\model\\tako\\idle\\tako_idle_a_08.fbx");

	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_RUN][0] = ModelLoad("asset\\model\\tako\\run\\tako_walk_a_01.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_RUN][1] = ModelLoad("asset\\model\\tako\\run\\tako_walk_a_02.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_RUN][2] = ModelLoad("asset\\model\\tako\\run\\tako_walk_a_03.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_RUN][3] = ModelLoad("asset\\model\\tako\\run\\tako_walk_a_04.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_RUN][4] = ModelLoad("asset\\model\\tako\\run\\tako_walk_a_05.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_RUN][5] = ModelLoad("asset\\model\\tako\\run\\tako_walk_a_06.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_RUN][6] = ModelLoad("asset\\model\\tako\\run\\tako_walk_a_07.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_RUN][7] = ModelLoad("asset\\model\\tako\\run\\tako_walk_a_08.fbx");

	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYBALL][0] = ModelLoad("asset\\model\\tako\\carry_ball\\tako_walk_b_01.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYBALL][1] = ModelLoad("asset\\model\\tako\\carry_ball\\tako_walk_b_02.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYBALL][2] = ModelLoad("asset\\model\\tako\\carry_ball\\tako_walk_b_03.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYBALL][3] = ModelLoad("asset\\model\\tako\\carry_ball\\tako_walk_b_04.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYBALL][4] = ModelLoad("asset\\model\\tako\\carry_ball\\tako_walk_b_05.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYBALL][5] = ModelLoad("asset\\model\\tako\\carry_ball\\tako_walk_b_06.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYBALL][6] = ModelLoad("asset\\model\\tako\\carry_ball\\tako_walk_b_07.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYBALL][7] = ModelLoad("asset\\model\\tako\\carry_ball\\tako_walk_b_08.fbx");

	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYBALL][0] = ModelLoad("asset\\model\\tako\\idle_ball\\tako_idle_b_01.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYBALL][1] = ModelLoad("asset\\model\\tako\\idle_ball\\tako_idle_b_02.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYBALL][2] = ModelLoad("asset\\model\\tako\\idle_ball\\tako_idle_b_03.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYBALL][3] = ModelLoad("asset\\model\\tako\\idle_ball\\tako_idle_b_04.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYBALL][4] = ModelLoad("asset\\model\\tako\\idle_ball\\tako_idle_b_05.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYBALL][5] = ModelLoad("asset\\model\\tako\\idle_ball\\tako_idle_b_06.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYBALL][6] = ModelLoad("asset\\model\\tako\\idle_ball\\tako_idle_b_07.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYBALL][7] = ModelLoad("asset\\model\\tako\\idle_ball\\tako_idle_b_08.fbx");

	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYWOOD][0] = ModelLoad("asset\\model\\tako\\carry_wood\\tako_walk_c_01.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYWOOD][1] = ModelLoad("asset\\model\\tako\\carry_wood\\tako_walk_c_02.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYWOOD][2] = ModelLoad("asset\\model\\tako\\carry_wood\\tako_walk_c_03.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYWOOD][3] = ModelLoad("asset\\model\\tako\\carry_wood\\tako_walk_c_04.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYWOOD][4] = ModelLoad("asset\\model\\tako\\carry_wood\\tako_walk_c_05.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYWOOD][5] = ModelLoad("asset\\model\\tako\\carry_wood\\tako_walk_c_06.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYWOOD][6] = ModelLoad("asset\\model\\tako\\carry_wood\\tako_walk_c_07.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_CARRYWOOD][7] = ModelLoad("asset\\model\\tako\\carry_wood\\tako_walk_c_08.fbx");

	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYWOOD][0] = ModelLoad("asset\\model\\tako\\idle_wood\\tako_idle_c_01.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYWOOD][1] = ModelLoad("asset\\model\\tako\\idle_wood\\tako_idle_c_02.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYWOOD][2] = ModelLoad("asset\\model\\tako\\idle_wood\\tako_idle_c_03.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYWOOD][3] = ModelLoad("asset\\model\\tako\\idle_wood\\tako_idle_c_04.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYWOOD][4] = ModelLoad("asset\\model\\tako\\idle_wood\\tako_idle_c_05.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYWOOD][5] = ModelLoad("asset\\model\\tako\\idle_wood\\tako_idle_c_06.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYWOOD][6] = ModelLoad("asset\\model\\tako\\idle_wood\\tako_idle_c_07.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_IDLE_CARRYWOOD][7] = ModelLoad("asset\\model\\tako\\idle_wood\\tako_idle_c_08.fbx");

	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_SHOT][0] = ModelLoad("asset\\model\\tako\\attack\\tako_attack_01.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_SHOT][1] = ModelLoad("asset\\model\\tako\\attack\\tako_attack_02.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_SHOT][2] = ModelLoad("asset\\model\\tako\\attack\\tako_attack_03.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_SHOT][3] = ModelLoad("asset\\model\\tako\\attack\\tako_attack_04.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_SHOT][4] = ModelLoad("asset\\model\\tako\\attack\\tako_attack_05.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_SHOT][5] = ModelLoad("asset\\model\\tako\\attack\\tako_attack_06.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_SHOT][6] = ModelLoad("asset\\model\\tako\\attack\\tako_attack_07.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_SHOT][7] = ModelLoad("asset\\model\\tako\\attack\\tako_attack_08.fbx");


	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_REPAIR][0] = ModelLoad("asset\\model\\tako\\repair\\tako_fix_01.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_REPAIR][1] = ModelLoad("asset\\model\\tako\\repair\\tako_fix_02.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_REPAIR][2] = ModelLoad("asset\\model\\tako\\repair\\tako_fix_03.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_REPAIR][3] = ModelLoad("asset\\model\\tako\\repair\\tako_fix_04.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_REPAIR][4] = ModelLoad("asset\\model\\tako\\repair\\tako_fix_05.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_REPAIR][5] = ModelLoad("asset\\model\\tako\\repair\\tako_fix_06.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_REPAIR][6] = ModelLoad("asset\\model\\tako\\repair\\tako_fix_07.fbx");
	g_PlayerModel[PLAYERTYPE_TAKO][MODELPLAYER_REPAIR][7] = ModelLoad("asset\\model\\tako\\repair\\tako_fix_08.fbx");


	LoadDraw();
	//かじき関連モデル
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE][0] = ModelLoad("asset\\model\\kajiki\\idle\\kajiki_idle_a_01.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE][1] = ModelLoad("asset\\model\\kajiki\\idle\\kajiki_idle_a_02.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE][2] = ModelLoad("asset\\model\\kajiki\\idle\\kajiki_idle_a_03.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE][3] = ModelLoad("asset\\model\\kajiki\\idle\\kajiki_idle_a_04.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE][4] = ModelLoad("asset\\model\\kajiki\\idle\\kajiki_idle_a_05.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE][5] = ModelLoad("asset\\model\\kajiki\\idle\\kajiki_idle_a_06.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE][6] = ModelLoad("asset\\model\\kajiki\\idle\\kajiki_idle_a_07.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE][7] = ModelLoad("asset\\model\\kajiki\\idle\\kajiki_idle_a_08.fbx");

	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_RUN][0] = ModelLoad("asset\\model\\kajiki\\run\\kajiki_walk_a_01.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_RUN][1] = ModelLoad("asset\\model\\kajiki\\run\\kajiki_walk_a_02.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_RUN][2] = ModelLoad("asset\\model\\kajiki\\run\\kajiki_walk_a_03.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_RUN][3] = ModelLoad("asset\\model\\kajiki\\run\\kajiki_walk_a_04.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_RUN][4] = ModelLoad("asset\\model\\kajiki\\run\\kajiki_walk_a_05.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_RUN][5] = ModelLoad("asset\\model\\kajiki\\run\\kajiki_walk_a_06.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_RUN][6] = ModelLoad("asset\\model\\kajiki\\run\\kajiki_walk_a_07.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_RUN][7] = ModelLoad("asset\\model\\kajiki\\run\\kajiki_walk_a_08.fbx");

	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYBALL][0] = ModelLoad("asset\\model\\kajiki\\carry_ball\\kajiki_walk_b_01.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYBALL][1] = ModelLoad("asset\\model\\kajiki\\carry_ball\\kajiki_walk_b_02.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYBALL][2] = ModelLoad("asset\\model\\kajiki\\carry_ball\\kajiki_walk_b_03.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYBALL][3] = ModelLoad("asset\\model\\kajiki\\carry_ball\\kajiki_walk_b_04.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYBALL][4] = ModelLoad("asset\\model\\kajiki\\carry_ball\\kajiki_walk_b_05.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYBALL][5] = ModelLoad("asset\\model\\kajiki\\carry_ball\\kajiki_walk_b_06.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYBALL][6] = ModelLoad("asset\\model\\kajiki\\carry_ball\\kajiki_walk_b_07.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYBALL][7] = ModelLoad("asset\\model\\kajiki\\carry_ball\\kajiki_walk_b_08.fbx");

	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYBALL][0] = ModelLoad("asset\\model\\kajiki\\idle_ball\\kajiki_idle_b_01.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYBALL][1] = ModelLoad("asset\\model\\kajiki\\idle_ball\\kajiki_idle_b_02.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYBALL][2] = ModelLoad("asset\\model\\kajiki\\idle_ball\\kajiki_idle_b_03.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYBALL][3] = ModelLoad("asset\\model\\kajiki\\idle_ball\\kajiki_idle_b_04.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYBALL][4] = ModelLoad("asset\\model\\kajiki\\idle_ball\\kajiki_idle_b_05.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYBALL][5] = ModelLoad("asset\\model\\kajiki\\idle_ball\\kajiki_idle_b_06.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYBALL][6] = ModelLoad("asset\\model\\kajiki\\idle_ball\\kajiki_idle_b_07.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYBALL][7] = ModelLoad("asset\\model\\kajiki\\idle_ball\\kajiki_idle_b_08.fbx");

	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYWOOD][0] = ModelLoad("asset\\model\\kajiki\\carry_wood\\kajiki_walk_c_01.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYWOOD][1] = ModelLoad("asset\\model\\kajiki\\carry_wood\\kajiki_walk_c_02.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYWOOD][2] = ModelLoad("asset\\model\\kajiki\\carry_wood\\kajiki_walk_c_03.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYWOOD][3] = ModelLoad("asset\\model\\kajiki\\carry_wood\\kajiki_walk_c_04.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYWOOD][4] = ModelLoad("asset\\model\\kajiki\\carry_wood\\kajiki_walk_c_05.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYWOOD][5] = ModelLoad("asset\\model\\kajiki\\carry_wood\\kajiki_walk_c_06.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYWOOD][6] = ModelLoad("asset\\model\\kajiki\\carry_wood\\kajiki_walk_c_07.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_CARRYWOOD][7] = ModelLoad("asset\\model\\kajiki\\carry_wood\\kajiki_walk_c_08.fbx");

	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYWOOD][0] = ModelLoad("asset\\model\\kajiki\\idle_wood\\kajiki_idle_c_01.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYWOOD][1] = ModelLoad("asset\\model\\kajiki\\idle_wood\\kajiki_idle_c_02.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYWOOD][2] = ModelLoad("asset\\model\\kajiki\\idle_wood\\kajiki_idle_c_03.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYWOOD][3] = ModelLoad("asset\\model\\kajiki\\idle_wood\\kajiki_idle_c_04.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYWOOD][4] = ModelLoad("asset\\model\\kajiki\\idle_wood\\kajiki_idle_c_05.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYWOOD][5] = ModelLoad("asset\\model\\kajiki\\idle_wood\\kajiki_idle_c_06.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYWOOD][6] = ModelLoad("asset\\model\\kajiki\\idle_wood\\kajiki_idle_c_07.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_IDLE_CARRYWOOD][7] = ModelLoad("asset\\model\\kajiki\\idle_wood\\kajiki_idle_c_08.fbx");

	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_SHOT][0] = ModelLoad("asset\\model\\kajiki\\attack\\kajiki_attack_01.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_SHOT][1] = ModelLoad("asset\\model\\kajiki\\attack\\kajiki_attack_02.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_SHOT][2] = ModelLoad("asset\\model\\kajiki\\attack\\kajiki_attack_03.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_SHOT][3] = ModelLoad("asset\\model\\kajiki\\attack\\kajiki_attack_04.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_SHOT][4] = ModelLoad("asset\\model\\kajiki\\attack\\kajiki_attack_05.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_SHOT][5] = ModelLoad("asset\\model\\kajiki\\attack\\kajiki_attack_06.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_SHOT][6] = ModelLoad("asset\\model\\kajiki\\attack\\kajiki_attack_07.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_SHOT][7] = ModelLoad("asset\\model\\kajiki\\attack\\kajiki_attack_08.fbx");

	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_REPAIR][0] = ModelLoad("asset\\model\\kajiki\\repair\\kajiki_fix_01.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_REPAIR][1] = ModelLoad("asset\\model\\kajiki\\repair\\kajiki_fix_02.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_REPAIR][2] = ModelLoad("asset\\model\\kajiki\\repair\\kajiki_fix_03.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_REPAIR][3] = ModelLoad("asset\\model\\kajiki\\repair\\kajiki_fix_04.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_REPAIR][4] = ModelLoad("asset\\model\\kajiki\\repair\\kajiki_fix_05.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_REPAIR][5] = ModelLoad("asset\\model\\kajiki\\repair\\kajiki_fix_06.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_REPAIR][6] = ModelLoad("asset\\model\\kajiki\\repair\\kajiki_fix_07.fbx");
	g_PlayerModel[PLAYERTYPE_KAJIKI][MODELPLAYER_REPAIR][7] = ModelLoad("asset\\model\\kajiki\\repair\\kajiki_fix_08.fbx");

	LoadDraw();
	//オウム関連モデル
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE][0] = ModelLoad("asset\\model\\oumu\\idle\\oumu_idle_a_01.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE][1] = ModelLoad("asset\\model\\oumu\\idle\\oumu_idle_a_02.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE][2] = ModelLoad("asset\\model\\oumu\\idle\\oumu_idle_a_03.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE][3] = ModelLoad("asset\\model\\oumu\\idle\\oumu_idle_a_04.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE][4] = ModelLoad("asset\\model\\oumu\\idle\\oumu_idle_a_05.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE][5] = ModelLoad("asset\\model\\oumu\\idle\\oumu_idle_a_06.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE][6] = ModelLoad("asset\\model\\oumu\\idle\\oumu_idle_a_07.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE][7] = ModelLoad("asset\\model\\oumu\\idle\\oumu_idle_a_08.fbx");

	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_RUN][0] = ModelLoad("asset\\model\\oumu\\run\\oumu_walk_a_01.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_RUN][1] = ModelLoad("asset\\model\\oumu\\run\\oumu_walk_a_02.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_RUN][2] = ModelLoad("asset\\model\\oumu\\run\\oumu_walk_a_03.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_RUN][3] = ModelLoad("asset\\model\\oumu\\run\\oumu_walk_a_04.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_RUN][4] = ModelLoad("asset\\model\\oumu\\run\\oumu_walk_a_05.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_RUN][5] = ModelLoad("asset\\model\\oumu\\run\\oumu_walk_a_06.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_RUN][6] = ModelLoad("asset\\model\\oumu\\run\\oumu_walk_a_07.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_RUN][7] = ModelLoad("asset\\model\\oumu\\run\\oumu_walk_a_08.fbx");

	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYBALL][0] = ModelLoad("asset\\model\\oumu\\carry_ball\\oumu_walk_b_01.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYBALL][1] = ModelLoad("asset\\model\\oumu\\carry_ball\\oumu_walk_b_02.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYBALL][2] = ModelLoad("asset\\model\\oumu\\carry_ball\\oumu_walk_b_03.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYBALL][3] = ModelLoad("asset\\model\\oumu\\carry_ball\\oumu_walk_b_04.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYBALL][4] = ModelLoad("asset\\model\\oumu\\carry_ball\\oumu_walk_b_05.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYBALL][5] = ModelLoad("asset\\model\\oumu\\carry_ball\\oumu_walk_b_06.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYBALL][6] = ModelLoad("asset\\model\\oumu\\carry_ball\\oumu_walk_b_07.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYBALL][7] = ModelLoad("asset\\model\\oumu\\carry_ball\\oumu_walk_b_08.fbx");

	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYBALL][0] = ModelLoad("asset\\model\\oumu\\idle_ball\\oumu_idle_b_01.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYBALL][1] = ModelLoad("asset\\model\\oumu\\idle_ball\\oumu_idle_b_02.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYBALL][2] = ModelLoad("asset\\model\\oumu\\idle_ball\\oumu_idle_b_03.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYBALL][3] = ModelLoad("asset\\model\\oumu\\idle_ball\\oumu_idle_b_04.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYBALL][4] = ModelLoad("asset\\model\\oumu\\idle_ball\\oumu_idle_b_05.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYBALL][5] = ModelLoad("asset\\model\\oumu\\idle_ball\\oumu_idle_b_06.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYBALL][6] = ModelLoad("asset\\model\\oumu\\idle_ball\\oumu_idle_b_07.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYBALL][7] = ModelLoad("asset\\model\\oumu\\idle_ball\\oumu_idle_b_08.fbx");

	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYWOOD][0] = ModelLoad("asset\\model\\oumu\\carry_wood\\oumu_walk_c_01.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYWOOD][1] = ModelLoad("asset\\model\\oumu\\carry_wood\\oumu_walk_c_02.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYWOOD][2] = ModelLoad("asset\\model\\oumu\\carry_wood\\oumu_walk_c_03.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYWOOD][3] = ModelLoad("asset\\model\\oumu\\carry_wood\\oumu_walk_c_04.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYWOOD][4] = ModelLoad("asset\\model\\oumu\\carry_wood\\oumu_walk_c_05.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYWOOD][5] = ModelLoad("asset\\model\\oumu\\carry_wood\\oumu_walk_c_06.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYWOOD][6] = ModelLoad("asset\\model\\oumu\\carry_wood\\oumu_walk_c_07.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_CARRYWOOD][7] = ModelLoad("asset\\model\\oumu\\carry_wood\\oumu_walk_c_08.fbx");

	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYWOOD][0] = ModelLoad("asset\\model\\oumu\\idle_wood\\oumu_idle_c_01.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYWOOD][1] = ModelLoad("asset\\model\\oumu\\idle_wood\\oumu_idle_c_02.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYWOOD][2] = ModelLoad("asset\\model\\oumu\\idle_wood\\oumu_idle_c_03.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYWOOD][3] = ModelLoad("asset\\model\\oumu\\idle_wood\\oumu_idle_c_04.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYWOOD][4] = ModelLoad("asset\\model\\oumu\\idle_wood\\oumu_idle_c_05.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYWOOD][5] = ModelLoad("asset\\model\\oumu\\idle_wood\\oumu_idle_c_06.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYWOOD][6] = ModelLoad("asset\\model\\oumu\\idle_wood\\oumu_idle_c_07.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_IDLE_CARRYWOOD][7] = ModelLoad("asset\\model\\oumu\\idle_wood\\oumu_idle_c_08.fbx");

	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_SHOT][0] = ModelLoad("asset\\model\\oumu\\attack\\oumu_attack_01.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_SHOT][1] = ModelLoad("asset\\model\\oumu\\attack\\oumu_attack_02.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_SHOT][2] = ModelLoad("asset\\model\\oumu\\attack\\oumu_attack_03.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_SHOT][3] = ModelLoad("asset\\model\\oumu\\attack\\oumu_attack_04.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_SHOT][4] = ModelLoad("asset\\model\\oumu\\attack\\oumu_attack_05.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_SHOT][5] = ModelLoad("asset\\model\\oumu\\attack\\oumu_attack_06.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_SHOT][6] = ModelLoad("asset\\model\\oumu\\attack\\oumu_attack_07.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_SHOT][7] = ModelLoad("asset\\model\\oumu\\attack\\oumu_attack_08.fbx");

	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_REPAIR][0] = ModelLoad("asset\\model\\oumu\\repair\\oumu_fix_01.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_REPAIR][1] = ModelLoad("asset\\model\\oumu\\repair\\oumu_fix_02.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_REPAIR][2] = ModelLoad("asset\\model\\oumu\\repair\\oumu_fix_03.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_REPAIR][3] = ModelLoad("asset\\model\\oumu\\repair\\oumu_fix_04.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_REPAIR][4] = ModelLoad("asset\\model\\oumu\\repair\\oumu_fix_05.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_REPAIR][5] = ModelLoad("asset\\model\\oumu\\repair\\oumu_fix_06.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_REPAIR][6] = ModelLoad("asset\\model\\oumu\\repair\\oumu_fix_07.fbx");
	g_PlayerModel[PLAYERTYPE_OUMU][MODELPLAYER_REPAIR][7] = ModelLoad("asset\\model\\oumu\\repair\\oumu_fix_08.fbx");

	LoadDraw();
	//ねこ関連モデル
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE][0] = ModelLoad("asset\\model\\neko\\idle\\neko_idle_a_01.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE][1] = ModelLoad("asset\\model\\neko\\idle\\neko_idle_a_02.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE][2] = ModelLoad("asset\\model\\neko\\idle\\neko_idle_a_03.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE][3] = ModelLoad("asset\\model\\neko\\idle\\neko_idle_a_04.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE][4] = ModelLoad("asset\\model\\neko\\idle\\neko_idle_a_05.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE][5] = ModelLoad("asset\\model\\neko\\idle\\neko_idle_a_06.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE][6] = ModelLoad("asset\\model\\neko\\idle\\neko_idle_a_07.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE][7] = ModelLoad("asset\\model\\neko\\idle\\neko_idle_a_08.fbx");

	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_RUN][0] = ModelLoad("asset\\model\\neko\\run\\neko_walk_a_01.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_RUN][1] = ModelLoad("asset\\model\\neko\\run\\neko_walk_a_02.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_RUN][2] = ModelLoad("asset\\model\\neko\\run\\neko_walk_a_03.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_RUN][3] = ModelLoad("asset\\model\\neko\\run\\neko_walk_a_04.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_RUN][4] = ModelLoad("asset\\model\\neko\\run\\neko_walk_a_05.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_RUN][5] = ModelLoad("asset\\model\\neko\\run\\neko_walk_a_06.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_RUN][6] = ModelLoad("asset\\model\\neko\\run\\neko_walk_a_07.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_RUN][7] = ModelLoad("asset\\model\\neko\\run\\neko_walk_a_08.fbx");

	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYBALL][0] = ModelLoad("asset\\model\\neko\\carry_ball\\neko_walk_b_01.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYBALL][1] = ModelLoad("asset\\model\\neko\\carry_ball\\neko_walk_b_02.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYBALL][2] = ModelLoad("asset\\model\\neko\\carry_ball\\neko_walk_b_03.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYBALL][3] = ModelLoad("asset\\model\\neko\\carry_ball\\neko_walk_b_04.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYBALL][4] = ModelLoad("asset\\model\\neko\\carry_ball\\neko_walk_b_05.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYBALL][5] = ModelLoad("asset\\model\\neko\\carry_ball\\neko_walk_b_06.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYBALL][6] = ModelLoad("asset\\model\\neko\\carry_ball\\neko_walk_b_07.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYBALL][7] = ModelLoad("asset\\model\\neko\\carry_ball\\neko_walk_b_08.fbx");

	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYBALL][0] = ModelLoad("asset\\model\\neko\\idle_ball\\neko_idle_b_01.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYBALL][1] = ModelLoad("asset\\model\\neko\\idle_ball\\neko_idle_b_02.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYBALL][2] = ModelLoad("asset\\model\\neko\\idle_ball\\neko_idle_b_03.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYBALL][3] = ModelLoad("asset\\model\\neko\\idle_ball\\neko_idle_b_04.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYBALL][4] = ModelLoad("asset\\model\\neko\\idle_ball\\neko_idle_b_05.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYBALL][5] = ModelLoad("asset\\model\\neko\\idle_ball\\neko_idle_b_06.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYBALL][6] = ModelLoad("asset\\model\\neko\\idle_ball\\neko_idle_b_07.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYBALL][7] = ModelLoad("asset\\model\\neko\\idle_ball\\neko_idle_b_08.fbx");

	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYWOOD][0] = ModelLoad("asset\\model\\neko\\carry_wood\\neko_walk_c_01.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYWOOD][1] = ModelLoad("asset\\model\\neko\\carry_wood\\neko_walk_c_02.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYWOOD][2] = ModelLoad("asset\\model\\neko\\carry_wood\\neko_walk_c_03.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYWOOD][3] = ModelLoad("asset\\model\\neko\\carry_wood\\neko_walk_c_04.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYWOOD][4] = ModelLoad("asset\\model\\neko\\carry_wood\\neko_walk_c_05.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYWOOD][5] = ModelLoad("asset\\model\\neko\\carry_wood\\neko_walk_c_06.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYWOOD][6] = ModelLoad("asset\\model\\neko\\carry_wood\\neko_walk_c_07.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_CARRYWOOD][7] = ModelLoad("asset\\model\\neko\\carry_wood\\neko_walk_c_08.fbx");

	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYWOOD][0] = ModelLoad("asset\\model\\neko\\idle_wood\\neko_idle_c_01.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYWOOD][1] = ModelLoad("asset\\model\\neko\\idle_wood\\neko_idle_c_02.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYWOOD][2] = ModelLoad("asset\\model\\neko\\idle_wood\\neko_idle_c_03.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYWOOD][3] = ModelLoad("asset\\model\\neko\\idle_wood\\neko_idle_c_04.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYWOOD][4] = ModelLoad("asset\\model\\neko\\idle_wood\\neko_idle_c_05.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYWOOD][5] = ModelLoad("asset\\model\\neko\\idle_wood\\neko_idle_c_06.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYWOOD][6] = ModelLoad("asset\\model\\neko\\idle_wood\\neko_idle_c_07.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_IDLE_CARRYWOOD][7] = ModelLoad("asset\\model\\neko\\idle_wood\\neko_idle_c_08.fbx");

	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_SHOT][0] = ModelLoad("asset\\model\\neko\\attack\\neko_attack_01.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_SHOT][1] = ModelLoad("asset\\model\\neko\\attack\\neko_attack_02.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_SHOT][2] = ModelLoad("asset\\model\\neko\\attack\\neko_attack_03.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_SHOT][3] = ModelLoad("asset\\model\\neko\\attack\\neko_attack_04.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_SHOT][4] = ModelLoad("asset\\model\\neko\\attack\\neko_attack_05.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_SHOT][5] = ModelLoad("asset\\model\\neko\\attack\\neko_attack_06.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_SHOT][6] = ModelLoad("asset\\model\\neko\\attack\\neko_attack_07.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_SHOT][7] = ModelLoad("asset\\model\\neko\\attack\\neko_attack_08.fbx");

	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_REPAIR][0] = ModelLoad("asset\\model\\neko\\repair\\neko_fix_01.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_REPAIR][1] = ModelLoad("asset\\model\\neko\\repair\\neko_fix_02.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_REPAIR][2] = ModelLoad("asset\\model\\neko\\repair\\neko_fix_03.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_REPAIR][3] = ModelLoad("asset\\model\\neko\\repair\\neko_fix_04.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_REPAIR][4] = ModelLoad("asset\\model\\neko\\repair\\neko_fix_05.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_REPAIR][5] = ModelLoad("asset\\model\\neko\\repair\\neko_fix_06.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_REPAIR][6] = ModelLoad("asset\\model\\neko\\repair\\neko_fix_07.fbx");
	g_PlayerModel[PLAYERTYPE_CAT][MODELPLAYER_REPAIR][7] = ModelLoad("asset\\model\\neko\\repair\\neko_fix_08.fbx");

	//g_PlayerModel[MODELPLAYER_EXPLOSION][0] = ModelLoad("asset\\model\\tako_idolcannon\\tako_idle_b_01.fbx");
	LoadDraw();

	g_PlayerHoldModel[PLAYERTYPE_TAKO] = ModelLoad("asset\\model\\tako\\hold\\tako_kamae_01.fbx");
	g_PlayerHoldModel[PLAYERTYPE_KAJIKI] = ModelLoad("asset\\model\\kajiki\\hold\\kajiki_kamae_01.fbx");
	g_PlayerHoldModel[PLAYERTYPE_OUMU] = ModelLoad("asset\\model\\oumu\\hold\\oumu_kamae_01.fbx");
	g_PlayerHoldModel[PLAYERTYPE_CAT] = ModelLoad("asset\\model\\neko\\hold\\neko_kamae_01.fbx");


	g_PlayerBlowOffModel = ModelLoad("asset\\model\\tako\\blowoff\\tako_futtobi_01.fbx");

}

void ModelManagerFinalize()
{
	for (int i = 0; i < MODELNAME_MAX; i++)
	{
		ModelRelease(g_Model[i]);
	}
	for(int player_num = 0; player_num < PLAYERTYPE_MAX; player_num++)
	{
		for (int model_num = 0; model_num < MODELPLAYER_MAX; model_num++)
		{
			for (int num = 0; num < 8; num++)
			{
				ModelRelease(g_PlayerModel[player_num][model_num][num]);
			}
		}
		
		ModelRelease(g_PlayerHoldModel[player_num]);
	}

}

void ModelDraw(ModelName name, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 size)
{
	//マトリクス設定
	XMMATRIX world,wvp;

	
	//ワールドマトリクス
	XMMATRIX scale = XMMatrixScaling(size.x, size.y, size.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	XMMATRIX translation = XMMatrixTranslation(pos.x, pos.y, pos.z);
	world = scale * rotation * translation;

	wvp = world * g_view * g_projection;
	wvp = XMMatrixTranspose(wvp);
	world = XMMatrixTranspose(world);

	CONSTANT matrix;
	XMStoreFloat4x4(&matrix.wvp, wvp);
	XMStoreFloat4x4(&matrix.world, world);
	matrix.LightEnable = true;

	//頂点シェーダの定数バッファに行列を送信
	ShaderVertexSetMatrix(&matrix);

	//モデルの描画
	ModelDraw(g_Model[(int)name]);
}

void ModelDrawNoLight(ModelName name, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 size)
{
	//マトリクス設定
	XMMATRIX world, wvp;


	//ワールドマトリクス
	XMMATRIX scale = XMMatrixScaling(size.x, size.y, size.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	XMMATRIX translation = XMMatrixTranslation(pos.x, pos.y, pos.z);
	world = scale * rotation * translation;

	wvp = world * g_view * g_projection;
	wvp = XMMatrixTranspose(wvp);
	world = XMMatrixTranspose(world);

	CONSTANT matrix;
	XMStoreFloat4x4(&matrix.wvp, wvp);
	XMStoreFloat4x4(&matrix.world, world);
	matrix.LightEnable = false;

	//頂点シェーダの定数バッファに行列を送信
	ShaderVertexSetMatrix(&matrix);

	//モデルの描画
	ModelDraw(g_Model[(int)name]);
}

void ModelDraw(PlayerType type, ModelPlayer name, int AnimationNum, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 size)
{
	//マトリクス設定
	XMMATRIX world, wvp;


	//ワールドマトリクス
	XMMATRIX scale = XMMatrixScaling(size.x, size.y, size.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	XMMATRIX translation = XMMatrixTranslation(pos.x, pos.y, pos.z);
	world = scale * rotation * translation;

	wvp = world * g_view * g_projection;
	wvp = XMMatrixTranspose(wvp);
	world = XMMatrixTranspose(world);

	CONSTANT matrix;
	XMStoreFloat4x4(&matrix.wvp, wvp);
	XMStoreFloat4x4(&matrix.world, world);
	matrix.LightEnable = true;

	//頂点シェーダの定数バッファに行列を送信
	ShaderVertexSetMatrix(&matrix);

	//モデルの描画
	if (name == MODELPLAYER_HOLD)
	{
		ModelDraw(g_PlayerHoldModel[type]);
	}
	else if (name == MODELPLAYER_BLOWOFF)
	{
		ModelDraw(g_PlayerBlowOffModel);
	}
	else
	{
		ModelDraw(g_PlayerModel[type][name][AnimationNum]);
	}
	
}


MODEL* GetModel(ModelName name)
{
	return g_Model[(int)name];
}


void SetView(XMMATRIX view)
{
	g_view = view;
}

void SetProjection(XMMATRIX projection)
{
	g_projection = projection;
}
