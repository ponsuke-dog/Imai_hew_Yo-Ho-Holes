//	flotsam.h
//	
//
//	2024.12.20			PON

#ifndef FLOTSAM_H
#define FLOTSAM_H

#include "object_3d.h"

typedef enum
{
	FLOTSAM_TYPE_NONE = -1,

	FLOTSAM_TYPE_WOOD,
	FLOTSAM_TYPE_CANNON,
	FLOTSAM_TYPE_ENHANCE,

	FLOTSAM_TYPE_MAX,
}FlotsamType;

class Flotsam : public Object3D
{
private:
	XMFLOAT3 m_Velocity;	// 加速度
	FlotsamType m_Type;
public:

	Flotsam() = delete;
	Flotsam(Ship* pship ,XMFLOAT3 pos,FlotsamType type);
	~Flotsam();

	void Update()override;
	void Draw()const override;

	void SetVel(XMFLOAT3 vel);		// 加速度のセッター
	XMFLOAT3 GetVel();			// 加速度のゲッター

	FlotsamType GetFlotsamType();

};

#endif // !FLOTSAM_H