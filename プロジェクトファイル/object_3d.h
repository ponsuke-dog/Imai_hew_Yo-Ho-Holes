//	object_3d.h
//	3dオブジェクトの基底クラス
//
//	2024.12.20			大山凌太朗

#ifndef OBJECT_3D_H
#define OBJECT_3D_H

#include "directx.h"

typedef enum
{
	OBJECTTYPE_NONE = -1,
	
	OBJECTTYPE_PLAYER,				// 0
	OBJECTTYPE_DECK,				// 1
	OBJECTTYPE_CANNON,				// 2
	OBJECTTYPE_GATHER,				// 3
	OBJECTTYPE_ENHANCE,				// 4
	OBJECTTYPE_STORAGE,				// 5
	OBJECTTYPE_NET,					// 6
	OBJECTTYPE_FLOTSAM,				// 7
	OBJECTTYPE_CANNONBALL,			// 8
	OBJECTTYPE_EXPLOSION,			// 9
	OBJECTTYPE_AIM,					// 10
	OBJECTTYPE_EFFECT,				// 11

	OBJECTTYPE_MAX,					// 12
}ObjectType;

class Ship;

class Object3D
{
private:
	XMFLOAT3 m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);	// ポジション
	XMFLOAT3 m_Size = XMFLOAT3(1.0f, 1.0f, 1.0f);		// サイズ
	XMFLOAT3 m_Rotation = XMFLOAT3(0.0f,0.0f,0.0f);		// ローテーション
	ObjectType m_ObjectType = OBJECTTYPE_NONE;			// オブジェクトタイプ
	bool m_DestroyFlag = false;							// 削除フラグ

	Ship* m_pShip = nullptr;								//船のポインター


public:
	Object3D() = default;
	Object3D(XMFLOAT3 pos, XMFLOAT3 size,ObjectType type) :m_Position(pos), m_Size(size),m_ObjectType(type){}
	Object3D(Ship* ship,ObjectType type,XMFLOAT3 pos = {0.0f, 0.0f, 0.0f}, XMFLOAT3 size = {1.0f,1.0f,1.0f}) :m_pShip(ship), m_ObjectType(type), m_Position(pos), m_Size(size) {}
	virtual ~Object3D(){};

	//純粋仮想関数
	virtual void Update() = 0;
	virtual void Draw()const = 0;

	void SetPosition(XMFLOAT3 pos) { m_Position = pos; }
	const XMFLOAT3 GetPosition()const { return m_Position; }

	void SetSize(XMFLOAT3 size) { m_Size = size; }
	const XMFLOAT3 GetSize()const { return m_Size; }

	void SetRotation(XMFLOAT3 rot) { m_Rotation = rot; }
	const XMFLOAT3 GetRotation()const { return m_Rotation; }

	void SetObjectType(ObjectType type) { m_ObjectType = type; }
	const ObjectType GetObjectType()const { return m_ObjectType; }

	void SetDestroyFlag(bool destroy) { m_DestroyFlag = destroy; }
	const bool GetDestroy()const { return m_DestroyFlag; }

	void SetShip(Ship* pShip) { m_pShip = pShip; }
	Ship* GetShip()const { return m_pShip; }
};

#endif // OBJECT_3D_H