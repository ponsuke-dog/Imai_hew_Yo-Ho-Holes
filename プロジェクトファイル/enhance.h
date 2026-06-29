//********************************************************************************
//
// ‹­‰»[enhance.h] 
// 
//															Author : Hiroki Nakajima
//															Date   : 2024/??
// -------------------------------------------------------------------------------
//															Last Edited:2024/12/25
//********************************************************************************

#ifndef ENHANCE_H
#define ENHANCE_H

#include "object_3d.h"

class BillboardObject;

class Enhance : public Object3D
{
private:
	int m_CannonLevel = 0;
	int m_GatherLevel = 0;
	int m_RepairLevel = 0;
	int m_Stock = 0;
	bool m_IsUsingEnhance = false;
	BillboardObject* m_BillboardObject = nullptr;

public:
	Enhance() = delete;
	Enhance(XMFLOAT3 pos, Ship* pship);
	~Enhance();

	void Update() override;
	void Draw() const override;

	int GetCannonLevel();
	void SetCannonLevel();
	void SetCannonLevel(int level);
	int GetGatherLevel();
	void SetGatherLevel();
	void SetGatherLevel(int level);
	int GetRepairLevel();
	void SetRepairLevel();
	void SetRepairLevel(int level);

	int GetStock();
	void AddStock(int stock);
	void DecreaseStock(int stock);
	void SetStock(int stock);

	void SetUsingEnhance(bool buse);
	bool GetUsingEnhance();

	void Reset();
};

#endif // ENHANCE_H