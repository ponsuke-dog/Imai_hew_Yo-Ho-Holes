//********************************************************************************
//
// 強化UI[ui_enhance.cpp] 
// 
//															Author : Hiroki Nakajima
//															Date   : 2024/12/19
// -------------------------------------------------------------------------------
//															Last Edited:2025/01/17
//********************************************************************************
#include "sprite.h"
#include "texture.h"
#include "configuration.h"
#include "cri.h"
#include "mode.h"
#include "enhance.h"
#include "ui_enhance.h"

static constexpr int NUMBER_OF_SELECT = 3; //選択肢の数

//===================================================
// コンストラクタ
//===================================================
EnhanceUI::EnhanceUI()
{
	{
		m_RingTex = TextureLoad(L"asset\\texture\\select_ring.png");					// 選択エフェクト
		m_BackLineTex = TextureLoad(L"asset\\texture\\enhance_backline.png");			// 強化画面2の線(?)
		m_ButtonTex = TextureLoad(L"asset\\texture\\enhance_bbutton.png");				// Bボタンで強化

		m_EnhanceButtonTex = TextureLoad(L"asset\\texture\\EnhanceIcon.png");		// 3種の強化ボタン
		m_EnhanceTex = TextureLoad(L"asset\\texture\\enhance_text.png");				// 3種の強化テキスト
		m_ControlText = TextureLoad(L"asset\\texture\\textAB.png");						// Aボタン,Bボタンでの操作説明
		m_BlackTex = TextureLoad(L"asset\\texture\\black.png");							// 背景の黒
		m_CoalsTex = TextureLoad(L"asset\\texture\\coals.png");							// 必要燃料数
		m_LevelTex = TextureLoad(L"asset\\texture\\levels.png");						// Lv
		m_EnhanceContents = TextureLoad(L"asset\\texture\\enhance_contents.png");				// 強化内容

	}
	if(	GetGameMode() == GAMEMODE_SOLO||
		GetGameMode()==GAMEMODE_LOCAL_MULTIWINDOW||
		GetGameMode()==GAMEMODE_MULTIWINDOW||
		GetGameMode() == GAMEMODE_ONLINE)
	{
		m_SizeRaito = { 1.0f,1.0f };
	}
	else
	{
		m_SizeRaito = { 1.0f,0.5f };
	}
	m_Select = 1;
	m_Interface = 0;
	m_DrawFlag = false;
	m_IconPos = 425.0f;
}

//===================================================
// デストラクタ
//===================================================
EnhanceUI::~EnhanceUI()
{
}

//===================================================
// 強化種類選択カーソルの移動
//===================================================
void EnhanceUI::MoveSelect(int direction)
{
	switch (direction)
	{
	case 0: //左に移動
		m_Select = (m_Select - 1 + NUMBER_OF_SELECT) % NUMBER_OF_SELECT;
		SEStart(GetAppSE(), CRI_SE_カーソル移動);
		break;
	case 1: //右に移動
		m_Select = (m_Select + 1) % NUMBER_OF_SELECT;
		SEStart(GetAppSE(), CRI_SE_カーソル移動);
		break;
	default:
		break;
	}

	/*
	// 選択に加算
	m_Select += (direction ? 1 : -1);

	// 0より小さくなってしまった場合
	if (m_Select < 0)
		m_Select = BUTTON_AMOUNT - 1;    // 最後のインデックスにクランプ
	m_Select %= BUTTON_AMOUNT;  // 3で割った余りでクランプ
	*/
}

//===================================================
// 更新
//===================================================
void EnhanceUI::Update()
{
}

//===================================================
// 描画
//===================================================
void EnhanceUI::Draw()
{
	if (m_Interface == 0)
		DrawInterface1(); //強化画面1
	else if (m_Interface == 1)
		DrawInterface2(); //強化画面2
}

//===================================================
// 選択している強化種類の取得
//===================================================
int EnhanceUI::GetSelect()
{
	return m_Select;
}

//===================================================
// 描画フラグの取得
//===================================================
bool EnhanceUI::GetDrawFlag()
{
	return m_DrawFlag;
}

//===================================================
// 強化画面の開閉
//===================================================
void EnhanceUI::SwitchDrawFlag()
{
	m_DrawFlag = !m_DrawFlag; //m_DrawFlagを反転

	//選択と強化画面をリセット
	m_Select = 1;
	m_Interface = 0;
}

//===================================================
// 強化画面の番号の取得
//===================================================
int EnhanceUI::GetInterface()
{
	return m_Interface;
}

//===================================================
// 強化画面の番号の設定
//===================================================
void EnhanceUI::SetInterface(int Interface)
{
	m_Interface = Interface;
}

//===================================================
// 強化の取得
//===================================================
Enhance* EnhanceUI::GetEnhance()
{
	return m_Enhance;
}

//===================================================
// 強化の設定
//===================================================
void EnhanceUI::SetEnhance(Enhance* enhance)
{
	m_Enhance = enhance;
}

//===================================================
// 強化画面1の描画
//===================================================
void EnhanceUI::DrawInterface1()
{
	static constexpr float ICON_SIZE = 270.0f; //強化種類アイコンのサイズ
	static constexpr float ICON_SPACE = ((SCREEN_WIDTH - ICON_SIZE * 3.0f) / 4.0f); //アイコンと画面端、アイコンとアイコンの隙間の幅

	static constexpr float LEVEL_TEXT_WIDTH = 175.0f; //レベルテキストの幅
	static constexpr float LEVEL_TEXT_HEIGHT = 70.0f; //レベルテキストの高さ

	SpriteSetColor(1.0f, 1.0f, 1.0f, 0.4f);
	SpriteDraw(m_BlackTex, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);
	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);

	//Aボタンで閉じる
	SpriteDraw(m_ControlText, 122.0f, SCREEN_HEIGHT - 50.0f - m_Texture_Rectification, 250.0f, 70.0f*m_SizeRaito.y, 0, 0, 800, 100);
	//Bボタンで決定
	SpriteDraw(m_ControlText, 1160.0f, SCREEN_HEIGHT - 50.0f - m_Texture_Rectification, 225.0f, 70.0f * m_SizeRaito.y, 0, 100, 800, 100);

	float icon_Rectification = 0;
	if (GetGameMode()==GAMEMODE_LOCAL_SPLITSCREEN||
		GetGameMode()==GAMEMODE_SEGMENTATION)
	{
		icon_Rectification = 125.0f;
	}
	else
	{
		icon_Rectification = 0.0f;
	}

	//選択肢の数だけループ
	for (int i = 0; i < NUMBER_OF_SELECT; i++)
	{
		float icon_size = ICON_SIZE;
		float text_width = LEVEL_TEXT_WIDTH;
		float text_height = LEVEL_TEXT_HEIGHT;

		if (i == m_Select)
		{	//選択している種類のアイコンとテキストを拡大
			icon_size *= 1.2f;
			text_width *= 1.3f;
			text_height *= 1.3f;

			//選択時のエフェクト(?)
			SpriteDraw(m_RingTex, (ICON_SPACE + ICON_SIZE / 2.0f) + ((ICON_SPACE + ICON_SIZE) * i), 250.0f , icon_size * 1.1f * m_SizeRaito.x, icon_size * 1.1f * m_SizeRaito.y);
		}
		
		// 強化種類アイコン
		SpriteDraw(m_EnhanceButtonTex, (ICON_SPACE + ICON_SIZE / 2.0f) + ((ICON_SPACE + ICON_SIZE) * i), 250.0f , icon_size*m_SizeRaito.x, icon_size * m_SizeRaito.y, i * 500, 0, 500, 500);
		
		// 強化種類テキスト
		SpriteDraw(m_EnhanceTex, (ICON_SPACE + ICON_SIZE / 2.0f) + ((ICON_SPACE + ICON_SIZE) * i), 500.0f - icon_Rectification,
			text_width, text_height * m_SizeRaito.y,
			0, i * 100, 400, 100);//強化種類テキスト
	
	}
	// 現在のLevel
	SpriteDraw(m_LevelTex, (ICON_SPACE + ICON_SIZE / 2.0f) + ((ICON_SPACE + ICON_SIZE) * 0), 580.0f - icon_Rectification,		// 回収
				200.0f * m_SizeRaito.x, 90.0f * m_SizeRaito.y,0,m_Enhance->GetGatherLevel()*100, 200, 100);
	SpriteDraw(m_LevelTex, (ICON_SPACE + ICON_SIZE / 2.0f) + ((ICON_SPACE + ICON_SIZE) * 1), 580.0f - icon_Rectification,		// 砲撃
				200.0f * m_SizeRaito.x, 90.0f * m_SizeRaito.y,0, m_Enhance->GetCannonLevel()*100, 200, 100);
	SpriteDraw(m_LevelTex, (ICON_SPACE + ICON_SIZE / 2.0f) + ((ICON_SPACE + ICON_SIZE) * 2), 580.0f - icon_Rectification,		// 修復
				200.0f * m_SizeRaito.x, 90.0f * m_SizeRaito.y,0,m_Enhance->GetRepairLevel()*100, 200, 100);

}

//===================================================
// 強化画面2の描画
//===================================================
void EnhanceUI::DrawInterface2()
{
	static constexpr float ICON_SIZE_S = 100.0f;  //アイコンサイズ(小)
	static constexpr float ICON_SIZE_L = 130.0f; //アイコンサイズ(大)

	int level = -1;
	switch (m_Select)
	{
	case ENHANCETYPE_GATHER: //回収強化
		level = m_Enhance->GetGatherLevel();
		break;
	case ENHANCETYPE_CANNON: //大砲強化
		level = m_Enhance->GetCannonLevel();
		break;
	case ENHANCETYPE_REPAIR: //修復強化
		level = m_Enhance->GetRepairLevel();
		break;
	default:
		break;
	}
	//半透明の黒背景
	SpriteSetColor(1.0f, 1.0f, 1.0f, 0.5f);

	SpriteDraw(m_BlackTex, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);

	SpriteSetColor(1.0f, 1.0f, 1.0f, 0.3f);
	//半透明の黒背景(透明度80%)
	SpriteDraw(m_BlackTex, SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT / 2, SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT);

	SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	
	float icon_Interval = 0.0f;			// アイコンの間隔調整値
	float lineraito = 0.0f;				// 棒線の長さ調整値
	float posRectification = 0.0f;
	// 2画面なら
	if (GetGameMode() == GAMEMODE_LOCAL_SPLITSCREEN ||
		GetGameMode() == GAMEMODE_SEGMENTATION)
	{
		icon_Interval = 90.0f;
		lineraito = 0.9f;
		posRectification = 50.0f;
	}
	else // 1画面なら
	{
		icon_Interval = 105.0f;
		lineraito = 1.0f;
		posRectification = 25.0f;
	}
	
	//アイコンの後ろの線
	SpriteDraw(m_BackLineTex, m_IconPos, SCREEN_HEIGHT / 2 + posRectification, 20.0f, 600.0f*lineraito);

	if (level != 5)
	{
		// 必要燃料数
		SpriteDraw(m_CoalsTex, SCREEN_WIDTH * 0.8f, 520.0f, 170.0f * m_SizeRaito.x, 70.0f * m_SizeRaito.y, 0, level * 299, 1100, 299);
	}
	//Bボタンで強化
	SpriteDraw(m_ButtonTex, SCREEN_WIDTH * 0.8f, 620.0f, 250.0f * m_SizeRaito.x, 110.0f * m_SizeRaito.y);
	//Aボタンで戻る
	SpriteDraw(m_ControlText, 122.0f, SCREEN_HEIGHT -50.0f, 250.0f, 70.0f * m_SizeRaito.y, 0, 0, 800, 100);
	//強化内容テキスト
	SpriteDraw(m_EnhanceContents, SCREEN_WIDTH * 0.8f, 300.0f,
		425.0f * m_SizeRaito.x, 120.0f * m_SizeRaito.y, 
		level * TextureGetWidth(m_EnhanceContents) / 6, m_Select * TextureGetHeight(m_EnhanceContents) / 3,
		TextureGetWidth(m_EnhanceContents) / 6, TextureGetHeight(m_EnhanceContents)/3);
	
	//次のlv
	SpriteDraw(m_LevelTex, SCREEN_WIDTH * 0.9f, 160.0f, 140.0f * m_SizeRaito.x, 100.0f * m_SizeRaito.y, 0, level * 100, 200, 100);
	//強化種類テキスト
	SpriteDraw(m_EnhanceTex, SCREEN_WIDTH * 0.765f, 160.0f, 230.0f * m_SizeRaito.x, 90.0f * m_SizeRaito.y,0,m_Select*100,400,100);
	

	{
		float icon_size = ICON_SIZE_L;
		if (level == 0)
		{	//現在のレベルのアイコンを拡大
			icon_size *= 1.1f;
			//選択時のエフェクト(?)
			SpriteDraw(m_RingTex, m_IconPos, 620.0f + posRectification, icon_size * 1.1f * m_SizeRaito.x, icon_size * 1.1f * m_SizeRaito.y);
		}
		//強化種類アイコン(大)
		SpriteDraw(m_EnhanceButtonTex, m_IconPos,  620.0f+posRectification,
			icon_size * m_SizeRaito.x, icon_size * m_SizeRaito.y,m_Select*500,0,500,500);

	}

	int icon_light = 0;
	for (int i = 0; i < 5; i++)
	{
		float icon_size = 0;
		if (GetGameMode() == GAMEMODE_LOCAL_SPLITSCREEN ||
			GetGameMode() == GAMEMODE_SEGMENTATION)
		{
			icon_size = ICON_SIZE_S + 40.0f;
		}
		else
		{
			icon_size = ICON_SIZE_S - 15.0f;
		}
		if (level == i + 1)
		{	//現在のレベルのアイコンを拡大
			icon_size *= 1.2f;
			//選択時のエフェクト(?)
			SpriteDraw(m_RingTex, m_IconPos, 480.0f - icon_Interval * i + posRectification, icon_size * 1.1f * m_SizeRaito.x , icon_size * 1.1f * m_SizeRaito.y );
		}
		if (level < i + 1)// 光らない
		{
			icon_light = 1;
		}
		else				// 光る
		{
			icon_light = 0;
		}
		//強化種類アイコン(小)
		SpriteDraw(m_EnhanceButtonTex, m_IconPos, 480.0f - icon_Interval * i + posRectification,
			icon_size* m_SizeRaito.x, icon_size* m_SizeRaito.y, m_Select * 500,
			icon_light * 500, 500, 500);

		float textmove = 0;
		if (level == i + 1)
		{
			textmove = 20.0f;
		}
		else
		{
			textmove = 0.0f;
		}
		//lv1～5
		SpriteDraw(m_LevelTex, m_IconPos +textmove + (180.0f -icon_Interval)*1.1f,  480.0f - icon_Interval * i + posRectification,
			60.0f, 35.0f * m_SizeRaito.y, 0, (i + 1) * 100, 200, 100);
		SpriteSetColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
}