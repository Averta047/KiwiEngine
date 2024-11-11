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

bool CD3DTexture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;


	// Load the targa image data into memory.
	result = LoadTarga32Bit(filename);
	if(!result)
	{
		return false;
	}

	// Setup the description of the texture.
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
	{
		return false;
	}

	// Set the row pitch of the targa image data.
	rowPitch = (m_iWidth * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	deviceContext->UpdateSubresource(m_pTexture, 0, NULL, m_pTargaData, rowPitch, 0);

	// Setup the shader resource view description.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	hResult = device->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pTextureView);
	if(FAILED(hResult))
	{
		return false;
	}

	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(m_pTextureView);

	// Release the targa image data now that the image data has been loaded into the texture.
	delete [] m_pTargaData;
	m_pTargaData = 0;

	return true;
}

void CD3DTexture::Shutdown()
{
	// Release the texture view resource.
	if(m_pTextureView)
	{
		m_pTextureView->Release();
		m_pTextureView = 0;
	}

	// Release the texture.
	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = 0;
	}

	// Release the targa data.
	if(m_pTargaData)
	{
		delete [] m_pTargaData;
		m_pTargaData = 0;
	}
}

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


	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if(error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// Get the important information from the header.
	m_iHeight = (int)targaFileHeader.height;
	m_iWidth = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if(bpp != 32)
	{
		return false;
	}

	// Calculate the size of the 32 bit image data.
	imageSize = m_iWidth * m_iHeight * 4;

	// Allocate memory for the targa image data.
	targaImage = new unsigned char[imageSize];

	// Read in the targa image data.
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if(count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if(error != 0)
	{
		return false;
	}

	// Allocate memory for the targa destination data.
	m_pTargaData = new unsigned char[imageSize];

	// Initialize the index into the targa destination data array.
	index = 0;

	// Initialize the index into the targa image data.
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

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (m_iWidth * 8);
	}

	// Release the targa image data now that it was copied into the destination array.
	delete [] targaImage;
	targaImage = 0;

	return true;
}

int CD3DTexture::GetWidth()
{
    return m_iWidth;
}

int CD3DTexture::GetHeight()
{
    return m_iHeight;
}