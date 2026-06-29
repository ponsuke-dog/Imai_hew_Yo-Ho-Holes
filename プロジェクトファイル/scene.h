/*==============================================================================

   シーン（画面遷移）の制御 [scene.h]
														 Author : Youhei Sato
														 Date   : 2024/09/03
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef SCENE_H
#define SCENE_H
#include <Windows.h>

void SceneInitialize(HWND* hWnd , HINSTANCE hInstance , int nCmdShow);
void SceneFinalize(void);
void SceneUpdate(void);
void SceneDraw(void);
void SceneChangeExecute(void);

typedef enum {
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_MODE_SELECT,
	SCENE_ENTRY,
	SCENE_STAND_BY,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_MAX
} SCENE;

void SceneChange(SCENE scene);
void SceneAndWindowChange(SCENE scene);

HWND CreatehWnd(HINSTANCE hInstance);

bool GetModeChangePossible();
HWND GethWnd(int number);
void DeletehWnd(HWND hwnd);
int GetWindownumberFromhWnd(HWND);

void ResizeWindow(HWND hwnd,int x, int y, int w, int h);

void SetFullScreen(bool , int);

void LoadDraw();

#endif // SCENE_H
