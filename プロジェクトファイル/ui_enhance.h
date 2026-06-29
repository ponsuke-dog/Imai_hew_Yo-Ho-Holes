//********************************************************************************
//
// 強化UI[ui_enhance.h] 
// 
//															Author : Hiroki Nakajima
//															Date   : 2024/12/19
// -------------------------------------------------------------------------------
//															Last Edited:2025/01/17
//********************************************************************************
#ifndef UI_ENHANCE_H
#define UI_ENHANCE_H

#include "directx.h"


//強化タイプ
enum EnhanceType
{
	ENHANCETYPE_GATHER = 0,
	ENHANCETYPE_CANNON,
	ENHANCETYPE_REPAIR,
};

class Enhance;

class EnhanceUI
{
private:
	int m_EnhanceButtonTex = -1;
	int m_ButtonTex = -1;
	int m_ControlText = -1;
	int m_EnhanceContents = -1;
	int m_EnhanceTex = -1;
	int m_BlackTex = -1;
	int m_LevelTex = -1;
	int m_CoalsTex = -1;
	int m_RingTex = -1;
	int m_BackLineTex = -1;
	int m_GageTex = -1;
	int m_Select = 1; //強化種類の選択
	int m_Interface = 0;
	XMFLOAT2 m_SizeRaito = {0.0f,0.0f};
	bool m_DrawFlag = false;
	Enhance* m_Enhance = nullptr;
	float m_IconPos = 0.0f;
	float m_Texture_Rectification = 0;	// 座標調整値

public:
	EnhanceUI();
	~EnhanceUI();

	void MoveSelect(int direction);

	void Update();
	void Draw();

	int GetSelect();
	
	bool GetDrawFlag();
	void SwitchDrawFlag();
	int GetInterface();
	void SetInterface(int Interface);
	Enhance* GetEnhance();
	void SetEnhance(Enhance* enhance);

private: //コードを見やすくするための関数
	void DrawInterface1();
	void DrawInterface2();

};

#endif // UI_ENHANCE_H
