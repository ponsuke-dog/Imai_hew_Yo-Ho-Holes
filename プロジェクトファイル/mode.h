/*==============================================================================

  ゲームモードの変更 [mode.h]
														 Author : tomoya saita
														 Date   : 2025/02/01
--------------------------------------------------------------------------------

==============================================================================*/

#pragma once
#include "configuration.h"

typedef enum {
	MODESCENESTATE_NONE = -1,
	MODESCENESTATE_START,
	MODESCENESTATE_VERSUS,
	MODESCENESTATE_TEAM,
	MODESCENESTATE_MAX
}ModeSceneState;

void ModeInitialize();
void ModeFinalize();
void ModeUpdate();
void ModeDraw();
void SetGameMode(GAMEMODE gamemode);

GAMEMODE GetGameMode();

GAMEMODE ResetGameMode();