//********************************************************************************
//
// 強化アクション[action_enhance.h] 
// 
//															Author : Hiroki Nakajima
//															Date   : 2024/12/12
// -------------------------------------------------------------------------------
//															Last Edited:2025/01/17
//********************************************************************************
#ifndef ACTION_ENHANCE_H
#define ACTION_ENHANCE_H

class Player;
class EnhanceUI;

class EnhanceAction
{
private:
	EnhanceUI* m_EnhanceUI = nullptr;
	Player* m_Player = nullptr;
	//Controller* m_Controller??????
	
public:
	EnhanceAction() = delete;
	EnhanceAction(Player* player);
	~EnhanceAction();

	void Update();
	void Draw();

private: //コードを見やすくするための関数
	bool CheckLevel(int level);
};

#endif // ACTION_ENHANCE_H

