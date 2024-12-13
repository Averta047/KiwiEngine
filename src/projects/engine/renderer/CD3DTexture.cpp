//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//
<<<<<<< HEAD

#include "CD3DTexture.h"
=======
>>>>>>> parent of 04df0c7 (9.3.24 - backup)

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

<<<<<<< HEAD
bool CD3DTexture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
=======
bool CD3DTexture::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, char* filename)
>>>>>>> parent of 04df0c7 (9.3.24 - backup)
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
<<<<<<< HEAD
	textureDesc.Height = m_iHeight;
	textureDesc.Width = m_iWidth;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_pTexture);
	if(FAILED(hResult))
=======
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
>>>>>>> parent of 04df0c7 (9.3.24 - backup)
	{
		return false;
	}

	// Set the row pitch of the targa image data.
<<<<<<< HEAD
	rowPitch = (m_iWidth * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	deviceContext->UpdateSubresource(m_pTexture, 0, NULL, m_pTargaData, rowPitch, 0);
=======
	RowPitch = (m_iWidth * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	pDeviceContext->UpdateSubresource(m_pTexture, 0, NULL, m_pTargaData, RowPitch, 0);
>>>>>>> parent of 04df0c7 (9.3.24 - backup)

	// Setup the shader resource view description.
	srvDesc.Format = m_pSampleState.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
<<<<<<< HEAD
	hResult = device->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pTextureView);
	if(FAILED(hResult))
=======
	hResult = pDevice->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pTextureView);
	if (FAILED(hResult))
>>>>>>> parent of 04df0c7 (9.3.24 - backup)
	{
		return false;
	}

	// Generate mipmaps for this texture.
<<<<<<< HEAD
	deviceContext->GenerateMips(m_pTextureView);

	// Release the targa image data now that the image data has been loaded into the texture.
	delete [] m_pTargaData;
=======
	pDeviceContext->GenerateMips(m_pTextureView);

	// Release the targa image data now that the image data has been loaded into the texture.
	delete[] m_pTargaData;
>>>>>>> parent of 04df0c7 (9.3.24 - backup)
	m_pTargaData = 0;

	return true;
}

void CD3DTexture::Shutdown()
{
	// Release the texture view resource.
<<<<<<< HEAD
	if(m_pTextureView)
=======
	if (m_pTextureView)
>>>>>>> parent of 04df0c7 (9.3.24 - backup)
	{
		m_pTextureView->Release();
		m_pTextureView = 0;
	}

	// Release the texture.
<<<<<<< HEAD
	if(m_pTexture)
=======
	if (m_pTexture)
>>>>>>> parent of 04df0c7 (9.3.24 - backup)
	{
		m_pTexture->Release();
		m_pTexture = 0;
	}

	// Release the targa data.
<<<<<<< HEAD
	if(m_pTargaData)
	{
		delete [] m_pTargaData;
=======
	if (m_pTargaData)
	{
		delete[] m_pTargaData;
>>>>>>> parent of 04df0c7 (9.3.24 - backup)
		m_pTargaData = 0;
	}
}

<<<<<<< HEAD
ID3D11ShaderResourceView* CD3DTexture::GetTexture()
{
	return m_pTextureView;
}

bool CD3DTexture::LoadTarga32Bit(char* filename)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;
=======
bool CD3DTexture::LoadTarga32Bit(char* filename)
{
	int iError, iBPP, iImageSize, iIndex;
	FILE* pFile;
	unsigned int iCount;
	TargaHeader TargaFileHeader;
	unsigned char* pTargaImage;
>>>>>>> parent of 04df0c7 (9.3.24 - backup)


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
<<<<<<< HEAD
	m_iHeight = (int)targaFileHeader.height;
	m_iWidth = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;
=======
	m_iHeight = (int)TargaFileHeader.height;
	m_iWidth = (int)TargaFileHeader.width;
	iBPP = (int)TargaFileHeader.bpp;
>>>>>>> parent of 04df0c7 (9.3.24 - backup)

	// Check that it is 32 bit and not 24 bit.
	if (iBPP != 32)
	{
		return false;
	}

	// Calculate the size of the 32 bit image data.
<<<<<<< HEAD
	imageSize = m_iWidth * m_iHeight * 4;
=======
	iImageSize = m_iWidth * m_iHeight * 4;
>>>>>>> parent of 04df0c7 (9.3.24 - backup)

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
<<<<<<< HEAD
	m_pTargaData = new unsigned char[imageSize];
=======
	m_pTargaData = new unsigned char[iImageSize];
>>>>>>> parent of 04df0c7 (9.3.24 - backup)

	// Initialize the index into the targa destination data array.
	iIndex = 0;

	// Initialize the index into the targa image data.
<<<<<<< HEAD
	k = (m_iWidth * m_iHeight * 4) - (m_iWidth * 4);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down and also is not in RGBA order.
	for(j=0; j< m_iHeight; j++)
	{
		for(i=0; i< m_iWidth; i++)
		{
			m_pTargaData[index + 0] = targaImage[k + 2];  // Red.
			m_pTargaData[index + 1] = targaImage[k + 1];  // Green.
			m_pTargaData[index + 2] = targaImage[k + 0];  // Blue
			m_pTargaData[index + 3] = targaImage[k + 3];  // Alpha
=======
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
>>>>>>> parent of 04df0c7 (9.3.24 - backup)

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
<<<<<<< HEAD

int CD3DTexture::GetWidth()
{
    return m_iWidth;
}

int CD3DTexture::GetHeight()
{
    return m_iHeight;
}
=======
>>>>>>> parent of 04df0c7 (9.3.24 - backup)
