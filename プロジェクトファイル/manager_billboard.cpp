//********************************************************************************
//
// オブジェクトのビルボードUIマネージャー[manager_billboard.cpp] 
// 
//															Author :Sota Fukamizu
//															Date   :2025/01/18
// -------------------------------------------------------------------------------
//															Last Edited:2025/02/04
//********************************************************************************

#include "directx.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include <list>
#include "object_billboard.h"
#include "manager_billboard.h"

//グローバル関数
std::list<BillboardObject*> g_BillboradObjects;

static ID3D11Buffer* g_VertexBuffer = NULL;

static XMMATRIX g_Projection;
static XMMATRIX g_View;

static int g_Texture;

void BillBoradManagerInitialize()
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

	g_Texture = TextureLoad(L"asset\\texture\\UI_B.png");

}

void BillBoradManagerFinalize()
{
	SAFE_RELEASE(g_VertexBuffer);

	for (auto itr = g_BillboradObjects.begin(); itr != g_BillboradObjects.end();) {
		if (*itr) {
			delete* itr;
			itr = g_BillboradObjects.erase(itr);
		}
		else {
			++itr;
		}
	}

}

void BillBoradManagerUpdate()
{
	for (auto object : g_BillboradObjects)
	{
		object->Update();
	}

	g_BillboradObjects.remove_if(
		[](const BillboardObject* pObject) {
			if (pObject->GetDestroy()) {
				delete pObject;
				return true;
			}
			return false;
		}
	);
}

void BillBoradManagerDraw()
{
	//マトリクス設定
	XMMATRIX world, wvp;


	//ビューの逆行列
	XMMATRIX inView;
	inView = XMMatrixInverse(nullptr, g_View); //逆行列
	inView.r[3].m128_f32[0] = 0.0f;
	inView.r[3].m128_f32[1] = 0.0f;
	inView.r[3].m128_f32[2] = 0.0f;

	//頂点バッファ設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	DirectXGetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//テクスチャ設定
	ID3D11ShaderResourceView* texture = GetTexture(g_Texture);
	DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	//プリミティブトポロジ設定
	DirectXGetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	for(auto object : g_BillboradObjects)
	{
		if(object->GetDrawFlag())
		{
			//ワールドマトリクス
			XMMATRIX scale = XMMatrixScaling(object->GetSize().x, object->GetSize().y, object->GetSize().z); //拡大縮小
			//XMMATRIX rotation = XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z); //回転
			XMMATRIX translation = XMMatrixTranslation(object->GetPosition().x, object->GetPosition().y, object->GetPosition().z); //移動
			world = scale * inView * translation;//ビルボード処理

			wvp = world * g_View * g_Projection;
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
		}

	}

}

void BillBoradManagerRegister(BillboardObject* object)
{
	g_BillboradObjects.push_back(object);
}

void SetBillboardProjection(XMMATRIX projection)
{
	g_Projection = projection;
}

void SetBillboardView(XMMATRIX view)
{
	g_View = view;
}