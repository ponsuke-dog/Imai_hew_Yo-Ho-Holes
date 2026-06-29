//********************************************************************************
//
//	コントローラー制御[controller.cpp] 
// 
//							                                Author : Tomoya Saita
//							                                Date   :2024/10/29
// -------------------------------------------------------------------------------
//							                                Last Edited:2024/11/14
// 10/31    ゲッター3種追加
//              ・コントローラー
//              ・バイブレーション
//              ・接続台数
//          ポインターに修飾子の追加
//          PlayerAction()の消去
//          右スティックのXYの情報取得
//          LT,LRボタンの情報取得
//         トリガーの閾値の追加
// 
// 11/06    長押しに関するステートの追加
//          ボタンが押された瞬間の情報の更新の追加
//          前フレームのボタンの情報とボタン短押し時の処理の追加
//                                                                 追加のみで挙動の確認なし
// 11/07    挙動確認
//          IsLongPressの閾値の確認　(100ms)
// 
// 11/08    プレイヤーの入力と更新をUpdateに統括
//          関数の名前変更
// 
// 11/10    スティックが一周回転したかの関数の追加
//          回転方向の関数の追加
//          関数名の変更統一
// 
// 11/14    急激なスティックの変化に対応する関数の追加
//          スティックの判定のセクションを細かく分割
// 
// 11/20    コントローラーをクラス化及び整理
//           クラスかに伴いcontrollerクラス内では
//          一つのリモコンのみを取り扱うようにする
//          及びトリガープレスやロングプレス
//          を呼び出せるように新たに関数追加（本田）
// 
// 11/21   Controllerのステートにトリガーのステートの追加 
//         コントローラーのアップデートにトリガーのステートの更新の追加（才田）
//         
//         コントローラーのスティック関連を少し変更　（本田）
// 
// 12/01    コントローラーの回転受け取り最終調整（本田）
//********************************************************************************

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Xinput.h>

//最大プレイヤー数
#define PLAYERINDEX_MAX (4)
// デッドゾーンの定義
#define DEADZONE (8000)  
//円周率
#define M_PI (3.14159265358979323846)
// 連続した小さな角度変化を検知する
#define MIN_ROTATION_STEP (0.02f)        
// 急激な角度変化のしきい値
#define MAX_JUMP_THRESHOLD (M_PI / 2)  //ラジアン値で現在は90度で測る

 //********************************************************************************
 // 構造体の宣言
 //********************************************************************************
 //ボタンが長押しされているかの情報の構造体 Controllerの中で使用してるからcontrollerの下には移動しない
typedef struct
{
    bool isPressed;         // ボタンが現在押されているか
    DWORD pressStartTime;   // ボタンが押され始めた時間
    bool wasPressedLastFrame;  // 前フレームで押されていたか
} ButtonState;

// コントローラーの状態を保持する構造体
typedef struct
{
    XINPUT_STATE state;      // XInputの全体状態
    int isConnected;         // 接続状態
    ButtonState aButton;     // Aボタンの状態
    ButtonState bButton;     // Bボタンの状態
    ButtonState xButton;     // Xボタンの状態
    ButtonState yButton;     // Yボタンの状態
    ButtonState rshoulder;   // RBボタンの状態
    ButtonState lshoulder;   // LBボタンの状態
    ButtonState leftTrigger;  // 左トリガーの状態
    ButtonState rightTrigger; // 右トリガーの状態
    ButtonState startButton;  // スタートボタンの状態
    ButtonState backButton;   // ビューボタンの状態
    ButtonState DpadUp;       // 十字キー　↑
    ButtonState DpadDown;     // 十字キー　↓
    ButtonState DpadRight;    // 十字キー　→
    ButtonState DpadLeft;     // 十字キー　←

} ControllerState;

//バイブレーションの経過時間を保持する構造体
typedef struct
{
    DWORD startTime;    //振動開始の時刻
    DWORD duration;     //振動の持続時間
    int isVibrating;    //振動中かどうかのフラグ
}VibrationInfo;


class Controller
{
private:
   
    //********************************************************************************
    // メンバ変数
    //********************************************************************************

    //コントローラーのグローバル変数
    ControllerState g_Controllers;
    //コントローラーの番号
    int g_PlayerIndex = -1;
    //バイブレーションの時間管理
    VibrationInfo g_VibrationInfo;
    //初期化時の接続台数
    int g_Controller_Numbers = 0;

    SHORT LeftStickX = 0;
    SHORT LeftStickY = 0;
    SHORT RightStickX = 0;
    SHORT RightStickY = 0;

    //回転用変数
    //LeftStick（左ステック）
    float CurrentStickLocationLeft = 0.0f; //現在のスティック位置
    float PreviousStickLocationLeft = 0.0f; //前回のステック位置

    //RightStick（右ステック）
    float CurrentStickLocationRight = 0.0f; //現在のスティック位置
    float PreviousStickLocationRight = 0.0f; //前回のステック位置

    //********************************************************************************
    // メンバ関数????
    //********************************************************************************
    SHORT Controller_GetLeftStickX();    // 左スティックのX軸取得
    SHORT Controller_GetLeftStickY();    // 左スティックのY軸取得
    SHORT Controller_GetRightStickX();   // 右スティックのX軸取得
    SHORT Controller_GetRightStickY();   // 右スティックのY軸取得

    //回転用
    float Controller_GetStickAngle(SHORT x, SHORT y);                     // スティックの角度を取得
    int Controller_GetRotationSection(float angle); // 角度をセクションに分ける関数
    int Controller_GetRotationDirection(float previousAngle, float currentAngle);      // 回転の方向を確認

    //バイブレーション
    void Controller_StartVibration(WORD leftMotor, WORD rightMotor);  // モーターの振動開始

public:
    //********************************************************************************
    // 関数の宣言
    //********************************************************************************
   
    //コンストラクタ＆デストラクタ
    Controller(int controllernumber);// コントローラーの初期化と接続台数の登録
    ~Controller() {};


    //void Controller_Initialize();                   // コントローラーの初期化と接続台数の登録(初期版）
    
    void Controller_Set(); //コントローラーの初期化と接続台数の登録
    bool IsConnected();

    //コントローラーの更新関連
    void Controller_Update();                       // 状態の更新とすべてのプレイヤーの入力処理
    void Controller_UpdateButtonState(ButtonState* buttonState, bool isCurrentlyPressed); // ボタンの情報の更新
    int Controller_GetConnectedCount();                           // 初期化時の接続台数の取得

    int GetPlayerIndex() { return g_PlayerIndex; };
    //********************************************************************************
    //コントローラーボタン受付
    //********************************************************************************
    int Controller_IsButtonDown(WORD button);  // ボタンの押下確認
    bool Controller_IsLongPress( ButtonState* buttonState, DWORD threshold); // 長押しの閾値と比較
    bool Controller_IsJustPressed( ButtonState* buttonState);                  // 押された瞬間の判定
    int Controller_IsButtonUp(WORD button);                      //ボタンが上がったかどうかの判定
    BYTE Controller_GetLeftTrigger();    // 左トリガーの情報の取得
    BYTE Controller_GetRightTrigger();   // 右トリガーの情報の取得


   
    //********************************************************************************
    //コントローラースティック受付
    //********************************************************************************
    //ステックの回転チェック
    bool Controller_IsFullRotation_LeftStick();     //左ステックの回転
    bool Controller_IsFullRotation_RightStick();    //右ステックの回転
   

    //ステックの入力受け取り
    bool Controller_LeftStickIsUp();        //左ステックの上入力
    bool Controller_LeftStickIsDown();      //左ステックの下入力
    bool Controller_LeftStickIsLeft();      //左ステックの左入力
    bool Controller_LeftStickIsRight();     //左ステックの右入力
    bool Controller_RightStickIsUp();       //右ステックの上入力
    bool Controller_RightStickIsDown();     //右ステックの下入力
    bool Controller_RightStickIsLeft();     //右ステックの左入力
    bool Controller_RightStickIsRight();    //右ステックの右入力

    SHORT GetLeftStickX();    // 左スティックのX軸取得
    SHORT GetLeftStickY();    // 左スティックのY軸取得
    SHORT GetRightStickX();   // 右スティックのX軸取得
    SHORT GetRightStickY();   // 右スティックのY軸取得

    //********************************************************************************
    //コントローラーバイブレーション関連
    //********************************************************************************
    void Controller_SetVibration(WORD leftMotor, WORD rightMotor, DWORD duration); // モーターの振動を時間で管理
    void Controller_UpdateVibration();                 // バイブレーションの更新管理
   
    ButtonState* GetButtonForTrigger(WORD button);  //IsJustPressedで使うための関数
};




#endif // CONTROLLER_H

//********************************************************************************
// 
//	[デバイス] ボタン	
//XINPUT_GAMEPAD_A	    Aボタン
//XINPUT_GAMEPAD_B	    Bボタン
//XINPUT_GAMEPAD_X	    Xボタン
//XINPUT_GAMEPAD_Y	    Yボタン
// 
// XINPUT_GAMEPAD_RIGHT_SHOULDER    RBボタン
// XINPUT_GAMEPAD_LEFT_SHOULDER     LBボタン
// 
// XINPUT_GAMEPAD_START     スタートボタン
// XINPUT_GAMEPAD_BACK      ビューボタン
// 
// XINPUT_GAMEPAD_DPAD_UP       十字キー　↑
// XINPUT_GAMEPAD_DPAD_DOWN     十字キー　↓
// XINPUT_GAMEPAD_DPAD_LEFT     十字キー　→
// XINPUT_GAMEPAD_DPAD_RIGHT    十字キー　←
// 
//　
// XINPUT_GAMEPAD_LEFT_THUMB	LT（トリガーボタン）
// XINPUT_GAMEPAD_RIGHT_THUMB   RT（トリガーボタン）
//********************************************************************************

