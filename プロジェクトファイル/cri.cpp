/*===============================================================


		サウンド処理 [cri.h]

										Author : shusuke katori
										Date   : 2024/2/14

==================================================================*/

#include "mode.h"
#include "cri.h"

AppObj app_BGM = { 0 };
AppObj app_SE = { 0 };
App3dObj app3d_SE = { 0 };

App3dObj app3d_SEs[4] =
{
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
};


void CRIInitialize(void)
{

	//プレーヤーの初期化
	app_atomex_initialize(&app_BGM, g_cue_list_BGM, sizeof(g_cue_list_BGM) / sizeof(CueInfo), ACF_FILE_PATH_BGM, ACB_FILE_PATH_BGM, AWB_FILE_PATH_BGM);
	app_atomex_initialize(&app_SE, g_cue_list_SE, sizeof(g_cue_list_SE) / sizeof(CueInfo), ACF_FILE_PATH_SE, ACB_FILE_PATH_SE, AWB_FILE_PATH_SE);
	app3d_atomex_initialize(&app3d_SE, g_cue_list_SE3D, sizeof(g_cue_list_SE3D) / sizeof(CueInfo), ACF_FILE_PATH_SE3D, ACB_FILE_PATH_SE3D, AWB_FILE_PATH_SE3D);

	//ローカル用
	for (int i = 0; i < 4; i++)
	{
		app3d_atomex_initialize(&app3d_SEs[i], g_cue_list_SE3D, sizeof(g_cue_list_SE3D) / sizeof(CueInfo), ACF_FILE_PATH_SE3D, ACB_FILE_PATH_SE3D, AWB_FILE_PATH_SE3D);
	}
}

void CRIFinalize(void)
{

	app_atomex_finalize(&app_BGM);
	app_atomex_finalize(&app_SE);
	app3d_atomex_finalize(&app3d_SE);

	for (int i = 0; i < 4; i++)
	{
		app3d_atomex_finalize(&app3d_SEs[i]);
	}
}

void CRIUpdate(void)
{	

	if (GetGameMode() == GAMEMODE_ONLINE)
	{


		for (int i = 0; i < SOURCE_HANDLE_MAX; i++)
		{

			if (app3d_SE.source[i] != NULL)
			{

				//時間経過でソースを破棄する
				if (app3d_SE.destroy_time[i] < 0.0f)
				{
					DeleteSource(&app3d_SE, i);
				}
				else
				{
					float dt = 1.0f / 60.0f;
					app3d_SE.destroy_time[i] -= dt;
				}
			}
		}

		//3D音源全体の更新
		sound3DUpdate(&app3d_SE);
	}
	else //ローカル版
	{

		for (int i = 0; i < 4; i++)
		{


			for (int j = 0; j < SOURCE_HANDLE_MAX; j++)
			{
				//時間経過でソースを破棄する
				if (app3d_SEs[i].source[j] != NULL)
				{

					if (app3d_SEs[i].destroy_time[j] < 0.0f)
					{
						DeleteSource(&app3d_SEs[i], j);
					}
					else
					{
						float dt = 1.0f / 60.0f;
						app3d_SEs[i].destroy_time[j] -= dt;
					}
				}
			}

			//3D音源全体の更新
			sound3DUpdate(&app3d_SEs[i]);
		}
	}
	

	

}

void CRIDraw(void)
{
}

AppObj* GetAppBGM(void)
{
	return &app_BGM;
}

AppObj* GetAppSE(void)
{
	return &app_SE;
}

App3dObj* GetApp3dSE(void)
{
	return &app3d_SE;
}

App3dObj* GetApp3dSE(int num)
{
	return &app3d_SEs[num];
}



