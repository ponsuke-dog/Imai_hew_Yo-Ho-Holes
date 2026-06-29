//********************************************************************************
//
// オブジェクトのビルボードUIマネージャー[manager_billboard.h] 
// 
//															Author :Sota Fukamizu
//															Date   :2025/01/18
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/04
//********************************************************************************

#ifndef MANAGER_BILLBOARD_H
#define MANAGER_BILLBOARD_H

class BillboardObject;

void BillBoradManagerInitialize();
void BillBoradManagerFinalize();
void BillBoradManagerUpdate();
void BillBoradManagerDraw();

void BillBoradManagerRegister(BillboardObject* object);


void SetBillboardProjection(XMMATRIX projection);
void SetBillboardView(XMMATRIX view);


#endif // !MANAGER_BILLBOARD_H
