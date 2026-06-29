/*==============================================================================

   アプリケーションの構成・設定 [configuration.h]
														 Author : Youhei Sato
														 Date   : 2024/05/07
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define WINDOW_CAPTION "Yo-Ho-Holes"       // ウィンドウの名前

#define WINDOWS_COUNT	(4)
#define ONE_WINDOW  (1)
#define TWO_WINDOWS (2)

#define SCREEN_WIDTH  (1280)                // スクリーン(クライアント領域)の幅
#define SCREEN_HEIGHT  (720)                // スクリーン(クライアント領域)の高さ

typedef enum
{
	GAMEMODE_NONE = -1,
	GAMEMODE_SOLO,//ソロが一画面
	GAMEMODE_LOCAL_SPLITSCREEN,//一画面分割あり
	GAMEMODE_LOCAL_MULTIWINDOW,//二画面分割なし
	GAMEMODE_SEGMENTATION,//に画面分割あり
	GAMEMODE_MULTIWINDOW,//四画面分割なし
	GAMEMODE_ONLINE,//一画面分割なし
	GAMEMODE_MAX
}GAMEMODE;

//typedef enum
//{
//	GAMESTATE_NONE = -1,
//
//	GAMESTATE_PLAY,
//	GAMESTATE_EVENT,
//	GAMESTATE_END,
//
//	GAMESTATE_MAX,
//
//}GameState;

#endif // CONFIGURATION_H
