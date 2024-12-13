//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "CLightShader.h"

CLightShader::CLightShader()
{
	m_pVertexShader = 0;
	m_pPixelShader = 0;
	m_pLayout = 0;
	m_pSampleState = 0;
	m_pMatrixBuffer = 0;
	m_pLightBuffer = 0;
}

CLightShader::~CLightShader()
{
}


bool CLightShader::Initialize(ID3D11Device* pDevice, HWND hWnd)
{
	wchar_t vsFilename[128];
	wchar_t psFilename[128];
	int error;
	bool result;

	// Set the filename of the vertex shader.
	error = wcscpy_s(vsFilename, 128, L"shaders/light.vs");
	if (error != 0)
	{
		return false;
	}

	// Set the filename of the pixel shader.
	error = wcscpy_s(psFilename, 128, L"shaders/light.ps");
	if (error != 0)
	{
		return false;
	}

	// Initialize the vertex and pixel shaders.
	result = InitializeShader(pDevice, hWnd, vsFilename, psFilename);
	if (!result)
	{
		return false;
	}

	return true;
}


void CLightShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();
}


bool CLightShader::Render(ID3D11DeviceContext* pDeviceContext, int iIndexCount, XMMATRIX WorldMatrix, XMMATRIX ViewMatrix, XMMATRIX ProjectionMatrix, ID3D11ShaderResourceView* pTexture, XMFLOAT3 LightDirection, XMFLOAT4 DiffuseColor)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(pDeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, pTexture, LightDirection, DiffuseColor);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(pDeviceContext, iIndexCount);

	return true;
}


bool CLightShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT hResult;
	ID3D10Blob* pErrorMessage;
	ID3D10Blob* pVertexShaderBuffer;
	ID3D10Blob* pPixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC SamplerDesc;
	D3D11_BUFFER_DESC MatrixBufferDesc;
	D3D11_BUFFER_DESC LightBufferDesc;


	// Initialize the pointers this function will use to null.
	pErrorMessage = 0;
	pVertexShaderBuffer = 0;
	pPixelShaderBuffer = 0;

	// Compile the vertex shader code.
	hResult = D3DCompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pVertexShaderBuffer, &pErrorMessage);
	if (FAILED(hResult))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (pErrorMessage)
		{
			OutputShaderErrorMessage(pErrorMessage, hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBoxW(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	hResult = D3DCompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pPixelShaderBuffer, &pErrorMessage);
	if (FAILED(hResult))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (pErrorMessage)
		{
			OutputShaderErrorMessage(pErrorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBoxW(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	hResult = device->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hResult))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	hResult = device->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(), pPixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(hResult))
	{
		return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	PolygonLayout[0].SemanticName = "POSITION";
	PolygonLayout[0].SemanticIndex = 0;
	PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[0].InputSlot = 0;
	PolygonLayout[0].AlignedByteOffset = 0;
	PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[0].InstanceDataStepRate = 0;

	PolygonLayout[1].SemanticName = "TEXCOORD";
	PolygonLayout[1].SemanticIndex = 0;
	PolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	PolygonLayout[1].InputSlot = 0;
	PolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[1].InstanceDataStepRate = 0;

	PolygonLayout[2].SemanticName = "NORMAL";
	PolygonLayout[2].SemanticIndex = 0;
	PolygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[2].InputSlot = 0;
	PolygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(PolygonLayout) / sizeof(PolygonLayout[0]);

	// Create the vertex input layout.
	hResult = device->CreateInputLayout(PolygonLayout, numElements, pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(),
		&m_pLayout);
	if (FAILED(hResult))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	pVertexShaderBuffer->Release();
	pVertexShaderBuffer = 0;

	pPixelShaderBuffer->Release();
	pPixelShaderBuffer = 0;

	// Create a texture sampler state description.
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0.0f;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerDesc.BorderColor[0] = 0;
	SamplerDesc.BorderColor[1] = 0;
	SamplerDesc.BorderColor[2] = 0;
	SamplerDesc.BorderColor[3] = 0;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hResult = device->CreateSamplerState(&SamplerDesc, &m_pSampleState);
	if (FAILED(hResult))
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hResult = device->CreateBuffer(&MatrixBufferDesc, NULL, &m_pMatrixBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	LightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	LightBufferDesc.ByteWidth = sizeof(LightBufferType);
	LightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	LightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	LightBufferDesc.MiscFlags = 0;
	LightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hResult = device->CreateBuffer(&LightBufferDesc, NULL, &m_pLightBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}


void CLightShader::ShutdownShader()
{
	// Release the light constant buffer.
	if (m_pLightBuffer)
	{
		m_pLightBuffer->Release();
		m_pLightBuffer = 0;
	}

	// Release the matrix constant buffer.
	if (m_pMatrixBuffer)
	{
		m_pMatrixBuffer->Release();
		m_pMatrixBuffer = 0;
	}

	// Release the sampler state.
	if (m_pSampleState)
	{
		m_pSampleState->Release();
		m_pSampleState = 0;
	}

	// Release the layout.
	if (m_pLayout)
	{
		m_pLayout->Release();
		m_pLayout = 0;
	}

	// Release the pixel shader.
	if (m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = 0;
	}

	// Release the vertex shader.
	if (m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = 0;
	}
}


void CLightShader::OutputShaderErrorMessage(ID3D10Blob* pErrorMessage, HWND hWnd, WCHAR* ShaderFileName)
{
	char* CompileErrors;
	unsigned __int64 iBufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	CompileErrors = (char*)(pErrorMessage->GetBufferPointer());

	// Get the length of the message.
	iBufferSize = pErrorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < iBufferSize; i++)
	{
		fout << CompileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	pErrorMessage->Release();
	pErrorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBoxW(hWnd, L"Error compiling shader.  Check shader-error.txt for message.", ShaderFileName, MB_OK);
}


bool CLightShader::SetShaderParameters(ID3D11DeviceContext* pDeviceContext, XMMATRIX WorldMatrix, XMMATRIX ViewMatrix, XMMATRIX ProjectionMatrix, ID3D11ShaderResourceView* pTexture, XMFLOAT3 LightDirection, XMFLOAT4 DiffuseColor)
{
	HRESULT hResult;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	unsigned int iBufferNumber;
	MatrixBufferType* pData;
	LightBufferType* pData2;


	// Transpose the matrices to prepare them for the shader.
	WorldMatrix = XMMatrixTranspose(WorldMatrix);
	ViewMatrix = XMMatrixTranspose(ViewMatrix);
	ProjectionMatrix = XMMatrixTranspose(ProjectionMatrix);

	// Lock the constant buffer so it can be written to.
	hResult = pDeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(hResult))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	pData = (MatrixBufferType*)MappedResource.pData;

	// Copy the matrices into the constant buffer.
	pData->world = WorldMatrix;
	pData->view = ViewMatrix;
	pData->projection = ProjectionMatrix;

	// Unlock the constant buffer.
	pDeviceContext->Unmap(m_pMatrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	iBufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->VSSetConstantBuffers(iBufferNumber, 1, &m_pMatrixBuffer);

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	// Lock the light constant buffer so it can be written to.
	hResult = pDeviceContext->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(hResult))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	pData2 = (LightBufferType*)MappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	pData2->diffuseColor = DiffuseColor;
	pData2->lightDirection = LightDirection;
	pData2->padding = 0.0f;

	// Unlock the constant buffer.
	pDeviceContext->Unmap(m_pLightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	iBufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	pDeviceContext->PSSetConstantBuffers(iBufferNumber, 1, &m_pLightBuffer);

	return true;
}


void CLightShader::RenderShader(ID3D11DeviceContext* pDeviceContext, int iIndexCount)
{
	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(m_pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleState);

	// Render the triangle.
	pDeviceContext->DrawIndexed(iIndexCount, 0, 0);
}