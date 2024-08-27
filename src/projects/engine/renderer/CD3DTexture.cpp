//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "CD3DTexture.h"

CD3DTexture::CD3DTexture()
{
	m_pTargaData = 0;
	m_pTexture = 0;
	m_pTextureView = 0;
}

CD3DTexture::~CD3DTexture()
{
}

bool CD3DTexture::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, char* filename)
{
	bool result;
	int iHeight, iWidth;
	D3D11_TEXTURE2D_DESC m_pSampleState;
	HRESULT hResult;
	unsigned int RowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	// Load the targa image data into memory.
	result = LoadTarga32Bit(filename);
	if (!result)
	{
		return false;
	}

	// Setup the description of the texture.
	m_pSampleState.Height = m_iHeight;
	m_pSampleState.Width = m_iWidth;
	m_pSampleState.MipLevels = 0;
	m_pSampleState.ArraySize = 1;
	m_pSampleState.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_pSampleState.SampleDesc.Count = 1;
	m_pSampleState.SampleDesc.Quality = 0;
	m_pSampleState.Usage = D3D11_USAGE_DEFAULT;
	m_pSampleState.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	m_pSampleState.CPUAccessFlags = 0;
	m_pSampleState.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	hResult = pDevice->CreateTexture2D(&m_pSampleState, NULL, &m_pTexture);
	if (FAILED(hResult))
	{
		return false;
	}

	// Set the row pitch of the targa image data.
	RowPitch = (m_iWidth * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	pDeviceContext->UpdateSubresource(m_pTexture, 0, NULL, m_pTargaData, RowPitch, 0);

	// Setup the shader resource view description.
	srvDesc.Format = m_pSampleState.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	hResult = pDevice->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pTextureView);
	if (FAILED(hResult))
	{
		return false;
	}

	// Generate mipmaps for this texture.
	pDeviceContext->GenerateMips(m_pTextureView);

	// Release the targa image data now that the image data has been loaded into the texture.
	delete[] m_pTargaData;
	m_pTargaData = 0;

	return true;
}

void CD3DTexture::Shutdown()
{
	// Release the texture view resource.
	if (m_pTextureView)
	{
		m_pTextureView->Release();
		m_pTextureView = 0;
	}

	// Release the texture.
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = 0;
	}

	// Release the targa data.
	if (m_pTargaData)
	{
		delete[] m_pTargaData;
		m_pTargaData = 0;
	}

	return;
}

bool CD3DTexture::LoadTarga32Bit(char* filename)
{
	int iError, iBPP, iImageSize, iIndex;
	FILE* pFile;
	unsigned int iCount;
	TargaHeader TargaFileHeader;
	unsigned char* pTargaImage;


	// Open the targa file for reading in binary.
	iError = fopen_s(&pFile, filename, "rb");
	if (iError != 0)
	{
		return false;
	}

	// Read in the file header.
	iCount = (unsigned int)fread(&TargaFileHeader, sizeof(TargaHeader), 1, pFile);
	if (iCount != 1)
	{
		return false;
	}

	// Get the important information from the header.
	m_iHeight = (int)TargaFileHeader.height;
	m_iWidth = (int)TargaFileHeader.width;
	iBPP = (int)TargaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if (iBPP != 32)
	{
		return false;
	}

	// Calculate the size of the 32 bit image data.
	iImageSize = m_iWidth * m_iHeight * 4;

	// Allocate memory for the targa image data.
	pTargaImage = new unsigned char[iImageSize];

	// Read in the targa image data.
	iCount = (unsigned int)fread(pTargaImage, 1, iImageSize, pFile);
	if (iCount != iImageSize)
	{
		return false;
	}

	// Close the file.
	iError = fclose(pFile);
	if (iError != 0)
	{
		return false;
	}

	// Allocate memory for the targa destination data.
	m_pTargaData = new unsigned char[iImageSize];

	// Initialize the index into the targa destination data array.
	iIndex = 0;

	// Initialize the index into the targa image data.
	int k = (m_iWidth * m_iHeight * 4) - (m_iWidth * 4);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down and also is not in RGBA order.
	for (int j = 0; j < m_iHeight; j++)
	{
		for (int i = 0; i < m_iWidth; i++)
		{
			m_pTargaData[iIndex + 0] = pTargaImage[k + 2];  // Red.
			m_pTargaData[iIndex + 1] = pTargaImage[k + 1];  // Green.
			m_pTargaData[iIndex + 2] = pTargaImage[k + 0];  // Blue
			m_pTargaData[iIndex + 3] = pTargaImage[k + 3];  // Alpha

			// Increment the indexes into the targa data.
			k += 4;
			iIndex += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (m_iWidth * 8);
	}

	// Release the targa image data now that it was copied into the destination array.
	delete[] pTargaImage;
	pTargaImage = 0;

	return true;
}
