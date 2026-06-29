//********************************************************************************
//
// 修理アクションのクラス[action_repair.h] 
// 
//															Author : Riugo Honda
//															Date   :2024/12/29
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/29
//********************************************************************************


#ifndef _ACTION_REPAIR_H_
#define _ACTION_REPAIR_H_

class DeckBlock;
class Player;
class Enhance;
class Effect3D;

class RepairAction
{
private:
	//ポインター
	Player* m_pPlayer = nullptr;
	Enhance* m_pEnhance = nullptr;
	//Effect3D* m_Effect = nullptr;

	//その他変数
	int m_FrameCount = 0;
	int m_RepairCount = 0;

public:
	RepairAction() = delete;
	RepairAction(Player* p_player, Enhance* p_enhance);
	
	~RepairAction();

	void Update();

	void SetRepairBlock(DeckBlock* p_DeckBlock);
	
	void SetRepairCount();
	void ResetRepairCount();

	int GetRepairCount();

	Enhance* GetEnhance();
};

#endif // !_ACTION_REPAIR_H_
