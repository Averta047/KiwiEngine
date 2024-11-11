//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "CD3DSprite.h"

CD3DSprite::CD3DSprite()
{
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;
	m_pTextures = 0;
}

CD3DSprite::~CD3DSprite()
{
}

bool CD3DSprite::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, int iScreenWidth, int iScreenHeight, char* SpriteFilename, int iRenderX, int iRenderY)
{
	bool result;


	// Store the screen size.
	m_iScreenWidth = iScreenWidth;
	m_iScreenHeight = iScreenHeight;

	// Store where the sprite should be rendered to.
    m_iRenderX = iRenderX;
    m_iRenderY = iRenderY;

	// Initialize the frame time for this sprite object.
    m_fFrameTime = 0;

	// Initialize the vertex and index buffer that hold the geometry for the sprite bitmap.
	result = InitializeBuffers(pDevice);
	if(!result)
	{
		return false;
	}

	// Load the textures for this sprite.
	result = LoadTextures(pDevice, pDeviceContext, SpriteFilename);
	if(!result)
	{
		return false;
	}

	return true;
}

void CD3DSprite::Shutdown()
{
	// Release the textures used for this sprite.
	ReleaseTextures();

	// Release the vertex and index buffers.
	ShutdownBuffers();
}

bool CD3DSprite::Render(ID3D11DeviceContext* pDeviceContext)
{
	bool result;


	// Update the buffers if the position of the sprite has changed from its original position.
	result = UpdateBuffers(pDeviceContext);
	if(!result)
	{
		return false;
	}

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(pDeviceContext);

	return true;
}

void CD3DSprite::Update(float fFrameTime)
{
    // Increment the frame time each frame.
    m_fFrameTime += fFrameTime;

    // Check if the frame time has reached the cycle time.
    if(m_fFrameTime >= m_fCycleTime)
    {
        // If it has then reset the frame time and cycle to the next sprite in the texture array.
        m_fFrameTime -= m_fCycleTime;

        m_iCurrentTexture++;

        // If we are at the last sprite texture then go back to the beginning of the texture array to the first texture again.
        if(m_iCurrentTexture == m_iTextureCount)
        {
            m_iCurrentTexture = 0;
        }
    }
}

int CD3DSprite::GetIndexCount()
{
	return m_iIndexCount;
}

ID3D11ShaderResourceView* CD3DSprite::GetTexture()
{
	return m_pTextures[m_iCurrentTexture].GetTexture();
}

bool CD3DSprite::InitializeBuffers(ID3D11Device* pDevice)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Initialize the previous rendering position to negative one.
    m_iPrevPosX = -1;
    m_iPrevPosY = -1;

	// Set the number of vertices in the vertex array.
	m_iVertexCount = 6;

	// Set the number of indices in the index array.
	m_iIndexCount = m_iVertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_iVertexCount];

	// Create the index array.
	indices = new unsigned long[m_iIndexCount];

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_iVertexCount));

	// Load the index array with data.
	for(i=0; i<m_iIndexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_iVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	result = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_iIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void CD3DSprite::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = 0;
	}
}

bool CD3DSprite::UpdateBuffers(ID3D11DeviceContext* pDeviceContext)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* dataPtr;
	HRESULT result;


	 // If the position we are rendering this bitmap to hasn't changed then don't update the vertex buffer.
    if((m_iPrevPosX == m_iRenderX) && (m_iPrevPosY == m_iRenderY))
    {
        return true;
    }

	// If the rendering location has changed then store the new position and update the vertex buffer.
    m_iPrevPosX = m_iRenderX;
    m_iPrevPosY = m_iRenderY;

	// Create the vertex array.
	vertices = new VertexType[m_iVertexCount];

	// Calculate the screen coordinates of the left side of the bitmap.
	left = (float)((m_iScreenWidth / 2) * -1) + (float)m_iRenderX;

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + (float)m_iBitmapWidth;

	// Calculate the screen coordinates of the top of the bitmap.
	top = (float)(m_iScreenHeight / 2) - (float)m_iRenderY;

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - (float)m_iBitmapHeight;

	// Load the vertex array with data.
	// First triangle.
	vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	// Lock the vertex buffer.
	result = pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(dataPtr, (void*)vertices, (sizeof(VertexType) * m_iVertexCount));

	// Unlock the vertex buffer.
	pDeviceContext->Unmap(m_pVertexBuffer, 0);

	// Release the pointer reference.
	dataPtr = 0;

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}

void CD3DSprite::RenderBuffers(ID3D11DeviceContext* pDeviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool CD3DSprite::LoadTextures(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, char* Filename)
{
	char textureFilename[128];
    std::ifstream fin;
    int i, j;
    char input;
    bool result;


    // Open the sprite info data file.
    fin.open(Filename);
    if(fin.fail())
    {
        return false;
    }

    // Read in the number of textures.
    fin >> m_iTextureCount;

    // Create and initialize the texture array with the texture count from the file.
    m_pTextures = new CD3DTexture[m_iTextureCount];

    // Read to start of next line.
    fin.get(input);

    // Read in each texture file name.
    for(i = 0; i < m_iTextureCount; i++)
    {
        j=0;
        fin.get(input);
        while(input != '\n')
        {
            textureFilename[j] = input;
            j++;
            fin.get(input);
        }
        textureFilename[j] = '\0';

        // Once you have the filename then load the texture in the texture array.
        result = m_pTextures[i].Initialize(pDevice, pDeviceContext, textureFilename);
        if(!result)
        {
            return false;
        }
    }

    // Read in the cycle time.
    fin >> m_fCycleTime;

	// Convert the integer milliseconds to float representation.
	m_fCycleTime = m_fCycleTime * 0.001f;

    // Close the file.
    fin.close();

    // Get the dimensions of the first texture and use that as the dimensions of the 2D sprite images.
    m_iBitmapWidth = m_pTextures[0].GetWidth();
    m_iBitmapHeight = m_pTextures[0].GetHeight();

    // Set the starting texture in the cycle to be the first one in the list.
    m_iCurrentTexture = 0;

	return true;
}

void CD3DSprite::ReleaseTextures()
{
    int i;


    // Release the texture objects.
    if(m_pTextures)
    {
        for(i=0; i<m_iTextureCount; i++)
        {
            m_pTextures[i].Shutdown();
        }

        delete [] m_pTextures;
        m_pTextures = 0;
    }
}

void CD3DSprite::SetRenderLocation(int x, int y)
{
	m_iRenderX = x;
	m_iRenderY = y;
}