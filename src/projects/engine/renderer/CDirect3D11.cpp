//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "CDirect3D11.h"

CDirect3D::CDirect3D()
{
	m_pSwapChain = 0;
	m_pDevice = 0;
	m_pDeviceContext = 0;
	m_pRenderTargetView = 0;
	m_pDepthStencilBuffer = 0;
	m_pDepthStencilState = 0;
	m_pDepthStencilView = 0;
	m_pRasterState = 0;
}

CDirect3D::~CDirect3D()
{
}

bool CDirect3D::Initialize(int iScreenWidth, int iScreenHeight, bool bVSync, HWND hWnd, bool bFullScreen, float fScreenDepth, float fScreenNear)
{
	HRESULT hResult;
	IDXGIFactory* pFactory;
	IDXGIAdapter* pAdapter;
	IDXGIOutput* pAdapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long StringLength;
	DXGI_MODE_DESC* pDisplayModeList;
	DXGI_ADAPTER_DESC AdapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	D3D_FEATURE_LEVEL FeatureLevel;
	ID3D11Texture2D* pBackBuffer;
	D3D11_TEXTURE2D_DESC DepthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	D3D11_RASTERIZER_DESC RasterDesc;
	float fFieldOfView, fScreenAspect;


	// Store the vsync setting.
	m_bVSyncEnabled = bVSync;

	// Create a DirectX graphics interface factory.
	hResult = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (FAILED(hResult))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	hResult = pFactory->EnumAdapters(0, &pAdapter);
	if (FAILED(hResult))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	hResult = pAdapter->EnumOutputs(0, &pAdapterOutput);
	if (FAILED(hResult))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	hResult = pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hResult))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	pDisplayModeList = new DXGI_MODE_DESC[numModes];
	if (!pDisplayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	hResult = pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pDisplayModeList);
	if (FAILED(hResult))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
	{
		if (pDisplayModeList[i].Width == (unsigned int)iScreenWidth)
		{
			if (pDisplayModeList[i].Height == (unsigned int)iScreenHeight)
			{
				numerator = pDisplayModeList[i].RefreshRate.Numerator;
				denominator = pDisplayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	hResult = pAdapter->GetDesc(&AdapterDesc);
	if (FAILED(hResult))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_iVideoCardMemory = (int)(AdapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&StringLength, m_VideoCardDescription, 128, AdapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] pDisplayModeList;
	pDisplayModeList = 0;

	// Release the adapter output.
	pAdapterOutput->Release();
	pAdapterOutput = 0;

	// Release the adapter.
	pAdapter->Release();
	pAdapter = 0;

	// Release the factory.
	pFactory->Release();
	pFactory = 0;

	// Initialize the swap chain description.
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

	// Set to a single back buffer.
	SwapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	SwapChainDesc.BufferDesc.Width = iScreenWidth;
	SwapChainDesc.BufferDesc.Height = iScreenHeight;

	// Set regular 32-bit surface for the back buffer.
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (m_bVSyncEnabled)
	{
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	SwapChainDesc.OutputWindow = hWnd;

	// Turn multisampling off.
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (bFullScreen)
	{
		SwapChainDesc.Windowed = false;
	}
	else
	{
		SwapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	SwapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	hResult = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &FeatureLevel, 1,
		D3D11_SDK_VERSION, &SwapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext);
	if (FAILED(hResult))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	hResult = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	hResult = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	if (FAILED(hResult))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	pBackBuffer->Release();
	pBackBuffer = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&DepthBufferDesc, sizeof(DepthBufferDesc));

	// Set up the description of the depth buffer.
	DepthBufferDesc.Width = iScreenWidth;
	DepthBufferDesc.Height = iScreenHeight;
	DepthBufferDesc.MipLevels = 1;
	DepthBufferDesc.ArraySize = 1;
	DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthBufferDesc.SampleDesc.Count = 1;
	DepthBufferDesc.SampleDesc.Quality = 0;
	DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthBufferDesc.CPUAccessFlags = 0;
	DepthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	hResult = m_pDevice->CreateTexture2D(&DepthBufferDesc, NULL, &m_pDepthStencilBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));

	// Set up the description of the stencil state.
	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xFF;
	DepthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	hResult = m_pDevice->CreateDepthStencilState(&DepthStencilDesc, &m_pDepthStencilState);
	if (FAILED(hResult))
	{
		return false;
	}

	// Set the depth stencil state.
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));

	// Set up the depth stencil view description.
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hResult = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &DepthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(hResult))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	RasterDesc.AntialiasedLineEnable = false;
	RasterDesc.CullMode = D3D11_CULL_BACK;
	RasterDesc.DepthBias = 0;
	RasterDesc.DepthBiasClamp = 0.0f;
	RasterDesc.DepthClipEnable = true;
	RasterDesc.FillMode = D3D11_FILL_SOLID;
	RasterDesc.FrontCounterClockwise = false;
	RasterDesc.MultisampleEnable = false;
	RasterDesc.ScissorEnable = false;
	RasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hResult = m_pDevice->CreateRasterizerState(&RasterDesc, &m_pRasterState);
	if (FAILED(hResult))
	{
		return false;
	}

	// Now set the rasterizer state.
	m_pDeviceContext->RSSetState(m_pRasterState);

	// Setup the viewport for rendering.
	m_Viewport.Width = (float)iScreenWidth;
	m_Viewport.Height = (float)iScreenHeight;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_pDeviceContext->RSSetViewports(1, &m_Viewport);

	// Setup the projection matrix.
	fFieldOfView = 3.141592654f / 4.0f;
	fScreenAspect = (float)iScreenWidth / (float)iScreenHeight;

	// Create the projection matrix for 3D rendering.
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(fFieldOfView, fScreenAspect, fScreenNear, fScreenDepth);

	// Initialize the world matrix to the identity matrix.
	m_WorldMatrix = XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_OrthoMatrix = XMMatrixOrthographicLH((float)iScreenWidth, (float)iScreenHeight, fScreenNear, fScreenDepth);

	return true;
}

void CDirect3D::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_pSwapChain)
	{
		m_pSwapChain->SetFullscreenState(false, NULL);
	}

	if (m_pRasterState)
	{
		m_pRasterState->Release();
		m_pRasterState = 0;
	}

	if (m_pDepthStencilView)
	{
		m_pDepthStencilView->Release();
		m_pDepthStencilView = 0;
	}

	if (m_pDepthStencilState)
	{
		m_pDepthStencilState->Release();
		m_pDepthStencilState = 0;
	}

	if (m_pDepthStencilBuffer)
	{
		m_pDepthStencilBuffer->Release();
		m_pDepthStencilBuffer = 0;
	}

	if (m_pRenderTargetView)
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetView = 0;
	}

	if (m_pDeviceContext)
	{
		m_pDeviceContext->Release();
		m_pDeviceContext = 0;
	}

	if (m_pDevice)
	{
		m_pDevice->Release();
		m_pDevice = 0;
	}

	if (m_pSwapChain)
	{
		m_pSwapChain->Release();
		m_pSwapChain = 0;
	}
}

void CDirect3D::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

	// Clear the depth buffer.
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void CDirect3D::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (m_bVSyncEnabled)
	{
		// Lock to screen refresh rate.
		m_pSwapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_pSwapChain->Present(0, 0);
	}
}

void CDirect3D::GetVideoCardInfo(char* CardName, int& iMemory)
{
	strcpy_s(CardName, 128, m_VideoCardDescription);
	iMemory = m_iVideoCardMemory;
}

void CDirect3D::SetBackBufferRenderTarget()
{
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
}

void CDirect3D::ResetViewport()
{
	m_pDeviceContext->RSSetViewports(1, &m_Viewport);
}
