//********************************************************************************
//
// 収集アクションのクラス[action_gather.h] 
// 
//															Author : Riugo Honda
//															Date   :2024/12/12
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/12
//********************************************************************************

#ifndef _ACTION_GATHER_H_
#define _ACTION_GATHER_H_


class Gather;
class Aim;
class Player;
class Controller;
class Camera;

class GatherAction
{
private:
	Camera* m_pCamera = nullptr;

	Gather* m_pGather = nullptr;

	Aim* m_pAim = nullptr;

	Player* m_pPlayer = nullptr;
public:
	GatherAction() = delete;
	GatherAction(Player* p_player);

	~GatherAction();


	
	void Update() ;		//アップデート
	void Draw() const;	//ドロー
	
	void AimAction();

	void UseGatherObject(Gather* pgather);

	void SetNet(Player* p_player);

	void AimCancel();

};


#endif // !_ACTION_GATHER_H
