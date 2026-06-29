/*==============================================================================

   シェーダーの利用 [shader.h]
														 Author : Youhei Sato
														 Date   : 2024/05/14
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

void ShaderInitialize(ID3D11Device* device, ID3D11DeviceContext* device_context);
void ShaderFinalize(void);

struct CONSTANT
{
	XMFLOAT4X4 wvp;
	XMFLOAT4X4 world;
	BOOL LightEnable;
	float Dummy[3];
};

void ShaderVertexSetMatrix(const CONSTANT* constant);

#endif // SHADER_H