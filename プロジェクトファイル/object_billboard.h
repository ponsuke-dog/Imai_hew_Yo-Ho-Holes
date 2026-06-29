/*==============================================================================

   オブジェクト用のビルボードUI　[object_billboard.h]
														 Author : Souta Fukamizu
														 Date   : 2025/02/04
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef OBJECT_BILLBOARD_H
#define OBJECT_BILLBOARD_H

#include "object_3d.h"

typedef enum
{
	BILLBOARDTYPE_NONE = -1,

	BILLBOARDTYPE_CANNON,
	BILLBOARDTYPE_GATHER,
	BILLBOARDTYPE_STORAGE,
	BILLBOARDTYPE_ENHANCE,
	BILLBOARDTYPE_REPAIR,

	BILLBOARDTYPE_MAX,
}BillboardType;

class BillboardObject : public Object3D
{
private:
	BillboardType m_Type;
	bool m_DrawFlag;


public:
	BillboardObject() = delete;
	BillboardObject(BillboardType type, XMFLOAT3 pos, XMFLOAT3 size);
	virtual ~BillboardObject();

	virtual void Update() override;
	virtual void Draw() const override;

	void SetDrawFlag(bool flag);
	bool GetDrawFlag(void);
	BillboardType GetBillboardType(void);
};

#endif // OBJECT_BILLBOARD_H