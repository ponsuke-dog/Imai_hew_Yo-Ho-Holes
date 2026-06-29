/*===============================================================


        サウンドデータの設定 [CRIPlayer.cpp]

                                        Author : shusuke katori
                                        Date   : 2024/2/14

==================================================================*/

#include "Criplayer.h"
#include <stdlib.h>





CriBool app_atomex_initialize(AppObj* app_obj, CueInfo* cue_list, CriSint32 cue_list_count, const CriChar8* acf_file, const CriChar8* acb_file, const CriChar8* awb_file) {
    /* キューリスト設定 */
    app_obj->cue_list = cue_list;
    app_obj->cue_list_count = cue_list_count;
    app_obj->ui_cue_index = 0;
    /* 未取得なプレイバックID(Voiceキュー再生時のみ取得) */
    app_obj->playback_id = 0;

    criErr_SetCallback(user_error_callback_func);

    /* メモリアロケータの登録 */
    criAtomEx_SetUserAllocator(user_alloc_func, user_free_func, NULL);

    /* ライブラリの初期化 */
    CriAtomExConfig_WASAPI lib_config;
    CriFsConfig fs_config;
    criAtomEx_SetDefaultConfig_WASAPI(&lib_config);
    criFs_SetDefaultConfig(&fs_config);
    lib_config.atom_ex.max_virtual_voices = MAX_VIRTUAL_VOICE;
    lib_config.hca_mx.output_sampling_rate = SAMPLINGRATE_HCAMX;
    fs_config.num_loaders = MAX_CRIFS_LOADER;
    lib_config.atom_ex.fs_config = &fs_config;
    criAtomEx_Initialize_WASAPI(&lib_config, NULL, 0);

    /* D-Basの作成（最大ストリーム数はここで決まります） */
    app_obj->dbas_id = criAtomDbas_Create(NULL, NULL, 0);

    /* ボイスプールの作成 */
    CriAtomExStandardVoicePoolConfig standard_vpool_config;
    criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&standard_vpool_config);
    standard_vpool_config.num_voices = MAX_VOICE;
    standard_vpool_config.player_config.streaming_flag = CRI_TRUE;
    app_obj->standard_voice_pool = criAtomExVoicePool_AllocateStandardVoicePool(&standard_vpool_config, NULL, 0);

    /* ACFファイルの読み込みと登録 */
    app_obj->acf_file_path = acf_file;
    criAtomEx_RegisterAcfFile(NULL, app_obj->acf_file_path, NULL, 0);

    /* ACBファイルの読み込み */
    app_obj->acb_file_path = acb_file;
    app_obj->awb_file_path = awb_file;
    app_obj->acb_hn = criAtomExAcb_LoadAcbFile(NULL, app_obj->acb_file_path, NULL, app_obj->awb_file_path, NULL, 0);
    if (app_obj->acb_hn == NULL) {
        printf("Failed to load ACB or AWB file.\n");
        printf("ACB File: %s\n", app_obj->acb_file_path);
        printf("AWB File: %s\n", app_obj->awb_file_path);
        return CRI_FALSE;
    }

    /* プレーヤの作成 */
    app_obj->player = criAtomExPlayer_Create(NULL, NULL, 0);

    return CRI_TRUE;
}

CriBool app_atomex_finalize(AppObj* app_obj) {
    /* ACBハンドルの破棄 */
    if (app_obj->acb_hn != NULL) {
        criAtomExAcb_Release(app_obj->acb_hn);
        app_obj->acb_hn = NULL;
    }

    /* ボイスプールの破棄 */
    if (app_obj->standard_voice_pool != NULL) {
        criAtomExVoicePool_Free(app_obj->standard_voice_pool);
        app_obj->standard_voice_pool = NULL;
    }

    /* プレイヤーの破棄 */
    if (app_obj->player != NULL) {
        criAtomExPlayer_Destroy(app_obj->player);
        app_obj->player = NULL;
    }

    return CRI_TRUE;
}

CriBool app_atomex_start(AppObj* app_obj) {
    /* キューIDの指定 */
    CriAtomExCueId start_cue_id = app_obj->cue_list[app_obj->ui_cue_index].id;
    criAtomExPlayer_SetCueId(app_obj->player, app_obj->acb_hn, start_cue_id);
    /* 再生の開始 */
    CriAtomExPlaybackId playback_id = criAtomExPlayer_Start(app_obj->player);
    return CRI_TRUE;
}

CriBool app_atomex_stop_player(AppObj* app_obj) {
    /* プレーヤの停止 */
    criAtomExPlayer_Stop(app_obj->player);

    return CRI_TRUE;
}

 CriBool app_atomex_stop_cue(AppObj* app_obj) {
    /* 特定の再生音のみ停止 */
    criAtomExPlayback_Stop(app_obj->playback_id);

    return CRI_TRUE;
}

//CriBool app_execute_main(AppObj* app_obj)
CriBool app_execute_main() {
    /* サーバ処理の実行 */
    criAtomEx_ExecuteMain();
    return CRI_TRUE;
}


CriBool app3d_atomex_initialize(App3dObj* app3d_obj, CueInfo* cue_list, CriSint32 cue_list_count, const CriChar8* acf_file, const CriChar8* acb_file, const CriChar8* awb_file)
{
    /* キューリスト設定 */
    app3d_obj->cue_list = cue_list;
    app3d_obj->cue_list_count = cue_list_count;
    app3d_obj->ui_cue_index = 0;
    /* 未取得なプレイバックID(Voiceキュー再生時のみ取得) */
    app3d_obj->playback_id = 0;

    criErr_SetCallback(user_error_callback_func);

    /* メモリアロケータの登録 */
    criAtomEx_SetUserAllocator(user_alloc_func, user_free_func, NULL);

    /* ライブラリの初期化 */
    CriAtomExConfig_WASAPI lib_config;
    CriFsConfig fs_config;
    criAtomEx_SetDefaultConfig_WASAPI(&lib_config);
    criFs_SetDefaultConfig(&fs_config);
    lib_config.atom_ex.max_virtual_voices = MAX_VIRTUAL_VOICE;
    lib_config.hca_mx.output_sampling_rate = SAMPLINGRATE_HCAMX;
    fs_config.num_loaders = MAX_CRIFS_LOADER;
    lib_config.atom_ex.fs_config = &fs_config;
    criAtomEx_Initialize_WASAPI(&lib_config, NULL, 0);

    /* D-Basの作成（最大ストリーム数はここで決まります） */
    app3d_obj->dbas_id = criAtomDbas_Create(NULL, NULL, 0);

    /* ボイスプールの作成 */
    CriAtomExStandardVoicePoolConfig standard_vpool_config;
    criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&standard_vpool_config);
    standard_vpool_config.num_voices = MAX_VOICE;
    standard_vpool_config.player_config.streaming_flag = CRI_TRUE;
    app3d_obj->standard_voice_pool = criAtomExVoicePool_AllocateStandardVoicePool(&standard_vpool_config, NULL, 0);

    /* ACFファイルの読み込みと登録 */
    app3d_obj->acf_file_path = acf_file;
    criAtomEx_RegisterAcfFile(NULL, app3d_obj->acf_file_path, NULL, 0);

    /* ACBファイルの読み込み */
    app3d_obj->acb_file_path = acb_file;
    app3d_obj->awb_file_path = awb_file;
    app3d_obj->acb_hn = criAtomExAcb_LoadAcbFile(NULL, app3d_obj->acb_file_path, NULL, app3d_obj->awb_file_path, NULL, 0);
    if (app3d_obj->acb_hn == NULL) {
        printf("Failed to load ACB or AWB file.\n");
        printf("ACB File: %s\n", app3d_obj->acb_file_path);
        printf("AWB File: %s\n", app3d_obj->awb_file_path);
        return CRI_FALSE;
    }

    /* プレーヤの作成 */
    app3d_obj->player = criAtomExPlayer_Create(NULL, NULL, 0);

    //ソースリストの作成
    app3d_obj->source_list = criAtomEx3dSourceList_Create(NULL, NULL, 0);

    //リスナーの作成
    app3d_obj->listener = criAtomEx3dListener_Create(NULL, NULL, 0);

    //3Dポジション
    app3d_obj->listenerPos.x = 0.0f;
    app3d_obj->listenerPos.y = 0.0f;
    app3d_obj->listenerPos.z = 0.0f;

    for (int i = 0; i < SOURCE_HANDLE_MAX; i++)
    {
        app3d_obj->destroy_time[i] = 0.0f;
    }

    //プレーヤーにソースとリスナーのハンドルを設定
    criAtomExPlayer_Set3dSourceListHn(app3d_obj->player, app3d_obj->source_list);
    criAtomExPlayer_Set3dListenerHn(app3d_obj->player, app3d_obj->listener);

    return CRI_TRUE;
}

CriBool app3d_atomex_finalize(App3dObj* app3d_obj)
{
    /* ACBハンドルの破棄 */
    if (app3d_obj->acb_hn != NULL) {
        criAtomExAcb_Release(app3d_obj->acb_hn);
        app3d_obj->acb_hn = NULL;
    }

    /* ボイスプールの破棄 */
    if (app3d_obj->standard_voice_pool != NULL) {
        criAtomExVoicePool_Free(app3d_obj->standard_voice_pool);
        app3d_obj->standard_voice_pool = NULL;
    }

    /* ソースの破棄 */
    for (int i = 0; i < SOURCE_HANDLE_MAX; i++)
    {

        if (app3d_obj->source[i] != NULL) {
            criAtomEx3dSource_Destroy(app3d_obj->source[i]);
            app3d_obj->source[i] = NULL;
        }
    }

    /* リスナーの破棄 */
    if (app3d_obj->listener != NULL) {
        criAtomEx3dListener_Destroy(app3d_obj->listener);
        app3d_obj->listener = NULL;
    }

    //ソースリストの破棄
    if (app3d_obj->source_list != NULL)
    {
        criAtomEx3dSourceList_Destroy(app3d_obj->source_list);
        app3d_obj->source_list = NULL;
    }

    /* プレイヤーの破棄 */
    if (app3d_obj->player != NULL) {
        criAtomExPlayer_Destroy(app3d_obj->player);
        app3d_obj->player = NULL;
    }



    return CRI_TRUE;
}

CriBool app3d_atomex_start(App3dObj* app3d_obj)
{
    /* キューIDの指定 */
    CriAtomExCueId start_cue_id = app3d_obj->cue_list[app3d_obj->ui_cue_index].id;
    criAtomExPlayer_SetCueId(app3d_obj->player, app3d_obj->acb_hn, start_cue_id);
    /* 再生の開始 */
    CriAtomExPlaybackId playback_id = criAtomExPlayer_Start(app3d_obj->player);
    return CRI_TRUE;
}

CriBool app3d_atomex_stop_player(App3dObj* app3d_obj)
{
    /* プレーヤの停止 */
    criAtomExPlayer_Stop(app3d_obj->player);

    return CRI_TRUE;
}

CriBool app3d_atomex_stop_cue(App3dObj* app3d_obj)
{
    /* 特定の再生音のみ停止 */
    criAtomExPlayback_Stop(app3d_obj->playback_id);

    return CRI_TRUE;
}

//CriBool app3d_execute_main(App3dObj* app3d_obj)
//{
//    /* サーバ処理の実行 */
//    criAtomEx_ExecuteMain();
//    return CRI_TRUE;
//}


//3D音源の更新
void sound3DUpdate(App3dObj* app3d_obj)
{



    //3Dハンドルの更新
    for (int i = 0; i < SOURCE_HANDLE_MAX; i++)
    {
        criAtomEx3dSource_Update(app3d_obj->source[i]);
    }


    criAtomEx3dListener_Update(app3d_obj->listener);

    //3D音源全体の更新
    criAtomExPlayer_UpdateAll(app3d_obj->player);
}


//3Dポジションのセット
void SetSourcePosition(App3dObj* app3d_obj, int num)
{

    criAtomEx3dSource_SetPosition(app3d_obj->source[num], &app3d_obj->sourcePos[num]);
}


void SetListenerPosition(App3dObj* app3d_obj)
{

    criAtomEx3dListener_SetPosition(app3d_obj->listener, &app3d_obj->listenerPos);
}

//3Dソースハンドルの生成
void CreateSource(App3dObj* app3d_obj, XMFLOAT3 pos, int sound_num, float destroyTime)
{

    for (int i = 0; i < SOURCE_HANDLE_MAX; i++)
    {

        if (app3d_obj->source[i] == NULL)
        {
            app3d_obj->source[i] = criAtomEx3dSource_Create(NULL, NULL, 0);

            app3d_obj->destroy_time[i] = destroyTime;

            SwitchSourcePosition(app3d_obj, pos, i);

            criAtomEx3dSourceList_Add(app3d_obj->source_list, app3d_obj->source[i]);

            SEStart(app3d_obj, sound_num);

            return;
        }
    }

    return;
}

//3Dソースハンドルの削除
void DeleteSource(App3dObj* app3d_obj, int num)
{

    criAtomEx3dSourceList_Remove(app3d_obj->source_list, app3d_obj->source[num]);

    criAtomEx3dSource_Destroy(app3d_obj->source[num]);
    app3d_obj->source[num] = NULL;
}

void SwitchSourcePosition(App3dObj* app3d_obj, XMFLOAT3 pos, int num)
{


    app3d_obj->sourcePos[num].y = pos.y;
    app3d_obj->sourcePos[num].z = pos.z;
    app3d_obj->sourcePos[num].x = pos.x;

    SetSourcePosition(app3d_obj, num);
}


void SwitchListenerPosition(App3dObj* app3d_obj, XMFLOAT3 pos)
{

    app3d_obj->listenerPos.x = pos.x;
    app3d_obj->listenerPos.y = pos.y;
    app3d_obj->listenerPos.z = pos.z;

    SetListenerPosition(app3d_obj);
}



void user_error_callback_func(const CriChar8* errid, CriUint32 p1, CriUint32 p2, CriUint32* parray){
//void user_error_callback_func(const CriChar8* errid, CriUint32 p1, CriUint32 p2) {
    const CriChar8* errmsg;
    errmsg = criErr_ConvertIdToMessage(errid, p1, p2);
    printf("%s\n", errmsg);
}

void SetUI_Cue_index(AppObj* app_obj, int cueID)
{
    app_obj->ui_cue_index = cueID;
}

void SetUI_Cue_index3d(App3dObj* app3d_obj, int cueID)
{
    app3d_obj->ui_cue_index = cueID;
}

void* user_alloc_func(void* obj, CriUint32 size) {
    void* ptr;
    ptr = malloc(size);
    return ptr;
}

void user_free_func(void* obj, void* ptr)
//void user_free_func( void* ptr)
{
    free(ptr);
}


//サウンドの再生
void BGMStart(AppObj* app_obj, int sound_num)
{

    SetUI_Cue_index(app_obj, sound_num);
    app_atomex_start(app_obj);
}

void SEStart(AppObj* app_obj, int sound_num)
{

    SetUI_Cue_index(app_obj, sound_num);
    app_atomex_start(app_obj);
}

void SEStart(App3dObj* app3d_obj, int sound_num)
{

    SetUI_Cue_index3d(app3d_obj, sound_num);
    app3d_atomex_start(app3d_obj);
}


//サウンドの停止
void BGMStop(AppObj* app_obj)
{
    app_atomex_stop_player(app_obj);
}

void SEstop(AppObj* app_obj)
{
    app_atomex_stop_player(app_obj);
}

void SEstop(App3dObj* app3d_obj)
{
    app3d_atomex_stop_player(app3d_obj);
}