/*==============================================================================

   ƒ|ƒŠƒSƒ“‚Ì•\Ž¦ [sprite.h]
														 Author : Youhei Sato
														 Date   : 2024/05/14
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef SPRITE_H
#define SPRITE_H

#include <d3d11.h>

void SpriteInitialize(ID3D11Device* device, ID3D11DeviceContext* device_context);
void SpriteFinalize(void);

void SpriteSetColor(float r, float g, float b, float a);

void SpriteDraw(int texture_id, float x, float y, float w, float h, float angle = 0.0f);
void SpriteDraw(int texture_id, float x, float y, float w, float h, int tx, int ty, int tw, int th, float angle = 0.0f);
void SpriteDraw(int texture_id, float x, float y, float w, float h, int tx, int ty, int tw, int th, float angle,bool reverse);

#endif // SPRITE_H

