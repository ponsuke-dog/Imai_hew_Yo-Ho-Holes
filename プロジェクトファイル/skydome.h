//********************************************************************************
//
//	ãÛÇÃçÏê¨ÉwÉbÉ_Å[[skydome.h] 
// 
//															Author : Riugo Honda
//															Date   :2025/02/21
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/21
//********************************************************************************
#pragma once

#ifndef _SKYDOME_H_
#define _SKYDOME_H_

class GameManager;

void SkydomeInitialize(GameManager*);
void SkydomeFinalize(void);
void SkydomeUpdate(void);
void SkydomeDraw(void);

#endif // !_SKYDOME_H_
