/*==============================================================================

   DirectX内部処理 [directx.h]
														 Author : Riugo Honda
														 Date   : 2024/01/27
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef DIRECTX_H
#define DIRECTX_H

#define NOMINMAX
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include "DirectXTex.h"
#include "configuration.h"

#define SAFE_RELEASE(o)      if (o){  (o)->Release(); o = NULL; }

// 頂点構造体
struct Vertex {
	XMFLOAT3 pos;	// 座標
	XMFLOAT4 color;	// 頂点カラー

	XMFLOAT2 texcoord; // テクスチャ座標(UV値とも言う)
	XMFLOAT3 normal;	//法線
};

class MultiSwapChains
{
public:
	MultiSwapChains(HRESULT hr, HWND hWnd, IDXGIFactory* dxgiFactory , int windownumber);
	~MultiSwapChains();

	void SetViewport();
	void SetMultiViewport(int n);

	void Clear();
	void Present();

	void SetRenderTarget();

	int GetWindowNumber();


private:
	// 各オブジェクトを配列で管理

	IDXGISwapChain* m_SwapChain = nullptr;
	ID3D11Texture2D* m_DepthStencilTexture = nullptr;
	ID3D11RenderTargetView* m_RenderTargetView = nullptr;
	ID3D11DepthStencilView* m_DepthStencilView = nullptr;

	int m_ChainNumber;

};

void DirectXInitialize(HWND hWnd[] , GAMEMODE gamemode = GAMEMODE_NONE);

void DirectXFinalize();

ID3D11Device* DirectXGetDevice();
ID3D11DeviceContext* DirectXGetDeviceContext();

std::vector<MultiSwapChains*> GetSwapChains();

void SetDepthEnable(bool Enable);

#endif // DIRECTX_H

