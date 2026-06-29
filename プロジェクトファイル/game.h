/*==============================================================================

   ゲームのメインプログラム [game.h]
														 Author : Youhei Sato
														 Date   : 2024/05/07
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef GAME_H
#define GAME_H
#include "directx.h"

void GameInitialize(void);
void GameFinalize(void);
void GameUpdate(void);
void GameDraw(int windownumber);

#endif // GAME_H
