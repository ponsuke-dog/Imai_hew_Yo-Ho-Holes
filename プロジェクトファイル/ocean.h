//********************************************************************************
//
//	äCÇÃçÏê¨ÉwÉbÉ_Å[[ocean.h] 
// 
//															Author : Riugo Honda
//															Date   :2025/02/21
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/21
//********************************************************************************
#pragma once

#ifndef _OCEAN_H_
#define _OCEAN_H_

class GameManager;

void OceanInitialize(GameManager*);
void OceanFinalize(void);
void OceanUpdate(void);
void OceanDraw(void);

#endif // !_OCEAN_H_

