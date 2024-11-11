//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once 

#include <d3d11.h>
#include <stdio.h>

class CD3DTexture
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	CD3DTexture();
	~CD3DTexture();

	bool Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContent, char* filename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	int GetWidth();
    int GetHeight();

private:
	bool LoadTarga32Bit(char* filename);

private:
	unsigned char* m_pTargaData;
	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pTextureView;
	int m_iWidth, m_iHeight;
};
