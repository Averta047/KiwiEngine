//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "CD3DModel.h"

CD3DModel::CD3DModel()
{
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;

	m_pD3DTexture = 0;
	m_pModel = 0;
}

CD3DModel::~CD3DModel()
{
}

bool CD3DModel::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, char* ModelFileName, char* TextureFileName)
{
	bool result;

	// Load in the model data.
	result = LoadModel(ModelFileName);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(pDevice);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(pDevice, pDeviceContext, TextureFileName);
	if (!result)
	{
		return false;
	}

	return true;
}

void CD3DModel::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
}

void CD3DModel::Render(ID3D11DeviceContext* pDeviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(pDeviceContext);

	return;
}

ID3D11ShaderResourceView* CD3DModel::GetTexture()
{
	return m_pD3DTexture->GetTexture();
}

bool CD3DModel::InitializeBuffers(ID3D11Device* pDevice)
{
	VertexType* pVertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData, IndexData;
	HRESULT hResult;

	// Create the vertex array.
	pVertices = new VertexType[m_iVertexCount];
	if (!pVertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_iIndexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (int i = 0; i < m_iVertexCount; i++)
	{
		pVertices[i].position = XMFLOAT3(m_pModel[i].x, m_pModel[i].y, m_pModel[i].z);
		pVertices[i].texture = XMFLOAT2(m_pModel[i].tu, m_pModel[i].tv);
		pVertices[i].normal = XMFLOAT3(m_pModel[i].nx, m_pModel[i].ny, m_pModel[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * m_iVertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	VertexData.pSysMem = pVertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	hResult = pDevice->CreateBuffer(&VertexBufferDesc, &VertexData, &m_pVertexBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * m_iIndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	IndexData.pSysMem = indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	hResult = pDevice->CreateBuffer(&IndexBufferDesc, &IndexData, &m_pIndexBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] pVertices;
	pVertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void CD3DModel::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = 0;
	}
}

void CD3DModel::RenderBuffers(ID3D11DeviceContext* pDeviceContext)
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

bool CD3DModel::LoadTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, char* filename)
{
	bool result;


	// Create and initialize the texture object.
	m_pD3DTexture = new CD3DTexture;

	result = m_pD3DTexture->Initialize(pDevice, pDeviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void CD3DModel::ReleaseTexture()
{
	// Release the texture object.
	if (m_pD3DTexture)
	{
		m_pD3DTexture->Shutdown();
		delete m_pD3DTexture;
		m_pD3DTexture = 0;
	}
}

bool CD3DModel::LoadModel(char* filename)
{
	std::ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_iVertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_iIndexCount = m_iVertexCount;

	// Create the model using the vertex count that was read in.
	m_pModel = new ModelType[m_iVertexCount];

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < m_iVertexCount; i++)
	{
		fin >> m_pModel[i].x >> m_pModel[i].y >> m_pModel[i].z;
		fin >> m_pModel[i].tu >> m_pModel[i].tv;
		fin >> m_pModel[i].nx >> m_pModel[i].ny >> m_pModel[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

void CD3DModel::ReleaseModel()
{
	if (m_pModel)
	{
		delete[] m_pModel;
		m_pModel = 0;
	}

	return;
}