/*==============================================================================

   オブジェクト用のビルボードUI　[object_billboard.cpp]
														 Author : Souta Fukamizu
														 Date   : 2025/02/04
--------------------------------------------------------------------------------

==============================================================================*/

#include "directx.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "object_3d.h"
#include "object_billboard.h"

BillboardObject::BillboardObject(BillboardType type, XMFLOAT3 pos, XMFLOAT3 size) : Object3D(pos, size, OBJECTTYPE_EFFECT)
{
	m_Type = type;
	m_DrawFlag = false;

}

BillboardObject::~BillboardObject()
{

}

void BillboardObject::Update()
{

}

void BillboardObject::Draw() const
{
}

void BillboardObject::SetDrawFlag(bool flag)
{
	m_DrawFlag = flag;
}

bool BillboardObject::GetDrawFlag(void)
{
	return m_DrawFlag;
}

BillboardType BillboardObject::GetBillboardType()
{
	return m_Type;
}
