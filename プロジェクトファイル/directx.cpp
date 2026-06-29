/*==============================================================================

   DirectX内部処理 [directx.h]
														 Author : Riugo Honda
														 Date   : 2024/01/27
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3d11.h>
#include "configuration.h"
#include "directx.h"
#include <vector>

#pragma comment (lib, "d3d11.lib")	

#if _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif

static std::vector<MultiSwapChains*> g_SwapChains;

static ID3D11Device* g_Device = nullptr;
static ID3D11DeviceContext* g_DeviceContext = nullptr;

static ID3D11RasterizerState* g_RasterizerState = NULL;
static ID3D11BlendState* g_BlendState = NULL;
static ID3D11DepthStencilState* g_DepthStencilStateDepthEnable = NULL;
static ID3D11DepthStencilState* g_DepthStencilStateDepthDisable = NULL;


MultiSwapChains::MultiSwapChains(HRESULT hr, HWND hWnd, IDXGIFactory* dxgiFactory ,int windownumber):m_ChainNumber(windownumber)
{

	DXGI_SWAP_CHAIN_DESC sd = {};

	// スワップチェーンの設定
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = dxgiFactory->CreateSwapChain(g_Device, &sd, &m_SwapChain);
	if (FAILED(hr))
	{
		dxgiFactory->Release();
		return;
	}

	//レンダーターゲットビューの作成
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		dxgiFactory->Release();
		return;
	}
	hr = g_Device->CreateRenderTargetView(pBackBuffer, nullptr, &m_RenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		dxgiFactory->Release();
		return;
	}


	// デプスステンシルテクスチャ、ビューの作成
	D3D11_TEXTURE2D_DESC td = {};
	td.Width = sd.BufferDesc.Width;
	td.Height = sd.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc = sd.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	hr = g_Device->CreateTexture2D(&td, nullptr, &m_DepthStencilTexture);
	if (FAILED(hr))
	{
		dxgiFactory->Release();
		return;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	hr = g_Device->CreateDepthStencilView(m_DepthStencilTexture, &dsvd, &m_DepthStencilView);
	if (FAILED(hr))
	{
		dxgiFactory->Release();
		return;
	}

}

MultiSwapChains::~MultiSwapChains()
{

	SAFE_RELEASE(m_DepthStencilView);
	SAFE_RELEASE(m_DepthStencilTexture);
	SAFE_RELEASE(m_RenderTargetView);
	SAFE_RELEASE(m_SwapChain);



}


void MultiSwapChains::SetViewport()
{
	// ビューポート設定
	D3D11_VIEWPORT vp;

	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;




	g_DeviceContext->RSSetViewports(1, &vp);
}

void MultiSwapChains::SetMultiViewport(int n)
{
	// ビューポート設定
	D3D11_VIEWPORT vp[2];

	vp[0].Width = (FLOAT)SCREEN_WIDTH * 0.5f;
	vp[0].Height = (FLOAT)SCREEN_HEIGHT;
	vp[0].MinDepth = 0.0f;
	vp[0].MaxDepth = 1.0f;
	vp[0].TopLeftX = 0;
	vp[0].TopLeftY = 0.0f;//SCREEN_HEIGHT 0.1f;

	vp[1].Width = (FLOAT)SCREEN_WIDTH * 0.5f;
	vp[1].Height = (FLOAT)SCREEN_HEIGHT;
	vp[1].MinDepth = 0.0f;
	vp[1].MaxDepth = 1.0f;
	vp[1].TopLeftX = SCREEN_WIDTH * 0.5f;
	vp[1].TopLeftY = 0.0f;//SCREEN_HEIGHT * 0.1f;

	g_DeviceContext->RSSetViewports(1, &vp[n]);
}

//=============================================================================
// バックバッファクリア
//=============================================================================
void MultiSwapChains::Clear()
{
	// バックバッファクリア
	float clear_color[4] = { 0.4f, 0.4f, 0.8f, 1.0f };
	g_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clear_color);
	g_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


//=============================================================================
// プレゼント
//=============================================================================
void MultiSwapChains::Present()
{
	// バックバッファとフロントバッファの交換
	m_SwapChain->Present(0, 0);
}

void MultiSwapChains::SetRenderTarget()
{
	g_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

}

int MultiSwapChains::GetWindowNumber()
{
	return m_ChainNumber;
}

ID3D11Device* DirectXGetDevice(void)
{
	return g_Device;
}

ID3D11DeviceContext* DirectXGetDeviceContext(void)
{
	return g_DeviceContext;
}

std::vector<MultiSwapChains*> GetSwapChains()
{
	return g_SwapChains;
}

void DirectXInitialize(HWND hWnd[] , GAMEMODE gamemode)
{
	switch (gamemode)
	{
	case GAMEMODE_NONE:
		{
			HRESULT hr;
			DXGI_SWAP_CHAIN_DESC sd[ONE_WINDOW] = {};

			// デバイスの作成 (一度だけ)
			D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
			hr = D3D11CreateDevice(
				nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
				&feature_level, 1, D3D11_SDK_VERSION, &g_Device, nullptr, &g_DeviceContext
			);

			if (FAILED(hr)) {
				return;
			}

			// IDXGIFactoryの取得
			IDXGIDevice* dxgiDevice = nullptr;
			hr = g_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
			if (FAILED(hr)) return;

			IDXGIAdapter* dxgiAdapter = nullptr;
			hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
			if (FAILED(hr)) return;

			IDXGIFactory* dxgiFactory = nullptr;
			hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
			if (FAILED(hr)) return;

			dxgiDevice->Release();
			dxgiAdapter->Release();

			for (int i = 0; i < ONE_WINDOW; i++)
			{
				g_SwapChains.push_back(new MultiSwapChains(hr, hWnd[i], dxgiFactory, i));
			}

			dxgiFactory->Release();

			// ビューポート設定
			D3D11_VIEWPORT vp;
			vp.Width = (FLOAT)SCREEN_WIDTH;
			vp.Height = (FLOAT)SCREEN_HEIGHT;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			g_DeviceContext->RSSetViewports(1, &vp);

			// ラスタライザステートの作成
			D3D11_RASTERIZER_DESC rd = {};
			rd.FillMode = D3D11_FILL_SOLID;
			rd.CullMode = D3D11_CULL_BACK;
			//rd.CullMode = D3D11_CULL_NONE;
			rd.DepthClipEnable = TRUE;
			rd.MultisampleEnable = FALSE;
			g_Device->CreateRasterizerState(&rd, &g_RasterizerState);

			// デバイスコンテキストにラスタライザーステートを設定
			g_DeviceContext->RSSetState(g_RasterizerState);

			// ブレンドステート設定
			D3D11_BLEND_DESC bd = {};
			bd.AlphaToCoverageEnable = FALSE;
			bd.IndependentBlendEnable = FALSE;
			bd.RenderTarget[0].BlendEnable = TRUE;

			bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

			bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

			bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			g_Device->CreateBlendState(&bd, &g_BlendState);
			g_DeviceContext->OMSetBlendState(g_BlendState, blend_factor, 0xffffffff);

			// 深度ステンシルステート設定
			D3D11_DEPTH_STENCIL_DESC dsd = {};


			// 深度有効ステート
			dsd.DepthEnable = TRUE;
			dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsd.DepthFunc = D3D11_COMPARISON_LESS;
			dsd.StencilEnable = FALSE;

			g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthEnable);

			// 深度無効ステート
			dsd.DepthEnable = FALSE;
			dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

			g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthDisable);

			// とりあえず深度有効にしておく
			g_DeviceContext->OMSetDepthStencilState(g_DepthStencilStateDepthEnable, NULL);
		}
	break;
	case GAMEMODE_SOLO:
		{
			HRESULT hr;
			DXGI_SWAP_CHAIN_DESC sd[ONE_WINDOW] = {};

			// デバイスの作成 (一度だけ)
			D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
			hr = D3D11CreateDevice(
				nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
				&feature_level, 1, D3D11_SDK_VERSION, &g_Device, nullptr, &g_DeviceContext
			);

			if (FAILED(hr)) {
				return;
			}

			// IDXGIFactoryの取得
			IDXGIDevice* dxgiDevice = nullptr;
			hr = g_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
			if (FAILED(hr)) return;

			IDXGIAdapter* dxgiAdapter = nullptr;
			hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
			if (FAILED(hr)) return;

			IDXGIFactory* dxgiFactory = nullptr;
			hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
			if (FAILED(hr)) return;

			dxgiDevice->Release();
			dxgiAdapter->Release();

			for (int i = 0; i < ONE_WINDOW; i++)
			{
				g_SwapChains.push_back(new MultiSwapChains(hr, hWnd[0], dxgiFactory, i));
			}

			dxgiFactory->Release();

			// ビューポート設定
			D3D11_VIEWPORT vp;
			vp.Width = (FLOAT)SCREEN_WIDTH;
			vp.Height = (FLOAT)SCREEN_HEIGHT;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			g_DeviceContext->RSSetViewports(1, &vp);

			// ラスタライザステートの作成
			D3D11_RASTERIZER_DESC rd = {};
			rd.FillMode = D3D11_FILL_SOLID;
			rd.CullMode = D3D11_CULL_BACK;
			//rd.CullMode = D3D11_CULL_NONE;
			rd.DepthClipEnable = TRUE;
			rd.MultisampleEnable = FALSE;
			g_Device->CreateRasterizerState(&rd, &g_RasterizerState);

			// デバイスコンテキストにラスタライザーステートを設定
			g_DeviceContext->RSSetState(g_RasterizerState);

			// ブレンドステート設定
			D3D11_BLEND_DESC bd = {};
			bd.AlphaToCoverageEnable = FALSE;
			bd.IndependentBlendEnable = FALSE;
			bd.RenderTarget[0].BlendEnable = TRUE;

			bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

			bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

			bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			g_Device->CreateBlendState(&bd, &g_BlendState);
			g_DeviceContext->OMSetBlendState(g_BlendState, blend_factor, 0xffffffff);

			// 深度ステンシルステート設定
			D3D11_DEPTH_STENCIL_DESC dsd = {};


			// 深度有効ステート
			dsd.DepthEnable = TRUE;
			dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsd.DepthFunc = D3D11_COMPARISON_LESS;
			dsd.StencilEnable = FALSE;

			g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthEnable);

			// 深度無効ステート
			dsd.DepthEnable = FALSE;
			dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

			g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthDisable);

			// とりあえず深度有効にしておく
			g_DeviceContext->OMSetDepthStencilState(g_DepthStencilStateDepthEnable, NULL);
		}
	break;
	case GAMEMODE_LOCAL_SPLITSCREEN:
	{
		HRESULT hr;
		DXGI_SWAP_CHAIN_DESC sd[ONE_WINDOW] = {};

		// デバイスの作成 (一度だけ)
		D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
		hr = D3D11CreateDevice(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
			&feature_level, 1, D3D11_SDK_VERSION, &g_Device, nullptr, &g_DeviceContext
		);

		if (FAILED(hr)) {
			return;
		}

		// IDXGIFactoryの取得
		IDXGIDevice* dxgiDevice = nullptr;
		hr = g_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
		if (FAILED(hr)) return;

		IDXGIAdapter* dxgiAdapter = nullptr;
		hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
		if (FAILED(hr)) return;

		IDXGIFactory* dxgiFactory = nullptr;
		hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
		if (FAILED(hr)) return;

		dxgiDevice->Release();
		dxgiAdapter->Release();

		g_SwapChains.push_back(new MultiSwapChains(hr, hWnd[0], dxgiFactory, 0));

		dxgiFactory->Release();

		// ビューポート設定
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		g_DeviceContext->RSSetViewports(1, &vp);

		// ラスタライザステートの作成
		D3D11_RASTERIZER_DESC rd = {};
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		//rd.CullMode = D3D11_CULL_NONE;
		rd.DepthClipEnable = TRUE;
		rd.MultisampleEnable = FALSE;
		g_Device->CreateRasterizerState(&rd, &g_RasterizerState);

		// デバイスコンテキストにラスタライザーステートを設定
		g_DeviceContext->RSSetState(g_RasterizerState);

		// ブレンドステート設定
		D3D11_BLEND_DESC bd = {};
		bd.AlphaToCoverageEnable = FALSE;
		bd.IndependentBlendEnable = FALSE;
		bd.RenderTarget[0].BlendEnable = TRUE;

		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_Device->CreateBlendState(&bd, &g_BlendState);
		g_DeviceContext->OMSetBlendState(g_BlendState, blend_factor, 0xffffffff);

		// 深度ステンシルステート設定
		D3D11_DEPTH_STENCIL_DESC dsd = {};


		// 深度有効ステート
		dsd.DepthEnable = TRUE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS;
		dsd.StencilEnable = FALSE;

		g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthEnable);

		// 深度無効ステート
		dsd.DepthEnable = FALSE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthDisable);

		// とりあえず深度有効にしておく
		g_DeviceContext->OMSetDepthStencilState(g_DepthStencilStateDepthEnable, NULL);
	}
	break;
	case GAMEMODE_LOCAL_MULTIWINDOW:
		{
			HRESULT hr;
			DXGI_SWAP_CHAIN_DESC sd[TWO_WINDOWS] = {};

			// デバイスの作成 (一度だけ)
			D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
			hr = D3D11CreateDevice(
				nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
				&feature_level, 1, D3D11_SDK_VERSION, &g_Device, nullptr, &g_DeviceContext
			);

			if (FAILED(hr)) {
				return;
			}

			// IDXGIFactoryの取得
			IDXGIDevice* dxgiDevice = nullptr;
			hr = g_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
			if (FAILED(hr)) return;

			IDXGIAdapter* dxgiAdapter = nullptr;
			hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
			if (FAILED(hr)) return;

			IDXGIFactory* dxgiFactory = nullptr;
			hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
			if (FAILED(hr)) return;

			dxgiDevice->Release();
			dxgiAdapter->Release();

			for (int i = 0; i < TWO_WINDOWS; i++)
			{
				g_SwapChains.push_back(new MultiSwapChains(hr, hWnd[i], dxgiFactory, i));
			}

			dxgiFactory->Release();

			// ビューポート設定
			D3D11_VIEWPORT vp;
			vp.Width = (FLOAT)SCREEN_WIDTH;
			vp.Height = (FLOAT)SCREEN_HEIGHT;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			g_DeviceContext->RSSetViewports(1, &vp);

			// ラスタライザステートの作成
			D3D11_RASTERIZER_DESC rd = {};
			rd.FillMode = D3D11_FILL_SOLID;
			rd.CullMode = D3D11_CULL_BACK;
			//rd.CullMode = D3D11_CULL_NONE;
			rd.DepthClipEnable = TRUE;
			rd.MultisampleEnable = FALSE;
			g_Device->CreateRasterizerState(&rd, &g_RasterizerState);

			// デバイスコンテキストにラスタライザーステートを設定
			g_DeviceContext->RSSetState(g_RasterizerState);

			// ブレンドステート設定
			D3D11_BLEND_DESC bd = {};
			bd.AlphaToCoverageEnable = FALSE;
			bd.IndependentBlendEnable = FALSE;
			bd.RenderTarget[0].BlendEnable = TRUE;

			bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

			bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

			bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			g_Device->CreateBlendState(&bd, &g_BlendState);
			g_DeviceContext->OMSetBlendState(g_BlendState, blend_factor, 0xffffffff);

			// 深度ステンシルステート設定
			D3D11_DEPTH_STENCIL_DESC dsd = {};


			// 深度有効ステート
			dsd.DepthEnable = TRUE;
			dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsd.DepthFunc = D3D11_COMPARISON_LESS;
			dsd.StencilEnable = FALSE;

			g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthEnable);

			// 深度無効ステート
			dsd.DepthEnable = FALSE;
			dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

			g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthDisable);

			// とりあえず深度有効にしておく
			g_DeviceContext->OMSetDepthStencilState(g_DepthStencilStateDepthEnable, NULL);
		}
	break;
	case GAMEMODE_SEGMENTATION:
	{
		HRESULT hr;
		DXGI_SWAP_CHAIN_DESC sd[TWO_WINDOWS] = {};

		// デバイスの作成 (一度だけ)
		D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
		hr = D3D11CreateDevice(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
			&feature_level, 1, D3D11_SDK_VERSION, &g_Device, nullptr, &g_DeviceContext
		);

		if (FAILED(hr)) {
			return;
		}

		// IDXGIFactoryの取得
		IDXGIDevice* dxgiDevice = nullptr;
		hr = g_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
		if (FAILED(hr)) return;

		IDXGIAdapter* dxgiAdapter = nullptr;
		hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
		if (FAILED(hr)) return;

		IDXGIFactory* dxgiFactory = nullptr;
		hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
		if (FAILED(hr)) return;

		dxgiDevice->Release();
		dxgiAdapter->Release();

		for (int i = 0; i < 2; i++)
		{
			if (i == 0 || i == 3)
			{
				g_SwapChains.push_back(new MultiSwapChains(hr, hWnd[0], dxgiFactory, i));
			}
			else
			{
				g_SwapChains.push_back(new MultiSwapChains(hr, hWnd[1], dxgiFactory, i));
			}
			
		}

		dxgiFactory->Release();

		// ビューポート設定
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		g_DeviceContext->RSSetViewports(1, &vp);

		// ラスタライザステートの作成
		D3D11_RASTERIZER_DESC rd = {};
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		//rd.CullMode = D3D11_CULL_NONE;
		rd.DepthClipEnable = TRUE;
		rd.MultisampleEnable = FALSE;
		g_Device->CreateRasterizerState(&rd, &g_RasterizerState);

		// デバイスコンテキストにラスタライザーステートを設定
		g_DeviceContext->RSSetState(g_RasterizerState);

		// ブレンドステート設定
		D3D11_BLEND_DESC bd = {};
		bd.AlphaToCoverageEnable = FALSE;
		bd.IndependentBlendEnable = FALSE;
		bd.RenderTarget[0].BlendEnable = TRUE;

		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_Device->CreateBlendState(&bd, &g_BlendState);
		g_DeviceContext->OMSetBlendState(g_BlendState, blend_factor, 0xffffffff);

		// 深度ステンシルステート設定
		D3D11_DEPTH_STENCIL_DESC dsd = {};


		// 深度有効ステート
		dsd.DepthEnable = TRUE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS;
		dsd.StencilEnable = FALSE;

		g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthEnable);

		// 深度無効ステート
		dsd.DepthEnable = FALSE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthDisable);

		// とりあえず深度有効にしておく
		g_DeviceContext->OMSetDepthStencilState(g_DepthStencilStateDepthEnable, NULL);
	}
	break;
	case GAMEMODE_MULTIWINDOW:
		{
			HRESULT hr;
			DXGI_SWAP_CHAIN_DESC sd[WINDOWS_COUNT] = {};

			// デバイスの作成 (一度だけ)
			D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
			hr = D3D11CreateDevice(
				nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
				&feature_level, 1, D3D11_SDK_VERSION, &g_Device, nullptr, &g_DeviceContext
			);

			if (FAILED(hr)) {
				return;
			}

			// IDXGIFactoryの取得
			IDXGIDevice* dxgiDevice = nullptr;
			hr = g_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
			if (FAILED(hr)) return;

			IDXGIAdapter* dxgiAdapter = nullptr;
			hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
			if (FAILED(hr)) return;

			IDXGIFactory* dxgiFactory = nullptr;
			hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
			if (FAILED(hr)) return;

			dxgiDevice->Release();
			dxgiAdapter->Release();

			for (int i = 0; i < WINDOWS_COUNT; i++)
			{
				g_SwapChains.push_back(new MultiSwapChains(hr, hWnd[i], dxgiFactory, i));
			}

			dxgiFactory->Release();

			// ビューポート設定
			D3D11_VIEWPORT vp;
			vp.Width = (FLOAT)SCREEN_WIDTH;
			vp.Height = (FLOAT)SCREEN_HEIGHT;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			g_DeviceContext->RSSetViewports(1, &vp);

			// ラスタライザステートの作成
			D3D11_RASTERIZER_DESC rd = {};
			rd.FillMode = D3D11_FILL_SOLID;
			rd.CullMode = D3D11_CULL_BACK;
			//rd.CullMode = D3D11_CULL_NONE;
			rd.DepthClipEnable = TRUE;
			rd.MultisampleEnable = FALSE;
			g_Device->CreateRasterizerState(&rd, &g_RasterizerState);

			// デバイスコンテキストにラスタライザーステートを設定
			g_DeviceContext->RSSetState(g_RasterizerState);

			// ブレンドステート設定
			D3D11_BLEND_DESC bd = {};
			bd.AlphaToCoverageEnable = FALSE;
			bd.IndependentBlendEnable = FALSE;
			bd.RenderTarget[0].BlendEnable = TRUE;

			bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

			bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

			bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			g_Device->CreateBlendState(&bd, &g_BlendState);
			g_DeviceContext->OMSetBlendState(g_BlendState, blend_factor, 0xffffffff);

			// 深度ステンシルステート設定
			D3D11_DEPTH_STENCIL_DESC dsd = {};


			// 深度有効ステート
			dsd.DepthEnable = TRUE;
			dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsd.DepthFunc = D3D11_COMPARISON_LESS;
			dsd.StencilEnable = FALSE;

			g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthEnable);

			// 深度無効ステート
			dsd.DepthEnable = FALSE;
			dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

			g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthDisable);

			// とりあえず深度有効にしておく
			g_DeviceContext->OMSetDepthStencilState(g_DepthStencilStateDepthEnable, NULL);
		}
	break;
	case GAMEMODE_ONLINE:
	{
		HRESULT hr;
		DXGI_SWAP_CHAIN_DESC sd[ONE_WINDOW] = {};

		// デバイスの作成 (一度だけ)
		D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
		hr = D3D11CreateDevice(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
			&feature_level, 1, D3D11_SDK_VERSION, &g_Device, nullptr, &g_DeviceContext
		);

		if (FAILED(hr)) {
			return;
		}

		// IDXGIFactoryの取得
		IDXGIDevice* dxgiDevice = nullptr;
		hr = g_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
		if (FAILED(hr)) return;

		IDXGIAdapter* dxgiAdapter = nullptr;
		hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
		if (FAILED(hr)) return;

		IDXGIFactory* dxgiFactory = nullptr;
		hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
		if (FAILED(hr)) return;

		dxgiDevice->Release();
		dxgiAdapter->Release();

		for (int i = 0; i < ONE_WINDOW; i++)
		{
			g_SwapChains.push_back(new MultiSwapChains(hr, hWnd[i], dxgiFactory, i));
		}

		dxgiFactory->Release();

		// ビューポート設定
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		g_DeviceContext->RSSetViewports(1, &vp);

		// ラスタライザステートの作成
		D3D11_RASTERIZER_DESC rd = {};
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		//rd.CullMode = D3D11_CULL_NONE;
		rd.DepthClipEnable = TRUE;
		rd.MultisampleEnable = FALSE;
		g_Device->CreateRasterizerState(&rd, &g_RasterizerState);

		// デバイスコンテキストにラスタライザーステートを設定
		g_DeviceContext->RSSetState(g_RasterizerState);

		// ブレンドステート設定
		D3D11_BLEND_DESC bd = {};
		bd.AlphaToCoverageEnable = FALSE;
		bd.IndependentBlendEnable = FALSE;
		bd.RenderTarget[0].BlendEnable = TRUE;

		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_Device->CreateBlendState(&bd, &g_BlendState);
		g_DeviceContext->OMSetBlendState(g_BlendState, blend_factor, 0xffffffff);

		// 深度ステンシルステート設定
		D3D11_DEPTH_STENCIL_DESC dsd = {};


		// 深度有効ステート
		dsd.DepthEnable = TRUE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS;
		dsd.StencilEnable = FALSE;

		g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthEnable);

		// 深度無効ステート
		dsd.DepthEnable = FALSE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthDisable);

		// とりあえず深度有効にしておく
		g_DeviceContext->OMSetDepthStencilState(g_DepthStencilStateDepthEnable, NULL);
	}
	break;
	default:
	break;
	}
}

void DirectXFinalize()
{
	for (int i = 0; i < g_SwapChains.size(); i++) {
		if (g_SwapChains[i]) {
			delete g_SwapChains[i];
			g_SwapChains[i] = nullptr;
		}
	}
	g_SwapChains.clear();

	SAFE_RELEASE(g_DepthStencilStateDepthDisable);
	SAFE_RELEASE(g_DepthStencilStateDepthEnable);
	SAFE_RELEASE(g_BlendState);
	SAFE_RELEASE(g_RasterizerState);


	SAFE_RELEASE(g_DeviceContext);
	SAFE_RELEASE(g_Device);


}


void SetDepthEnable(bool Enable)
{
	if (Enable)
	{
		g_DeviceContext->OMSetDepthStencilState(g_DepthStencilStateDepthEnable, NULL);
	}
	else
	{
		g_DeviceContext->OMSetDepthStencilState(g_DepthStencilStateDepthDisable, NULL);
	}
}