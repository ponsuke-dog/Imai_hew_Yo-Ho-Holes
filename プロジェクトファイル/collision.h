#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "directx.h"

bool CheckBoxCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, XMFLOAT2 SizeA, XMFLOAT2 SizeB);
bool CheckBoxCollider(XMFLOAT3 PosA, XMFLOAT3 PosB, XMFLOAT3 SizeA, XMFLOAT3 SizeB);
bool CheckCircleCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, float rA, float rB);

#endif