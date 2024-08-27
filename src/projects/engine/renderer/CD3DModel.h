//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
//using namespace std;

#include "CD3DTexture.h"

class CD3DModel
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        //XMFLOAT4 color; // color shader
        XMFLOAT2 texture;
        XMFLOAT3 normal;
    };

    struct ModelType
    {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
    };

public:
    CD3DModel();
    ~CD3DModel();

    //bool Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, char* TextureFileName);
    bool Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, char* ModelFileName, char* TextureFileName);
    void Shutdown();
    void Render(ID3D11DeviceContext* pDeviceContext);

    int GetIndexCount() { return m_iIndexCount; }

    ID3D11ShaderResourceView* GetTexture();

    bool LoadTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, char* filename);
    void ReleaseTexture();

    bool LoadModel(char* filename);
    void ReleaseModel();

private:
    bool InitializeBuffers(ID3D11Device* pDevice);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext* pDeviceContext);

private:
    ID3D11Buffer* m_pVertexBuffer, * m_pIndexBuffer;
    int m_iVertexCount, m_iIndexCount;

private:
    CD3DTexture* m_pD3DTexture;
    ModelType* m_pModel;
};
