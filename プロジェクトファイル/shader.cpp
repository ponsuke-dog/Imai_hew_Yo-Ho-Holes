/*==============================================================================

   シェーダーの利用 [shader.cpp]
														 Author : Youhei Sato
														 Date   : 2024/05/14
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
using namespace DirectX;
#include "directx.h"
#include "shader.h"

#pragma comment(lib, "d3dcompiler.lib")

// 頂点構造体
struct Vertex2D
{
	XMFLOAT3 Position;
	XMFLOAT4 Diffuse;

	XMFLOAT2 TexCoord; // texture coordinate
};

static ID3D11SamplerState* g_SamplerState = NULL;
static ID3D11VertexShader* g_VertexShader = NULL;
static ID3D11InputLayout* g_VertexLayout = NULL;
static ID3D11PixelShader* g_PixelShader = NULL;
static ID3D11Buffer* g_ConstantBuffer = NULL;

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_Device = NULL;
static ID3D11DeviceContext* g_DeviceContext = NULL;

void ShaderInitialize(ID3D11Device* device, ID3D11DeviceContext* device_context)
{
	HRESULT hr;

	// 取得するdeviceとdevice_contextのどちらかがNULLだったら正しく初期化できないので、エラー処理した方が良い
	if (!device || !device_context) {
		// MessageBoxなどでエラー処理すべし
	}

	// デバイスとデバイスコンテキストの保存
	g_Device = device;
	g_DeviceContext = device_context;

	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc = {};
	//samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;	//extent by pixel 
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//ID3D11SamplerState* samplerState = NULL;
	g_Device->CreateSamplerState(&samplerDesc, &g_SamplerState);

	g_DeviceContext->PSSetSamplers(0, 1, &g_SamplerState);

	// 頂点シェーダコンパイル・生成
	ID3DBlob* pErrorBlob;
	ID3DBlob* pVSBlob = NULL;
	hr = D3DCompileFromFile(L"shader.hlsl", NULL, NULL, "VS", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVSBlob, &pErrorBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
	}

	g_Device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_VertexShader);

	// 入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,          0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	g_Device->CreateInputLayout(layout,
		numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&g_VertexLayout);

	pVSBlob->Release();

	// 入力レイアウト設定
	g_DeviceContext->IASetInputLayout(g_VertexLayout);

	// ピクセルシェーダコンパイル・生成
	ID3DBlob* pPSBlob = NULL;
	hr = D3DCompileFromFile(L"shader.hlsl", NULL, NULL, "PS", "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pPSBlob, &pErrorBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	}

	g_Device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_PixelShader);

	pPSBlob->Release();


	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(CONSTANT);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	g_Device->CreateBuffer(&hBufferDesc, NULL, &g_ConstantBuffer);
	g_DeviceContext->VSSetConstantBuffers(0, 1, &g_ConstantBuffer);
	g_DeviceContext->PSSetConstantBuffers(0, 1, &g_ConstantBuffer);

	// シェーダ設定
	g_DeviceContext->VSSetShader(g_VertexShader, NULL, 0);
	g_DeviceContext->PSSetShader(g_PixelShader, NULL, 0);
}

void ShaderFinalize()
{
	SAFE_RELEASE(g_ConstantBuffer);
	SAFE_RELEASE(g_PixelShader);
	SAFE_RELEASE(g_VertexLayout);
	SAFE_RELEASE(g_VertexShader);
	SAFE_RELEASE(g_SamplerState);
}

void ShaderVertexSetMatrix(const CONSTANT* matrix)
{
	g_DeviceContext->UpdateSubresource(g_ConstantBuffer, 0, NULL, matrix, 0, 0);
}
