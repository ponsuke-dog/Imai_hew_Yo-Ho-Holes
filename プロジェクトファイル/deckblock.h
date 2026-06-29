//	deckblock.h
//	
//
//	2024.11.19			ポンスケ
#ifndef DECKBLOCK_H
#define DECKBLOCK_H

#include "object_3d.h"
#include "packet.h"

class BillboardObject;
class Effect3D;

typedef enum
{
	DECKTYPE_NONE = -1,
	DECKTYPE_WOOD,				// 0
	DECKTYPE_IRON,				// 1
	DECKTYPE_CANNON,			// 2
	DECKTYPE_GATHER,			// 3
	DECKTYPE_ENHANCE,			// 4
	DECKTYPE_REPAIR_STORAGE,	// 5
	DECKTYPE_CANNON_STORAGE,	// 6
	DECKTYPE_ENHANCE_STORAGE,	// 7
	DECKTYPE_UNBREAKEABLE,		// 8
}DeckType;


// クラス
class DeckBlock : public Object3D
{
private:
	int m_id;

	DeckType m_decktype;	// 甲板のタイプ
	float m_hitpoint;		// 甲板の体力
	bool m_buse;			// 甲板の使用フラグ

	int m_RepairCount = 0;	//修復回数

	BillboardObject* m_BillboardObject = nullptr;
	Effect3D* m_pEffectobject = nullptr;

public:

	DeckBlock() = delete;
	DeckBlock(Ship* ship,XMFLOAT3 pos, DeckType decktype,int id);
	~DeckBlock();

	void Update()override;
	void Draw()const override;

	DeckType GetDeckType();		// 甲板のタイプのゲッター
	float GetHitPoint();		// 甲板の体力のゲッター
	bool GetUse();				// 甲板の使用フラグのゲッター
	int GetId();

	void SetDeckType(DeckType Type);
	void SetHitPoint(float HP);
	void SetUse(bool use);
	
	void Damage(float damage);
	void Damage(BlockData data);

	void Repair();
	void Repair(float hp);

};
#endif // !DECKBLOCK_H