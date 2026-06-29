/*==============================================================================

   Windowの表示 [main.cpp]
														 Author : Youhei Sato
														 Date   : 2024/05/07
--------------------------------------------------------------------------------

==============================================================================*/
#include <Windows.h>
#include "configuration.h"
#include "game.h"
#include <sstream>

#include "scene.h"
#include "directx.h"

#include "keyboard.h"
#include "mouse.h"

#pragma comment (lib, "winmm.lib")

/*------------------------------------------------------------------------------
   定数定義
------------------------------------------------------------------------------*/
#define CLASS_NAME     "GameWindow"         // ウインドウクラスの名前

/*------------------------------------------------------------------------------
   プロトタイプ宣言
------------------------------------------------------------------------------*/
// ウィンドウプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*------------------------------------------------------------------------------
	グローバル変数
------------------------------------------------------------------------------*/
static DWORD g_CountFPS = 0;
//static bool g_ChangingWindow = false;



/*------------------------------------------------------------------------------
   メイン
------------------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	// 使用しない一時変数を明示
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

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
	DWORD window_style = WS_OVERLAPPEDWINDOW ;

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
	HWND hWnd1 = CreateWindow(
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

	HWND hWndFullScreen = CreateWindow(
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

	HWND hWnd3 = CreateWindow(
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

	HWND hWnd4 = CreateWindow(
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

	if (hWnd1 == NULL) {
		// ウィンドウハンドルが何らかの理由で生成出来なかった
		return 0;
	}
	if (hWndFullScreen == NULL) {
		// ウィンドウハンドルが何らかの理由で生成出来なかった
		return 0;
	}

	if (hWnd3 == NULL) {
		// ウィンドウハンドルが何らかの理由で生成出来なかった
		return 0;
	}
	if (hWnd4 == NULL) {
		// ウィンドウハンドルが何らかの理由で生成出来なかった
		return 0;
	}

	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	HWND hWnds[WINDOWS_COUNT] = { hWnd1, hWndFullScreen , hWnd3, hWnd4};
	//HWND hWnds[WINDOWS_COUNT] = { hWnd1};

	// 初期化処理
	SceneInitialize(hWnds , hInstance , nCmdShow);

	// 指定のウィンドウハンドルのウィンドウを指定の方法で表示
	for (int i = 0; i < ONE_WINDOW; i++) {
		ShowWindow(hWnds[i], nCmdShow);
	}


	// 時間計測用
	DWORD dwExecLastTime = 0;
	DWORD dwFPSLastTime = 0;
	DWORD dwCurrentTime = 0;
	DWORD dwFrameCount = 0;

	timeBeginPeriod(1); // 分解能を設定

	dwExecLastTime = dwFPSLastTime = timeGetTime(); // システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	// Windowsゲーム用メインループ
	MSG msg = {}; // msg.message == WM_NULL
	while (WM_QUIT != msg.message) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// メッセージがある場合はメッセージ処理を優先
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

			dwCurrentTime = timeGetTime(); // システム時刻を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 1000) // 1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime; // FPSを測定した時刻を保存
				dwFrameCount = 0; // カウントをクリア
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60)) // 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime; // 処理した時刻を保存
#ifdef _DEBUG // デバッグ版の時だけFPSを表示する
				std::stringstream caption;
				caption << WINDOW_CAPTION << " FPS:" << g_CountFPS;
				SetWindowText(hWnd1, caption.str().c_str());
				SetWindowText(hWndFullScreen, caption.str().c_str());
#endif
				// ここがゲーム処理
				SceneUpdate();
				
				SceneDraw();

				SceneChangeExecute();

				Keyboard_Update();

				dwFrameCount++; // 処理回数のカウントを加算
			}
			else {
				Sleep(0);
			}
		}
	}

	// あとかたづけ
	SceneFinalize();

	return (int)msg.wParam;
}


//ウィンドプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SYSCOMMAND:
		if (wParam == SC_MAXIMIZE) {
			// 最大化ボタンが押されたとき
			
			// 最大化状態にする
			LONG style = GetWindowLong(hWnd, GWL_STYLE);
			style &= ~WS_CAPTION; // タイトルバーを削除
			SetWindowLong(hWnd, GWL_STYLE, style);

			// 最大化後にウィンドウ位置とサイズを維持
			RECT rc;
			GetWindowRect(hWnd, &rc);
			//SetWindowPos(hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED);
			ResizeWindow(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
			
			SetFullScreen(true , GetWindownumberFromhWnd(hWnd));
		}
	
		
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DeletehWnd(hWnd);

			DestroyWindow(hWnd);		//指定のウィンドウにWM_DESTROYメッセージを送る
		}
		return 0;		//DefWindowPrac関数にメッセージを流さず終了することによって何もなかったことにする

	case WM_DESTROY:		//ウィンドウの破棄メッセージ
		
		if (GethWnd(0) == NULL)
		{
			PostQuitMessage(0);		//WM_QUITメッセージの送信
		}
		return 0;

	case WM_ACTIVATEAPP:
		Keyboard_ProcessMessage(uMsg, wParam, lParam);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}

		if (wParam == VK_F11) {
			// F11キーが押されたときに最大化と復元を切り替える

		// 復元処理
			LONG style = GetWindowLong(hWnd, GWL_STYLE);
			style |= WS_CAPTION; // タイトルバーを再表示
			SetWindowLong(hWnd, GWL_STYLE, style);

			// 現在表示されているモニター基準で復元
			MONITORINFO mi;
			mi.cbSize = sizeof(MONITORINFO);

			// 現在のウィンドウが表示されているモニターを取得
			HMONITOR currentMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
			if (GetMonitorInfo(currentMonitor, &mi)) {
				// 現在のモニターの位置にウィンドウを復元
				SetWindowPos(hWnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_FRAMECHANGED);
			}




		}

	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard_ProcessMessage(uMsg, wParam, lParam);
		break;


	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse_ProcessMessage(uMsg, wParam, lParam);
		break;
	}

	

	//通常メッセージ処理はこの関数に任せる		※終了処理は肩代わりできない
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//HWND CreatehWnd(HINSTANCE hInstance)
//{
//	// ウィンドウクラス構造体の設定
//	WNDCLASS wc = {};
//	wc.lpfnWndProc = WndProc;                          // ウィンドウプロシージャの指定
//	wc.lpszClassName = CLASS_NAME;                     // クラス名の設定
//	wc.hInstance = hInstance;                          // インスタンスハンドルの指定
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // マウスカーソルを指定
//	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // ウインドウのクライアント領域の背景色を設定
//
//	// クラス登録
//	RegisterClass(&wc);
//
//	// DWORD window_style = (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) ^ WS_MAXIMIZEBOX;
//	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
//
//	// 基本矩形座標
//	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
//
//	// 指定したクライアント領域を確保するために新たな矩形座標を計算
//	AdjustWindowRect(&window_rect, window_style, FALSE);
//
//	// 新たなWindowの矩形座標から幅と高さを算出
//	int window_width = window_rect.right - window_rect.left;
//	int window_height = window_rect.bottom - window_rect.top;
//
//	// プライマリモニターの画面解像度取得
//	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
//	int desktop_height = GetSystemMetrics(SM_CYSCREEN);
//
//	// デスクトップの真ん中にウィンドウが生成されるように座標を計算
//	// ※ただし万が一、デスクトップよりウィンドウが大きい場合は左上に表示
//	int window_x = max((desktop_width - window_width) / 2, 0);
//	int window_y = max((desktop_height - window_height) / 2, 0);
//
//	// ウィンドウの生成
//	HWND hWnd = CreateWindow(
//		CLASS_NAME,          // ウィンドウ クラス
//		WINDOW_CAPTION_ONE,      // ウィンドウ テキスト
//		window_style,        // ウィンドウ スタイル
//
//		// サイズと位置
//		window_x,
//		window_y,
//		window_width,
//		window_height,
//
//		NULL,       // 親ウィンドウハンドル
//		NULL,       // メニューハンドル
//		hInstance,  // インスタンスハンドル
//		NULL        // 追加のアプリケーションデータ
//	);
//
//	return hWnd;
//}