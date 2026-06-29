//	collision.h
//	
//
//	2025.1.19			大山凌太朗

#include "collision.h"

bool CheckBoxCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, XMFLOAT2 SizeA, XMFLOAT2 SizeB)
{
    // PosAとPosBはそれぞれの矩形の中心座標
    if ((PosA.x + SizeA.x / 2 > PosB.x - SizeB.x / 2) &&
        (PosA.x - SizeA.x / 2 < PosB.x + SizeB.x / 2)) {
        if ((PosA.y + SizeA.y / 2 > PosB.y - SizeB.y / 2) &&
            (PosA.y - SizeA.y / 2 < PosB.y + SizeB.y / 2)) {
            return true;
        }
    }

    // 当たっていない
    return false;
}

bool CheckBoxCollider(XMFLOAT3 PosA, XMFLOAT3 PosB, XMFLOAT3 SizeA, XMFLOAT3 SizeB)
{
    // PosAとPosBはそれぞれの矩形の中心座標
    if ((PosA.x + SizeA.x / 2 > PosB.x - SizeB.x / 2) &&
        (PosA.x - SizeA.x / 2 < PosB.x + SizeB.x / 2) &&
        (PosA.y + SizeA.y / 2 > PosB.y - SizeB.y / 2) &&
        (PosA.y - SizeA.y / 2 < PosB.y + SizeB.y / 2) &&
        (PosA.z + SizeA.z / 2 > PosB.z - SizeB.z / 2) &&
        (PosA.z - SizeA.z / 2 < PosB.z + SizeB.z / 2)) {
        return true;
    }

    // 当たっていない
    return false;
}

bool CheckCircleCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, float rA, float rB)
{
    // PosAとPosBはそれぞれの円の中心座標
    float dx = PosA.x - PosB.x;
    float dy = PosA.y - PosB.y;
    float dr = dx * dx + dy * dy;

    float ar = rA + rB;
    float dl = ar * ar;
    if (dr < dl)
    {
        return true;
    }

    // 当たっていない
    return false;
}