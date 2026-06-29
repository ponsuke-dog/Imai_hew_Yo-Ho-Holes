/*==============================================================================

   ポリゴンの表示 [polygon.cpp]
														 Author : Youhei Sato
														 Date   : 2024/05/14
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include "DirectXTex.h"

#include "directx.h"
#include "configuration.h"
#include "shader.h"
#include "texture.h"

#include <math.h>
#include "sprite.h"


#define NUM_VERTEX 4 // 必要な頂点の数

static void SetVertex(void);
static void SetVertex(int x, int y, int width, int height, int texture_width, int texture_height);
static void SetReverseVertex(int x, int y, int width, int height, int texture_width, int texture_height);
static void SetMatrix(float x, float y, float width, float height, float angle);

static float g_r = 1.0f, g_g = 1.0f, g_b = 1.0f, g_a = 1.0f;

static ID3D11Buffer* g_VertexBuffer = NULL;

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_Device = NULL;
static ID3D11DeviceContext* g_DeviceContext = NULL;

/*
// 頂点構造体
struct Vertex {
	XMFLOAT3 pos;	// 座標
	XMFLOAT4 color;	// 頂点カラー

	XMFLOAT2 texcoord; // テクスチャ座標(UV値とも言う)
};
*/

void SpriteInitialize(ID3D11Device* device, ID3D11DeviceContext* device_context)
{
	// 取得するdeviceとdevice_contextのどちらかがNULLだったら正しく初期化できないので、エラー処理した方が良い
	if (!device || !device_context) {
		// MessageBoxなどでエラー処理すべし
	}

	// デバイスとデバイスコンテキストの保存
	g_Device = device;
	g_DeviceContext = device_context;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_Device->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	g_r = g_g = g_b = g_a = 1.0f;
}

void SpriteFinalize(void)
{
	SAFE_RELEASE(g_VertexBuffer);
}

void SpriteSetColor(float r, float g, float b, float a)
{
	g_r = r, g_g = g, g_b = b, g_a = a;
}

void SetVertex(void)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	g_DeviceContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* v = (Vertex*)msr.pData;

	v[0].pos.x = -0.5f;
	v[0].pos.y = -0.5f;
	v[0].pos.z = 0.0f;
	v[0].color = { g_r, g_g, g_b, g_a };
	v[0].texcoord = { 0.0f, 0.0f };

	v[1].pos.x = 0.5f;
	v[1].pos.y = -0.5f;
	v[1].pos.z = 0.0f;
	v[1].color = { g_r, g_g, g_b, g_a };
	v[1].texcoord = { 1.0f, 0.0f };

	v[2].pos.x = -0.5f;
	v[2].pos.y = 0.5f;
	v[2].pos.z = 0.0f;
	v[2].color = { g_r, g_g, g_b, g_a };
	v[2].texcoord = { 0.0f, 1.0f };

	v[3].pos.x = 0.5f;
	v[3].pos.y = 0.5f;
	v[3].pos.z = 0.0f;
	v[3].color = { g_r, g_g, g_b, g_a };
	v[3].texcoord = { 1.0f, 1.0f };

	g_DeviceContext->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_DeviceContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void SetVertex(int x, int y, int width, int height, int texture_width, int texture_height)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	g_DeviceContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* v = (Vertex*)msr.pData;

	float u0 = (float)x / texture_width;
	float v0 = (float)y / texture_height;
	float u1 = (float)(x + width) / texture_width;
	float v1 = (float)(y + height) / texture_height;

	v[0].pos.x = -0.5f;
	v[0].pos.y = -0.5f;
	v[0].pos.z = 0.0f;
	v[0].color = { g_r, g_g, g_b, g_a };
	v[0].texcoord = { u0, v0 };

	v[1].pos.x = 0.5f;
	v[1].pos.y = -0.5f;
	v[1].pos.z = 0.0f;
	v[1].color = { g_r, g_g, g_b, g_a };
	v[1].texcoord = { u1, v0 };

	v[2].pos.x = -0.5f;
	v[2].pos.y = 0.5f;
	v[2].pos.z = 0.0f;
	v[2].color = { g_r, g_g, g_b, g_a };
	v[2].texcoord = { u0, v1 };

	v[3].pos.x = 0.5f;
	v[3].pos.y = 0.5f;
	v[3].pos.z = 0.0f;
	v[3].color = { g_r, g_g, g_b, g_a };
	v[3].texcoord = { u1, v1 };

	g_DeviceContext->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_DeviceContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void SetReverseVertex(int x, int y, int width, int height, int texture_width, int texture_height)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	g_DeviceContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* v = (Vertex*)msr.pData;

	float u0 = (float)x / texture_width;
	float v0 = (float)y / texture_height;
	float u1 = (float)(x + width) / texture_width;
	float v1 = (float)(y + height) / texture_height;

	v[0].pos.x = -0.5f;
	v[0].pos.y = -0.5f;
	v[0].pos.z = 0.0f;
	v[0].color = { g_r, g_g, g_b, g_a };
	v[0].texcoord = { u1, v0 };

	v[1].pos.x = 0.5f;
	v[1].pos.y = -0.5f;
	v[1].pos.z = 0.0f;
	v[1].color = { g_r, g_g, g_b, g_a };
	v[1].texcoord = { u0, v0 };

	v[2].pos.x = -0.5f;
	v[2].pos.y = 0.5f;
	v[2].pos.z = 0.0f;
	v[2].color = { g_r, g_g, g_b, g_a };
	v[2].texcoord = { u1, v1 };

	v[3].pos.x = 0.5f;
	v[3].pos.y = 0.5f;
	v[3].pos.z = 0.0f;
	v[3].color = { g_r, g_g, g_b, g_a };
	v[3].texcoord = { u0, v1 };

	g_DeviceContext->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_DeviceContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void SetMatrix(float x, float y, float width, float height, float angle)
{
	// マトリクス設定
	XMMATRIX projection;

	XMMATRIX scale = XMMatrixScaling(width, height, 1.0f);
	XMMATRIX rotation = XMMatrixRotationZ(XMConvertToRadians(angle));
	XMMATRIX trancelation = XMMatrixTranslation(x, y, 0.0f);

	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	projection = XMMatrixTranspose(scale * rotation * trancelation * projection);

	XMMATRIX world = XMMatrixIdentity();

	CONSTANT constant;
	XMStoreFloat4x4(&constant.wvp, projection);
	XMStoreFloat4x4(&constant.world, world);
	constant.LightEnable = false;

	// 頂点シェーダの定数バッファに行列を送信
	ShaderVertexSetMatrix(&constant);
}

void SpriteDraw(int texture_id, float x, float y, float width, float height, float angle)
{
	// 頂点バッファへ基本の頂点座標を書き込み
	SetVertex();

	SetMatrix(x, y, width, height, angle);

	// テクスチャ設定
	ID3D11ShaderResourceView* srv = GetTexture(texture_id);
	g_DeviceContext->PSSetShaderResources(0, 1, &srv);

	// ポリゴン描画
	g_DeviceContext->Draw(NUM_VERTEX, 0);
}

void SpriteDraw(int texture_id, float x, float y, float width, float height, int tx, int ty, int tw, int th, float angle)
{
	// 頂点バッファへ基本の頂点座標を書き込み
	SetVertex(tx, ty, tw, th, TextureGetWidth(texture_id), TextureGetHeight(texture_id));

	SetMatrix(x, y, width, height, angle);

	// テクスチャ設定
	ID3D11ShaderResourceView* srv = GetTexture(texture_id);
	g_DeviceContext->PSSetShaderResources(0, 1, &srv);

	// ポリゴン描画
	g_DeviceContext->Draw(NUM_VERTEX, 0);
}
void SpriteDraw(int texture_id, float x, float y, float width, float height, int tx, int ty, int tw, int th, float angle, bool reverse)
{
	// 頂点バッファへ基本の頂点座標を書き込み
	if (reverse)
	{
		SetReverseVertex(tx, ty, tw, th, TextureGetWidth(texture_id), TextureGetHeight(texture_id));
	}
	else
	{
	SetVertex(tx, ty, tw, th, TextureGetWidth(texture_id), TextureGetHeight(texture_id));
	}

	SetMatrix(x, y, width, height, angle);

	// テクスチャ設定
	ID3D11ShaderResourceView* srv = GetTexture(texture_id);
	g_DeviceContext->PSSetShaderResources(0, 1, &srv);

	// ポリゴン描画
	g_DeviceContext->Draw(NUM_VERTEX, 0);
}