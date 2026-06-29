/*===============================================================


		サウンド処理 [cri.h]

										Author : shusuke katori
										Date   : 2024/2/14

==================================================================*/
#ifndef CRI_H

#define CRI_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <cri_le_atom_ex.h>
#include "Criplayer.h"

#include "BGM.h"
#include "SE.h"
#include "SE3D.h"


static CueInfo g_cue_list_BGM[] =
{
	CRI_BGM_BGM_TITLE,
	CRI_BGM_BGM_MODE_SELECT,
	CRI_BGM_BGM_GAME,
	CRI_BGM_BGM_RESULT_WIN,
	CRI_BGM_BGM_RESULT_LOSE,
	CRI_BGM_BGM_RESULT_DRAW,
	CRI_BGM_環境音_波,
	CRI_BGM_BGM_STANDBY,
	
};

static CueInfo g_cue_list_SE[] =
{
	CRI_SE_クリック音,
	CRI_SE_ゲームスタート,
	CRI_SE_警報が鳴る,
	CRI_SE_コマンド成功音,
	CRI_SE_コマンド失敗音,
	CRI_SE_ボタン連打,
	CRI_SE_イベント開始,
	CRI_SE_気を溜める1,
	CRI_SE_魚が水面で暴れる,
	CRI_SE_笛,
	CRI_SE_カーソル移動,
	CRI_SE_決定ボタン,
	CRI_SE_キャンセルボタン,
	CRI_SE_STANDBY,
	CRI_SE_COUNTDOWN,
	CRI_SE_イベント爆発,
	CRI_SE_イベント吹っ飛ぶ,
	CRI_SE_パワーアップ,
};

static CueInfo g_cue_list_SE3D[] =
{
	CRI_SE3D_爆発,
	CRI_SE3D_銃火器・大砲,
	CRI_SE3D_修復,
	CRI_SE3D_ネット発射,
	CRI_SE3D_ネット着水,
	CRI_SE3D_ストレージ,
	CRI_SE3D_強化画面,
	CRI_SE3D_吹っ飛ぶ,
};

void CRIInitialize(void);
void CRIFinalize(void);
void CRIUpdate(void);
void CRIDraw(void);

AppObj* GetAppBGM(void);
AppObj* GetAppSE(void);
App3dObj* GetApp3dSE(void);
App3dObj* GetApp3dSE(int num);


#endif //CRI_H

