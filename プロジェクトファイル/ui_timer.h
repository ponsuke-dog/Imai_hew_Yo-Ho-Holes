/*==============================================================================

   [timer.h]
														 Author : Imai Hayato
														 Date   : //
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef TIMER_H
#define TIMER_H

class Timer
{

private:
	int m_Texture = -1;
	int m_Texture_time = -1;
	int m_TextureAttention = -1;
	int m_maxtime = 0;

	int m_Time = 0;

	float m_AttentionAlpha = 0.0f;
	float m_AttentionPositionX = 0.0f;
	int m_AttentionTexPosX = 0;



	XMFLOAT2 m_Position = XMFLOAT2(0.0f, 0.0f);
	XMFLOAT2 m_Size = XMFLOAT2(0.0f, 0.0f);

	bool m_Attentionflg = false;
public:
	Timer();
	~Timer();
	void Update();
	void Draw();

	void SetTimer(int time);
};



#endif // !TIMER_H