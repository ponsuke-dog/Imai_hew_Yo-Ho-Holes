/*===============================================================


		サウンドデータの設定 [CRIPlayer.h]

										Author : shusuke katori
										Date   : 2024/2/14

==================================================================*/

#ifndef CRIPLAYER_H

#define CRIPLAYER_H


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <cri_adx2le.h>

#include "directx.h"

/* 最大ボイス数を増やすための関連パラメータ */
#define MAX_VOICE			(48)
#define MAX_VIRTUAL_VOICE	(128)		/* ざっくり多め(通常ボイス＋HCA-MXボイス＋α) */
#define MAX_CRIFS_LOADER	(128)		/* ざっくり多め(通常ボイス＋HCA-MXボイス＋α) */

/* 最大サンプリングレート（ピッチ変更含む） */
#define MAX_SAMPLING_RATE	(48000*2)

/* HCA-MXコーデックのサンプリングレート */
#define SAMPLINGRATE_HCAMX		(32000)

#define X_POS_OFFSET_FOR_MAP_DRAW	(20)
#define PITCH_CHANGE_VALUE			(-200.0f)

#define PATH				"asset\\sound"　//　ファイルに合わせて要変更


//BGMとSE用のパスのマクロ
//----------------------------------------------------------------------------------------------

//BGM
#define ACB_FILE_PATH_BGM "asset\\sound\\BGM.acb"
#define ACF_FILE_PATH_BGM "asset\\sound\\BGM.acf"
#define AWB_FILE_PATH_BGM "asset\\sound\\BGM.awb"

//SE
#define ACB_FILE_PATH_SE "asset\\sound\\SE.acb"
#define ACF_FILE_PATH_SE "asset\\sound\\SE.acf"
#define AWB_FILE_PATH_SE "asset\\sound\\SE.awb"

//3DSE
#define ACB_FILE_PATH_SE3D "asset\\sound\\SE3D.acb"
#define ACF_FILE_PATH_SE3D "asset\\sound\\SE3D.acf"
#define AWB_FILE_PATH_SE3D "asset\\sound\\SE3D.awb"

//-----------------------------------------------------------------------------------------------


//ソースハンドルの最大数
#define SOURCE_HANDLE_MAX (100)


/**************************************************************************
 * 型定義
 * Type definitions
 **************************************************************************/

typedef struct {
    CriAtomExCueId id;     /* キューID */
} CueInfo;

typedef struct {
    CriAtomExPlayerHn       player;                 /* プレーヤ */
    CriAtomExVoicePoolHn    standard_voice_pool;    /* ボイスプール(ADX/HCA用) */
    CriAtomExVoicePoolHn    hcamx_voice_pool;       /* ボイスプール(HCA-MX用) */
    CriAtomExAcbHn          acb_hn;                /* ACBハンドル */
    CriAtomDbasId           dbas_id;               /* D-BAS ID */
    CriAtomExPlaybackId     playback_id;           /* プレイバックID */
    CriSint32               ui_cue_index;          /* 現在選択中のキュー */

    CueInfo* cue_list;              /* キューリストのポインタ */
    CriSint32               cue_list_count;        /* キューリストの数 */

    const CriChar8* acb_file_path;
    const CriChar8* awb_file_path;
    const CriChar8* acf_file_path;

} AppObj;


typedef struct {
    CriAtomExPlayerHn       player;                 /* プレーヤ */
    CriAtomExVoicePoolHn    standard_voice_pool;    /* ボイスプール(ADX/HCA用) */
    CriAtomExVoicePoolHn    hcamx_voice_pool;       /* ボイスプール(HCA-MX用) */
    CriAtomExAcbHn          acb_hn;                /* ACBハンドル */
    CriAtomDbasId           dbas_id;               /* D-BAS ID */
    CriAtomExPlaybackId     playback_id;           /* プレイバックID */
    CriSint32               ui_cue_index;          /* 現在選択中のキュー */

    CueInfo* cue_list;              /* キューリストのポインタ */
    CriSint32               cue_list_count;        /* キューリストの数 */

    const CriChar8* acb_file_path;
    const CriChar8* awb_file_path;
    const CriChar8* acf_file_path;

    //3d関係----------------------------------------


    //3dの追加ハンドル
    CriAtomEx3dSourceListHn source_list;
    CriAtomEx3dListenerHn   listener;               //3D音源のリスナーハンドル
    CriAtomExVector         listenerPos;            //3D音源のリスナーのポジション

    //3D音源のソースデータ 
    CriAtomEx3dSourceHn source[SOURCE_HANDLE_MAX];
    CriAtomExVector sourcePos[SOURCE_HANDLE_MAX];
    float destroy_time[SOURCE_HANDLE_MAX];

} App3dObj;



/* 関数宣言 */
CriBool app_atomex_initialize(AppObj* app_obj, CueInfo* cue_list, CriSint32 cue_list_count, const CriChar8* acf_file, const CriChar8* acb_path, const CriChar8* awb_file);
CriBool app_atomex_finalize(AppObj* app_ob);
CriBool app_atomex_start(AppObj* app_obj);
CriBool app_atomex_stop_player(AppObj* app_obj);
CriBool app_atomex_stop_cue(AppObj* app_obj);
//CriBool app_execute_main(AppObj* app_obj);
CriBool app_execute_main();

//3D音源版
CriBool app3d_atomex_initialize(App3dObj* app3d_obj, CueInfo* cue_list, CriSint32 cue_list_count, const CriChar8* acf_file, const CriChar8* acb_path, const CriChar8* awb_file);
CriBool app3d_atomex_finalize(App3dObj* app3d_ob);
CriBool app3d_atomex_start(App3dObj* app3d_obj);
CriBool app3d_atomex_stop_player(App3dObj* app3d_obj);
CriBool app3d_atomex_stop_cue(App3dObj* app3d_obj);
//CriBool app3d_execute_main(App3dObj* app3d_obj);

//3Dポジションの更新
void sound3DUpdate(App3dObj* app3d_obj);

//3Dポジションのセット
void SetSourcePosition(App3dObj* app3d_obj, int num);
void SetListenerPosition(App3dObj* app3d_obj);


//3Dハンドルの生成
void CreateSource(App3dObj* app3d_obj, XMFLOAT3 pos, int sound_num, float destroyTime = 1.5f);


//3Dハンドルの削除
void DeleteSource(App3dObj* app3d_obj, int num);

//3次元座標の型変換
void SwitchSourcePosition(App3dObj* app3d_obj, XMFLOAT3 pos, int num);
void SwitchListenerPosition(App3dObj* app3d_obj, XMFLOAT3 pos);

static void* user_alloc_func(void* obj, CriUint32 size);
static void user_free_func(void* obj, void* ptr);
//static void user_free_func(void* ptr);


void user_error_callback_func(const CriChar8* errid, CriUint32 p1, CriUint32 p2, CriUint32* parray);
//void user_error_callback_func(const CriChar8* errid, CriUint32 p1, CriUint32 p2);

void SetUI_Cue_index(AppObj* app_obj, int cueID);
void SetUI_Cue_index3d(App3dObj* app3d_obj, int cueID);


//サウンドの再生
void BGMStart(AppObj* app_obj, int sound_num);
void SEStart(AppObj* app_obj, int sound_num);
void SEStart(App3dObj* app3d_obj, int sound_num);

//サウンドの停止
void BGMStop(AppObj* app_obj);
void SEstop(AppObj* app_obj);
void SEstop(App3dObj* app3d_obj);

#endif //CRIPLAYER_H

