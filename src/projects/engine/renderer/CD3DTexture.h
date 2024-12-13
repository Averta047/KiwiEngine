//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

<<<<<<< HEAD
<<<<<<< HEAD
#pragma once 
=======
#pragma once
>>>>>>> parent of 04df0c7 (9.3.24 - backup)
=======
#pragma once
>>>>>>> parent of 04df0c7 (9.3.24 - backup)

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
<<<<<<< HEAD
<<<<<<< HEAD
	CD3DTexture();
	~CD3DTexture();

	bool Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContent, char* filename);
	void Shutdown();
=======
    CD3DTexture();
    ~CD3DTexture();

    bool Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, char* filename);
    void Shutdown();
>>>>>>> parent of 04df0c7 (9.3.24 - backup)

    ID3D11ShaderResourceView* GetTexture() { return m_pTextureView; }

=======
    CD3DTexture();
    ~CD3DTexture();

    bool Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, char* filename);
    void Shutdown();

    ID3D11ShaderResourceView* GetTexture() { return m_pTextureView; }

>>>>>>> parent of 04df0c7 (9.3.24 - backup)
    int GetWidth() { return m_iWidth; }
    int GetHeight() { return m_iHeight; }

private:
<<<<<<< HEAD
<<<<<<< HEAD
	bool LoadTarga32Bit(char* filename);

private:
	unsigned char* m_pTargaData;
	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pTextureView;
	int m_iWidth, m_iHeight;
};
=======
    bool LoadTarga32Bit(char*);

private:
=======
    bool LoadTarga32Bit(char*);

private:
>>>>>>> parent of 04df0c7 (9.3.24 - backup)
    unsigned char* m_pTargaData;
    ID3D11Texture2D* m_pTexture;
    ID3D11ShaderResourceView* m_pTextureView;
    int m_iWidth, m_iHeight;
<<<<<<< HEAD
};
>>>>>>> parent of 04df0c7 (9.3.24 - backup)
=======
};
>>>>>>> parent of 04df0c7 (9.3.24 - backup)
