/*==============================================================================

   ƒŠƒUƒ‹ƒg‰æ–Ê [result.h]
														 Author : Souta Fukamizu
														 Date   : 2024/12/05
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef RESULT_H
#define RESULT_H

void ResultInitialize(void);
void ResultFinalize(void);
void ResultUpdate(void);
void ResultDraw(int);

void SetWhoWon(bool);
void SetScore(int score);

#endif // RESULT_H