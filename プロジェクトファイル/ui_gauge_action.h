/*===============================================================


		ゲージアクションのUI [ui_gauge_action.h]

										Author : shusuke katori
										Date   : 2024/12/28

==================================================================*/

#ifndef UI_GAUGE_ACITION_H

#define UI_GAUGE_ACITION_H

#include "directx.h"

class UIObject;

class GaugeActionUI
{

private:
	int m_Texture[2];
	XMFLOAT2 m_positionXY[2];
	XMFLOAT2 m_sizeXY[2];
	int m_ControllersTex[3] = {};
	int m_BGblack = -1;
	int m_CannonIcon = -1;

	int m_SizeRaito = 0;
	float m_ReverseMove = 0.0f;
	
	bool m_ReverseFlg = false;

	int m_AnimationTime = 0;
	int m_FrameCount = 0;
public:

	GaugeActionUI(void);
	~GaugeActionUI(void);

	void Update();
	void Draw();

	void MoveGaugeBar(float gaugeCount);
	void MoveGaugeBar(float gaugeCount, int TeamId);

};


#endif //UI_GAUGE_ACITION_H
