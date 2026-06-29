/*==============================================================================

   [.h]
														 Author : Imai Hayato
														 Date   : //
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef EVENT_COMMAND_H
#define EVENT_COMMAND_H

#include "directx.h"
#include "event.h"



class CommandEvent :public Event
{
private:
	int m_Count[2] = {};		// 両チームのカウント
	int m_Time = 0;
	int m_FrameCount = 0;
	int m_Texture = -1;
	int m_ButtonTex[2] = {};
	int m_InstructionTex = -1;
	int m_EventBGTexture = -1;
	int m_GageTexture = -1;
	int m_GageEffect[2] = {};
	int m_SharkTexture = -1;
	int m_SplashTexture = -1;
	int m_CharaTex = -1;
	int m_RodTex = -1;
	int m_CharaAnimation[2] = {};
	int m_SharkAnimation = -1;
	XMFLOAT2 m_CharaPosition = XMFLOAT2(0.0f, 0.0f);
	XMFLOAT2 m_CharaVelocity = XMFLOAT2(0.0f, 0.0f);
	XMFLOAT2 m_SharkPos = XMFLOAT2(0, 0);
	XMFLOAT2 m_SharkVelocity = XMFLOAT2(0.0f, 0.0f);
	int m_Speed = 0;
	float m_SharkRot = 0.0f;
	float m_CountSize = 0.0f;
	float m_ButtonFrame[2][2] = {};
	float m_WrongButtonMove[2][2] = {};
	float m_WrongButtonMove_online = 0.0f;


	bool m_CharaReverseflg = false;

	bool m_Drawflg = false;
	bool m_SharkReverseflg = false;

	bool m_ButtonInputFlg[2][2] = {};
	int m_PushWait[2][2] = {};
	int m_PushWait_online = 0;

	int m_ButtonPush[2][2] = {};

	void ButtonSlot();
	void ButtonSlot(int shipID,int charaID);
	void ButtonDraw(int windownum);
	void KeyBordInput(int i,int j);// キーボード入力

	int m_TexCut[2][2] = {};
	int m_TexCut_online = -1;
	WORD m_Button_online = -1;
	WORD m_WrongButton_online[3] = {};
	WORD m_Button[2][2] = {};
	WORD m_WrongButton[2][2][3] = {};


	int m_Key[2][2] = {};

	void CountUpdate();
	void EventUpdate();
	void AnimUpdate();
	void DamageUpdate();
	int IDSearch();
public:
	CommandEvent(std::vector<Ship*>pShip, std::vector<Controller*>pController);
	~CommandEvent();

	void Update() override;
	void Draw(int windownum)override;
	void Damage(int shipid, int damage) override;

};


#endif // !EVENT_COMMAND_H