/*==============================================================================

   [ui_gather.h]
														 Author : Imai Hayato
														 Date   : //
--------------------------------------------------------------------------------

==============================================================================*/

#ifndef UI_GATHER_H
#define UI_GATHER_H

class GatherUI
{
private:
	int m_Texture[4] = {};
	int m_AnimationTime = 0;
	int m_FrameCount = 0;
	int m_SizeRaito = 0;
	int m_BGBlack = -1;
	float m_splitmove = 0;
public:
	GatherUI();
	~GatherUI();

	void Update();
	void Draw();
};

#endif // !UI_GATHER_H