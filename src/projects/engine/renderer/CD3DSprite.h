//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include <directxmath.h>
#include <fstream>

using namespace DirectX;
//using namespace std;

#include "CD3DTexture.h"

class CD3DSprite
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	CD3DSprite();
	~CD3DSprite();

	bool Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, int iScreenWidth, int iScreenHeight, char* SpriteFilename, int iRenderX, int iRenderY);
	void Shutdown();
	bool Render(ID3D11DeviceContext* pDeviceContext);

	void Update(float fFrameTime);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	void SetRenderLocation(int x, int y);

private:
	bool InitializeBuffers(ID3D11Device* pDevice);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext* pDeviceContext);
	void RenderBuffers(ID3D11DeviceContext* pDeviceContext);

	bool LoadTextures(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, char* SpriteFilename);
	void ReleaseTextures();

private:
	ID3D11Buffer *m_pVertexBuffer, *m_pIndexBuffer;
	int m_iVertexCount, m_iIndexCount, m_iScreenWidth, m_iScreenHeight, m_iBitmapWidth, m_iBitmapHeight, m_iRenderX, m_iRenderY, m_iPrevPosX, m_iPrevPosY;
	CD3DTexture* m_pTextures;
	float m_fFrameTime, m_fCycleTime;
	int m_iCurrentTexture, m_iTextureCount;
};
