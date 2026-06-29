
#include <Windows.h>
#include "directx.h"
#include "shader.h"
#include "texture.h"
#include "keyboard.h"
#include <math.h>
#include "game.h"
#include "title.h"
#include "result.h"
#include "sprite.h"
#include "manager_model.h"
#include "mode.h"
#include "standbyscreen.h"
#include "entry.h"
#include "fade.h"

//#pragma comment (lib, "winmm.lib")

#include "scene.h"
#include "cri.h"

void initialize(void);
void finalize(void);

/*------------------------------------------------------------------------------
   定数定義
------------------------------------------------------------------------------*/
#define CLASS_NAME     "GameWindow"         // ウインドウクラスの名前

/*------------------------------------------------------------------------------
   プロトタイプ宣言
------------------------------------------------------------------------------*/
// ウィンドウプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


static SCENE g_Scene = SCENE_TITLE;
static SCENE g_NextScene = g_Scene;
static HWND g_hWnd[4] = {};
static int g_nCmdShow;
static HINSTANCE g_hInstance;
//static bool  g_SceneAndWindowChange;
static bool  g_ModeChangePossible;
//static bool g_DontFinializeModelManagerAtStart;
static int g_LoadPercentage;
static int g_LoadingTexture[6];
static GAMEMODE g_GameMode;
static bool g_IsFullScreen[4];

void SceneInitialize(HWND hWnd[], HINSTANCE hInstance ,int nCmdShow)
{
	for (int i = 0; i < 4; i++)
	{
		g_hWnd[i] = hWnd[i];
	}
	g_hInstance = hInstance;

	g_nCmdShow = nCmdShow;

	//g_SceneAndWindowChange = false;

	g_ModeChangePossible = true;

	//g_DontFinializeModelManagerAtStart = true;

	g_LoadPercentage = 0;

	g_GameMode = GAMEMODE_NONE;

	g_IsFullScreen[0] = false;
	g_IsFullScreen[1] = false;
	g_IsFullScreen[2] = false;
	g_IsFullScreen[3] = false;

	DirectXInitialize(hWnd);
	ShaderInitialize(DirectXGetDevice(), DirectXGetDeviceContext());
	TextureInitialize(DirectXGetDevice());
	SpriteInitialize(DirectXGetDevice(), DirectXGetDeviceContext());

	FadeInitialize();

	CRIInitialize();

	initialize();
}

void SceneFinalize(void)
{
	finalize();

	CRIFinalize();

	//ModelManagerFinalize();
	FadeFinalize();

	SpriteFinalize();
	TextureFinalize();
	ShaderFinalize();
	DirectXFinalize();
}

void initialize(void)
{
	if (g_GameMode != GetGameMode())
	{

		DirectXInitialize(g_hWnd, GetGameMode());
		ShaderInitialize(DirectXGetDevice(), DirectXGetDeviceContext());
		TextureInitialize(DirectXGetDevice());
		SpriteInitialize(DirectXGetDevice(), DirectXGetDeviceContext());

		FadeInitialize();

		g_LoadingTexture[0] = TextureLoad(L"asset\\texture\\loading_00.png");
		g_LoadingTexture[1] = TextureLoad(L"asset\\texture\\loading_01.png");
		g_LoadingTexture[2] = TextureLoad(L"asset\\texture\\loading_02.png");
		g_LoadingTexture[3] = TextureLoad(L"asset\\texture\\loading_03.png");
		g_LoadingTexture[4] = TextureLoad(L"asset\\texture\\loading_04.png");
		g_LoadingTexture[5] = TextureLoad(L"asset\\texture\\loading_05.png");

		ModelManagerInitialize();
		
		g_LoadPercentage = 0;

		switch (GetGameMode())
		{
		case GAMEMODE_NONE:
		case GAMEMODE_SOLO:
		case GAMEMODE_LOCAL_SPLITSCREEN:
		{
			if (g_IsFullScreen[0])
			{
				ShowWindow(g_hWnd[0], SW_MAXIMIZE);
			}
			else
			{
				ShowWindow(g_hWnd[0], SW_NORMAL);
			}
			
		}
		break;
		case GAMEMODE_LOCAL_MULTIWINDOW:
		{
			for (int i = 0; i < TWO_WINDOWS; i++) {

				if (g_IsFullScreen[i])
				{
					ShowWindow(g_hWnd[i], SW_MAXIMIZE);
				}
				else
				{
					ShowWindow(g_hWnd[i], SW_NORMAL);
				}
				
			}
		}
		break;
		case GAMEMODE_SEGMENTATION:
		{
			for (int i = 0; i < TWO_WINDOWS; i++) 
			{
				if (g_IsFullScreen[i])
				{
					ShowWindow(g_hWnd[i], SW_MAXIMIZE);
				}
				else
				{
					ShowWindow(g_hWnd[i], SW_NORMAL);
				}
			}
		}
		break;
		case GAMEMODE_MULTIWINDOW:
		{
			for (int i = 0; i < WINDOWS_COUNT; i++) 
			{
				if (g_IsFullScreen[i])
				{
					ShowWindow(g_hWnd[i], SW_MAXIMIZE);
				}
				else
				{
					ShowWindow(g_hWnd[i], SW_NORMAL);
				}
			}
		}
		break;
		case GAMEMODE_ONLINE:
			if (g_IsFullScreen[0])
			{
				ShowWindow(g_hWnd[0], SW_MAXIMIZE);
			}
			else
			{
				ShowWindow(g_hWnd[0], SW_NORMAL);
			}
			break;
		}


		//g_SceneAndWindowChange = false;

		g_GameMode = GetGameMode();
	}

	switch (g_Scene)
	{
	case SCENE_TITLE:
		TitleInitialize();
		BGMStart(GetAppBGM(), CRI_BGM_BGM_TITLE);
		break;
	case SCENE_MODE_SELECT:
		ModeInitialize();
		BGMStart(GetAppBGM(), CRI_BGM_BGM_MODE_SELECT);
		if (!g_ModeChangePossible)
		{
			SetGameMode(g_GameMode);
		}
		break;
	case SCENE_ENTRY:
		EntryInitialize();
		break;
	case SCENE_STAND_BY:
		StandByScreenInitialize();
		BGMStart(GetAppBGM(), CRI_BGM_BGM_STANDBY);
		break;
	case SCENE_GAME:
		GameInitialize();
		BGMStart(GetAppBGM(), CRI_BGM_BGM_GAME);
		break;
	case SCENE_RESULT:
		ResultInitialize();
		BGMStart(GetAppBGM(), CRI_BGM_BGM_RESULT_DRAW);
		break;
	default:
		break;
	}
	//GameInitialize();

}

void finalize(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		TitleFinalize();
		BGMStop(GetAppBGM());
		break;
	case SCENE_MODE_SELECT:
		ModeFinalize();
		BGMStop(GetAppBGM());
		break;
	case SCENE_ENTRY:
		EntryFinalize();
		break;
	case SCENE_STAND_BY:
		StandByScreenFinalize();
		BGMStop(GetAppBGM());
		break;
	case SCENE_GAME:
		GameFinalize();
		BGMStop(GetAppBGM());
		break;
	case SCENE_RESULT:
		ResultFinalize();
		BGMStop(GetAppBGM());
		break;
	default:
		break;
	}

	if (g_GameMode != GetGameMode())
	{
		if (g_GameMode  != GAMEMODE_NONE /*!g_DontFinializeModelManagerAtStart*/)
		{
			ModelManagerFinalize();
		}

		FadeFinalize();

		SpriteFinalize();
		TextureFinalize();
		ShaderFinalize();
		DirectXFinalize();

		switch (GetGameMode())
		{
		case GAMEMODE_NONE:
		case GAMEMODE_SOLO:
		case GAMEMODE_LOCAL_SPLITSCREEN:
		{
			for (int i = 1; i < 4; i++)
			{
				DestroyWindow(g_hWnd[i]);
				SetFullScreen(false, i);
				g_hWnd[i] = NULL;
			}

			for (int i = 0; i < ONE_WINDOW; i++)
			{
				if (g_hWnd[i] == NULL)
				{
					g_hWnd[i] = CreatehWnd(g_hInstance);
				}
			}
		}
		break;
		case GAMEMODE_LOCAL_MULTIWINDOW:
		{
			for (int i = 2; i < 4; i++)
			{
				DestroyWindow(g_hWnd[i]);
				SetFullScreen(false, i);
				g_hWnd[i] = NULL;
			}

			for (int i = 0; i < TWO_WINDOWS; i++)
			{
				if (g_hWnd[i] == NULL)
				{
					g_hWnd[i] = CreatehWnd(g_hInstance);
				}
			}
		}
		break;
		case GAMEMODE_SEGMENTATION:
		{
			for (int i = 2; i < 4; i++)
			{
				DestroyWindow(g_hWnd[i]);
				SetFullScreen(false, i);
				g_hWnd[i] = NULL;
			}

			for (int i = 0; i < TWO_WINDOWS; i++)
			{
				if (g_hWnd[i] == NULL)
				{
					g_hWnd[i] = CreatehWnd(g_hInstance);
				}
			}
		}
		break;
		case GAMEMODE_MULTIWINDOW:
			for (int i = 0; i < WINDOWS_COUNT; i++)
			{
				if (g_hWnd[i] == NULL)
				{
					g_hWnd[i] = CreatehWnd(g_hInstance);
				}
			}

			break;
		case GAMEMODE_ONLINE:
			for (int i = 1; i < 4; i++)
			{
				DestroyWindow(g_hWnd[i]);
				SetFullScreen(false, i);
				g_hWnd[i] = NULL;
			}

			for (int i = 0; i < ONE_WINDOW; i++)
			{
				if (g_hWnd[i] == NULL)
				{
					g_hWnd[i] = CreatehWnd(g_hInstance);
				}
			}
			break;
		}

		//g_DontFinializeModelManagerAtStart = false;
	}

}

void SceneUpdate(void)
{
	if (Keyboard_IsKeyTrigger(KK_RIGHTCONTROL) && Keyboard_IsKeyDown(KK_E) && Keyboard_IsKeyDown(KK_N) && Keyboard_IsKeyDown(KK_D))
	{
		PostQuitMessage(0);
	}

	FadeUpdate();

	//if (GetFadeState() == FADE_STATE_OUT)return;

	switch (g_Scene)
	{
	case SCENE_TITLE:
		TitleUpdate();
		//ゲーム一回目は絶対に使わず二回目以降のみ使うこと
		if (Keyboard_IsKeyTrigger(KK_LEFTCONTROL) && Keyboard_IsKeyDown(KK_H) && Keyboard_IsKeyDown(KK_A) && Keyboard_IsKeyDown(KK_L))
		{
			//三項演算子・・・しっかり学びましょう(´・ω・`)
			g_ModeChangePossible == true ? (g_ModeChangePossible = false) : (g_ModeChangePossible = true);
		}
		break;
	case SCENE_MODE_SELECT:
		ModeUpdate();
		break;
	case SCENE_ENTRY:
		EntryUpdate();
		break;
	case SCENE_STAND_BY:
		StandByScreenUpdate();
		break;
	case SCENE_GAME:
		GameUpdate();
		break;
	case SCENE_RESULT:
		ResultUpdate();
		break;
	default:
		break;
	}

	CRIUpdate();

}

void SceneDraw(void)
{
	for (auto itr : GetSwapChains())
	{
		itr->SetRenderTarget();


		itr->Clear();

		

		switch (g_Scene)
		{
		case SCENE_TITLE:
			TitleDraw();
			break;
		case SCENE_MODE_SELECT:
			ModeDraw();
			break;
		case SCENE_ENTRY:
			EntryDraw();
			break;
		case SCENE_STAND_BY:
			StandByScreenDraw();
			break;
		case SCENE_GAME:
			GameDraw(itr->GetWindowNumber());
			break;
		case SCENE_RESULT:
			ResultDraw(itr->GetWindowNumber());
			break;
		default:
			break;
		}

		
		SetDepthEnable(false);
		FadeDraw();
		SetDepthEnable(true);

		itr->Present();
	}

	/*Clear();

	switch (g_Scene)
	{
	case SCENE_TITLE:
		TitleDraw();
		break;
	case SCENE_GAME:
		GameDraw();
		break;
	case SCENE_RESULT:
		ResultDraw();
		break;
	default:
		break;
	}

	Present();*/
}

void SceneChangeExecute(void)
{
	if (g_NextScene == g_Scene) {
		//g_SceneAndWindowChange = false;

		return;
	}

	
	
	//フェードアウトがある場合シーンチェンジしないようように設定
	if (GetFadeState() == FADE_STATE_OUT)
	{
		return;
	}
	finalize();
	g_Scene = g_NextScene;
	initialize();
 }

void SceneChange(SCENE scene)
{
	g_NextScene = scene;
}

//mo-deのみで使用
void SceneAndWindowChange(SCENE scene)
{
 	g_NextScene = scene;

	if (g_ModeChangePossible)
	{
		//g_SceneAndWindowChange = true;
	}
}



HWND CreatehWnd(HINSTANCE hInstance)
{
	// ウィンドウクラス構造体の設定
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;                          // ウィンドウプロシージャの指定
	wc.lpszClassName = CLASS_NAME;                     // クラス名の設定
	wc.hInstance = hInstance;                          // インスタンスハンドルの指定
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // マウスカーソルを指定
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // ウインドウのクライアント領域の背景色を設定

	// クラス登録
	RegisterClass(&wc);

	// DWORD window_style = (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) ^ WS_MAXIMIZEBOX;
	DWORD window_style = WS_OVERLAPPEDWINDOW;

	// 基本矩形座標
	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// 指定したクライアント領域を確保するために新たな矩形座標を計算
	AdjustWindowRect(&window_rect, window_style, FALSE);

	// 新たなWindowの矩形座標から幅と高さを算出
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	// プライマリモニターの画面解像度取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	// デスクトップの真ん中にウィンドウが生成されるように座標を計算
	// ※ただし万が一、デスクトップよりウィンドウが大きい場合は左上に表示
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	// ウィンドウの生成
	HWND hWnd = CreateWindow(
		CLASS_NAME,          // ウィンドウ クラス
		WINDOW_CAPTION,      // ウィンドウ テキスト
		window_style,        // ウィンドウ スタイル

		// サイズと位置
		window_x,
		window_y,
		window_width,
		window_height,

		NULL,       // 親ウィンドウハンドル
		NULL,       // メニューハンドル
		hInstance,  // インスタンスハンドル
		NULL        // 追加のアプリケーションデータ
	);

	return hWnd;
}

bool GetModeChangePossible()
{
	return g_ModeChangePossible;
}

HWND GethWnd(int number)
{
	return g_hWnd[number];
}

void DeletehWnd(HWND hwnd)
{
	for (int i = 0; i < 4; i++)
	{
		if (g_hWnd[i] == hwnd)
		{
			g_hWnd[i] = NULL;
		}
	}
}

int GetWindownumberFromhWnd(HWND hWnd)
{
	for (int i = 0; i < 4; i++)
	{
		if (g_hWnd[i] == hWnd)
		{
			return i;
		}
	}
	return -1;
}

void ResizeWindow(HWND hwnd, int x, int y, int w, int h)
{
	for (int i = 0; i < 4; i++)
	{
		if (g_hWnd[i] == hwnd)
		{
			SetWindowPos(g_hWnd[i], HWND_TOP, x, y, w, h, SWP_FRAMECHANGED);
		}
	}
}

void SetFullScreen(bool flag ,int windownumber)
{
	g_IsFullScreen[windownumber] = flag;
}

void LoadDraw()
{
	for (auto itr : GetSwapChains())
	{
		itr->SetRenderTarget();

		itr->Clear();

		SpriteDraw(g_LoadingTexture[g_LoadPercentage], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);

		itr->Present();
	}
	g_LoadPercentage++;
}
