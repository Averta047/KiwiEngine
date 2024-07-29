//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

using namespace DirectX;
using namespace std;

class CColorShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	CColorShader();
	~CColorShader();

	bool Initialize(ID3D11Device* pDevice, HWND hWnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* pDeviceContext, int iIndexCount, XMMATRIX WorldMatrix, XMMATRIX ViewMatrix, XMMATRIX ProjectionMatrix);

private:
	bool InitializeShader(ID3D11Device* pDevice, HWND hWnd, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* pErrorMessage, HWND hWnd, WCHAR* ShaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* pDeviceContext, XMMATRIX WorldMatrix, XMMATRIX ViewMatrix, XMMATRIX ProjectionMatrix);
	void RenderShader(ID3D11DeviceContext* pDeviceContext, int iIndexCount);

private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;
};