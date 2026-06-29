//	storage.h
//	ストレージクラス
//
//	2024.12.20			中島寛樹

#ifndef STORAGE_H
#define STORAGE_H

#include "object_3d.h"
#include "packet.h"

class BillboardObject;

//ストレージタイプ

class Storage : public Object3D
{
private:
	StorageType m_Type;  //ストレージタイプ
	int m_Stock;		 //在庫数
	BillboardObject* m_BillboardObject = nullptr;

public:
	Storage() = delete;
	Storage(StorageType type, XMFLOAT3 position, Ship* pship);
	~Storage();

	void Update() override;
	void Draw() const override;

	void Increase(int increment); //増やす
	void Decrease(int decrement); //減らす

	StorageType GetType();
	void SetType(StorageType type);
	int GetStock();
	void SetStock(int stock);
};

#endif // STORAGE_H