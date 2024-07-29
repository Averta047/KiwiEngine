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
}

CD3DModel::~CD3DModel()
{
}

bool CD3DModel::Initialize(ID3D11Device* pDevice)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(pDevice);
	if (!result)
	{
		return false;
	}

	return true;
}

void CD3DModel::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}

void CD3DModel::Render(ID3D11DeviceContext* pDeviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(pDeviceContext);

	return;
}

bool CD3DModel::InitializeBuffers(ID3D11Device* pDevice)
{
	VertexType* pVertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData, IndexData;
	HRESULT hResult;

	// Set the number of vertices in the vertex array.
	m_iVertexCount = 3;

	// Set the number of indices in the index array.
	m_iIndexCount = 3;

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

	// Load the vertex array with data.
	pVertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	pVertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	pVertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	pVertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	pVertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	pVertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

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