//	manager_flotsam.h
//	
//
//	2024.12.20			PON

#ifndef MANAGER_FLOTSAM_H
#define MANAGER_FLOTSAM_H

#include "flotsam.h"

class Ship;

class FlotsamManager
{
private:

	Ship* m_pShip = nullptr;
	int m_Count = 0;		// 間隔のカウント 
	
public:
	FlotsamManager() = delete;
	FlotsamManager(Ship* pship);// 中心、チーム、ゲームマネージャ
	~FlotsamManager();

	void Update();

	// 漂流物の生成
	void CreateFlotsam();

};


#endif // !MANAGER_FLOTSAM_H