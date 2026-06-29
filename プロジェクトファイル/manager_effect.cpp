//********************************************************************************
//
// エフェクトマネージャー[manager_effect.h] 
// 
//															Author :Riugo Honda
//															Date   :2025/01/18
// -------------------------------------------------------------------------------
//															Last Edited:2025/01/18
//********************************************************************************
#include "directx.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "object_3d.h"
#include <list>
#include "effect_3d.h"
#include "manager_effect.h"


//********************************************************************************
// グローバル変数
//********************************************************************************
std::list<Effect3D*> g_Effect3D;

static ID3D11Buffer* g_VertexBuffer = NULL;

static XMMATRIX g_Projection;
static XMMATRIX g_View;

//static int g_Texture[EFFECTTYPE_MAX];
static Effect_Info g_Effect_Info[EFFECTTYPE_MAX];
//********************************************************************************
// 関数
//********************************************************************************
void EffectManagerInitialize()
{
	Vertex vertex[4];

	vertex[0].pos = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertex[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].texcoord = XMFLOAT2(0.0f, 0.0f);
	vertex[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertex[1].pos = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texcoord = XMFLOAT2(1.0f, 0.0f);
	vertex[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertex[2].pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertex[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texcoord = XMFLOAT2(0.0f, 1.0f);
	vertex[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertex[3].pos = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertex[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texcoord = XMFLOAT2(1.0f, 1.0f);
	vertex[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(Vertex) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertex;

		DirectXGetDevice()->CreateBuffer(&bd, &sd, &g_VertexBuffer);
	}

	g_Effect_Info[EFFECTTYPE_EXPLOSION].Texture = TextureLoad(L"asset\\texture\\explosion_test_100.png");
	g_Effect_Info[EFFECTTYPE_EXPLOSION].Tx = 10;
	g_Effect_Info[EFFECTTYPE_EXPLOSION].Ty = 11;
	g_Effect_Info[EFFECTTYPE_EXPLOSION].FrameMax = g_Effect_Info[EFFECTTYPE_EXPLOSION].Tx * g_Effect_Info[EFFECTTYPE_EXPLOSION].Ty;
	
	g_Effect_Info[EFFECTTYPE_RUN].Texture = TextureLoad(L"asset\\texture\\Tsutikemuri.png");
	g_Effect_Info[EFFECTTYPE_RUN].Tx = 6;
	g_Effect_Info[EFFECTTYPE_RUN].Ty = 6;
	g_Effect_Info[EFFECTTYPE_RUN].FrameMax = g_Effect_Info[EFFECTTYPE_RUN].Tx * g_Effect_Info[EFFECTTYPE_RUN].Ty;

	g_Effect_Info[EFFECTTYPE_SHOTSMOKE].Texture = TextureLoad(L"asset\\texture\\kemuri_tate_1.png");
	g_Effect_Info[EFFECTTYPE_SHOTSMOKE].Tx = 6;
	g_Effect_Info[EFFECTTYPE_SHOTSMOKE].Ty = 6;
	g_Effect_Info[EFFECTTYPE_SHOTSMOKE].FrameMax = g_Effect_Info[EFFECTTYPE_SHOTSMOKE].Tx * g_Effect_Info[EFFECTTYPE_SHOTSMOKE].Ty;

	g_Effect_Info[EFFECTTYPE_SHOCKWAVE].Texture = TextureLoad(L"asset\\texture\\syogeki.png");
	g_Effect_Info[EFFECTTYPE_SHOCKWAVE].Tx = 4;
	g_Effect_Info[EFFECTTYPE_SHOCKWAVE].Ty = 4;
	g_Effect_Info[EFFECTTYPE_SHOCKWAVE].FrameMax = g_Effect_Info[EFFECTTYPE_SHOCKWAVE].Tx * g_Effect_Info[EFFECTTYPE_SHOCKWAVE].Ty;
	
	g_Effect_Info[EFFECTTYPE_SPLASH].Texture = TextureLoad(L"asset\\texture\\mizusibuki_type_b.png");
	g_Effect_Info[EFFECTTYPE_SPLASH].Tx = 4;
	g_Effect_Info[EFFECTTYPE_SPLASH].Ty = 4;
	g_Effect_Info[EFFECTTYPE_SPLASH].FrameMax = g_Effect_Info[EFFECTTYPE_SPLASH].Tx * g_Effect_Info[EFFECTTYPE_SPLASH].Ty;

	g_Effect_Info[EFFECTTYPE_FIRE].Texture = TextureLoad(L"asset\\texture\\Fire_1.png");
	g_Effect_Info[EFFECTTYPE_FIRE].Tx = 6;
	g_Effect_Info[EFFECTTYPE_FIRE].Ty = 5;
	g_Effect_Info[EFFECTTYPE_FIRE].FrameMax = g_Effect_Info[EFFECTTYPE_FIRE].Tx * g_Effect_Info[EFFECTTYPE_FIRE].Ty;
	//g_Texture[EFFECTTYPE_EXPLOSION] = TextureLoad(L"asset\\texture\\Explosion.png");

	g_Effect_Info[EFFECTTYPE_REPAIR].Texture = TextureLoad(L"asset\\texture\\syuhuku_kemuri_ru-pu.png");
	g_Effect_Info[EFFECTTYPE_REPAIR].Tx = 6;
	g_Effect_Info[EFFECTTYPE_REPAIR].Ty = 11;
	g_Effect_Info[EFFECTTYPE_REPAIR].FrameMax = g_Effect_Info[EFFECTTYPE_REPAIR].Tx * g_Effect_Info[EFFECTTYPE_REPAIR].Ty;
	
	g_Effect_Info[EFFECTTYPE_EXPLOSION_MOJI].Texture = TextureLoad(L"asset\\texture\\explosion_moji.png");
	g_Effect_Info[EFFECTTYPE_EXPLOSION_MOJI].Tx = 7;
	g_Effect_Info[EFFECTTYPE_EXPLOSION_MOJI].Ty = 7;
	g_Effect_Info[EFFECTTYPE_EXPLOSION_MOJI].FrameMax = g_Effect_Info[EFFECTTYPE_EXPLOSION_MOJI].Tx * g_Effect_Info[EFFECTTYPE_EXPLOSION_MOJI].Ty;

	g_Effect_Info[EFFECTTYPE_SPLASH_MOJI].Texture = TextureLoad(L"asset\\texture\\splash_moji.png");
	g_Effect_Info[EFFECTTYPE_SPLASH_MOJI].Tx = 6;
	g_Effect_Info[EFFECTTYPE_SPLASH_MOJI].Ty = 6;
	g_Effect_Info[EFFECTTYPE_SPLASH_MOJI].FrameMax = g_Effect_Info[EFFECTTYPE_SPLASH_MOJI].Tx * g_Effect_Info[EFFECTTYPE_SPLASH_MOJI].Ty;

	g_Effect_Info[EFFECTTYPE_BANG_MOJI].Texture = TextureLoad(L"asset\\texture\\bang_moji.png");
	g_Effect_Info[EFFECTTYPE_BANG_MOJI].Tx = 7;
	g_Effect_Info[EFFECTTYPE_BANG_MOJI].Ty = 7;
	g_Effect_Info[EFFECTTYPE_BANG_MOJI].FrameMax = g_Effect_Info[EFFECTTYPE_BANG_MOJI].Tx * g_Effect_Info[EFFECTTYPE_BANG_MOJI].Ty;

	g_Effect_Info[EFFECTTYPE_CLUNK_MOJI].Texture = TextureLoad(L"asset\\texture\\clunk_moji.png");
	g_Effect_Info[EFFECTTYPE_CLUNK_MOJI].Tx = 7;
	g_Effect_Info[EFFECTTYPE_CLUNK_MOJI].Ty = 7;
	g_Effect_Info[EFFECTTYPE_CLUNK_MOJI].FrameMax = g_Effect_Info[EFFECTTYPE_CLUNK_MOJI].Tx * g_Effect_Info[EFFECTTYPE_CLUNK_MOJI].Ty;

}

void EffectManagerFinalize()
{
	SAFE_RELEASE(g_VertexBuffer);

	for (auto itr = g_Effect3D.begin(); itr != g_Effect3D.end();) {
		if (*itr) {
			delete* itr;
			itr = g_Effect3D.erase(itr);
		}
		else {
			++itr;
		}
	}

}

void EffectManagerUpdate()
{
	for (auto itr : g_Effect3D)
	{
		itr->Update();
	}

	//ラムダ式消去法
	/*g_Effect3D.remove_if(
		[](const Effect3D* pObject) {
			if (pObject->GetDestroy()) {
				delete pObject;
				return true;
			}
			return false;
		}
	);*/

	for (auto itr = g_Effect3D.begin(); itr != g_Effect3D.end();) {
		if ((*itr)->GetDestroy()) {
			delete* itr;
			itr = g_Effect3D.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void EffectManagerDraw()
{
	//マトリクス設定
	XMMATRIX projection, view, world, wvp;

	projection = g_Projection;
	view = g_View;

	for (auto itr : g_Effect3D)
	{
		if (itr->GetEffectType() == EFFECTTYPE_EXPLOSION || itr->GetEffectType() == EFFECTTYPE_EXPLOSION_MOJI)
		{
			SetDepthEnable(false);
		}
		


		//テクスチャ座標算出
		float w = (1.0f / g_Effect_Info[itr->GetEffectType()].Tx);// m_Tx
		float h = (1.0f / g_Effect_Info[itr->GetEffectType()].Ty);//m_Ty
		float x = itr->GetFrame() % g_Effect_Info[itr->GetEffectType()].Tx * w;// m_Frame % m_Tx
		float y = itr->GetFrame() / g_Effect_Info[itr->GetEffectType()].Ty * h;//m_Frame / m_Tx 

		//頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		DirectXGetDeviceContext()->Map(g_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		Vertex* vertex = (Vertex*)msr.pData;
		{
			vertex[0].pos = XMFLOAT3(-1.0f, 1.0f, 0.0f);
			vertex[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[0].texcoord = XMFLOAT2(x, y);
			vertex[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			vertex[1].pos = XMFLOAT3(1.0f, 1.0f, 0.0f);
			vertex[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[1].texcoord = XMFLOAT2(x + w, y);
			vertex[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			vertex[2].pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);
			vertex[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[2].texcoord = XMFLOAT2(x, y + h);
			vertex[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			vertex[3].pos = XMFLOAT3(1.0f, -1.0f, 0.0f);
			vertex[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[3].texcoord = XMFLOAT2(x + w, y + h);
			vertex[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		}

		DirectXGetDeviceContext()->Unmap(g_VertexBuffer, 0);

		

		//ビューの逆行列
		XMMATRIX inView;
		inView = XMMatrixInverse(nullptr, view); //逆行列
		inView.r[3].m128_f32[0] = 0.0f;
		inView.r[3].m128_f32[1] = 0.0f;
		inView.r[3].m128_f32[2] = 0.0f;


		//頂点バッファ設定
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		DirectXGetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

		//テクスチャ設定
		ID3D11ShaderResourceView* texture = GetTexture(g_Effect_Info[itr->GetEffectType()].Texture);
		DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &texture);

		//プリミティブトポロジ設定
		DirectXGetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//ワールドマトリクス
		XMMATRIX scale = XMMatrixScaling(itr->GetSize().x, itr->GetSize().y, itr->GetSize().z); //拡大縮小
		//XMMATRIX rotation = XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z); //回転
		XMMATRIX translation = XMMatrixTranslation(itr->GetPosition().x, itr->GetPosition().y, itr->GetPosition().z); //移動
		world = scale * inView * translation;//ビルボード処理

		wvp = world * view * projection;
		wvp = XMMatrixTranspose(wvp);
		world = XMMatrixTranspose(world);

		CONSTANT matrix;
		XMStoreFloat4x4(&matrix.wvp, wvp);
		XMStoreFloat4x4(&matrix.world, world);
		matrix.LightEnable = false;


		//頂点シェーダの定数バッファに行列を送信
		ShaderVertexSetMatrix(&matrix);

		//ポリゴン描画
		DirectXGetDeviceContext()->Draw(4, 0);

		if (itr->GetEffectType() == EFFECTTYPE_EXPLOSION || itr->GetEffectType() == EFFECTTYPE_EXPLOSION_MOJI)
		{
			SetDepthEnable(true);
		}
	}
}

void EffectManagerRegister(EffectType effecttype, XMFLOAT3 pos, XMFLOAT3 size)
{
	g_Effect3D.push_back(new Effect3D(effecttype,  pos, size, g_Effect_Info[effecttype].FrameMax, EFFECT3DTYPE_NORMAL));
}

void EffectManagerRegister(Effect3D* object)
{
	g_Effect3D.push_back(object);
}

void SetEffectProjection(XMMATRIX projection)
{
	g_Projection = projection;
}

void SetEffectView(XMMATRIX view)
{
	g_View = view;
}

int GetEffectFrameMaxNum(EffectType effecttype)
{
	return g_Effect_Info[effecttype].FrameMax;
}
