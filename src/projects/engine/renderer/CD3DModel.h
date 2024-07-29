//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class CD3DModel
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

public:
    CD3DModel();
    ~CD3DModel();

    bool Initialize(ID3D11Device* pDevice);
    void Shutdown();
    void Render(ID3D11DeviceContext* pDeviceContext);

    int GetIndexCount() { return m_iIndexCount; }

private:
    bool InitializeBuffers(ID3D11Device* pDevice);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext* pDeviceContext);

private:
    ID3D11Buffer* m_pVertexBuffer, * m_pIndexBuffer;
    int m_iVertexCount, m_iIndexCount;
};
