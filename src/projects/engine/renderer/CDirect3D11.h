//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class CDirect3D
{
public:
    CDirect3D();
    ~CDirect3D();

    bool Initialize(int iScreenWidth, int iScreenHeight, bool bVSync, HWND hWnd, bool nFullScreen, float fScreenDepth, float fScreenNear);
    void Shutdown();

    void BeginScene(float, float, float, float);
    void EndScene();

    ID3D11Device* GetDevice() { return m_pDevice; }
    ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext; }

    void GetProjectionMatrix(XMMATRIX& ProjectionMatrix) { ProjectionMatrix = m_ProjectionMatrix; }
    void GetWorldMatrix(XMMATRIX& WorldMatrix) { WorldMatrix = m_WorldMatrix; }
    void GetOrthoMatrix(XMMATRIX& OrthoMatrix) { OrthoMatrix = m_OrthoMatrix; }

    void GetVideoCardInfo(char* CardName, int& iMemory);

    void SetBackBufferRenderTarget();
    void ResetViewport();

private:
    bool m_bVSyncEnabled;
    int m_iVideoCardMemory;
    char m_VideoCardDescription[128];

private:
    IDXGISwapChain* m_pSwapChain;
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pDeviceContext;
    ID3D11RenderTargetView* m_pRenderTargetView;
    ID3D11Texture2D* m_pDepthStencilBuffer;
    ID3D11DepthStencilState* m_pDepthStencilState;
    ID3D11DepthStencilView* m_pDepthStencilView;
    ID3D11RasterizerState* m_pRasterState;
    XMMATRIX m_ProjectionMatrix;
    XMMATRIX m_WorldMatrix;
    XMMATRIX m_OrthoMatrix;
    D3D11_VIEWPORT m_Viewport;
};