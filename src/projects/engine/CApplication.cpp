//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "CApplication.h"


CApplication::CApplication()
{
	m_pDirect3D = 0;
	m_pD3DCamera = 0;
	m_pD3DModel = 0;
	m_pColorShader = 0;
}

CApplication::~CApplication()
{
}

bool CApplication::Initialize(int iScreenWidth, int iScreenHeight, HWND hWnd)
{
	bool result;


	// Create and initialize the Direct3D object.
	m_pDirect3D = new CDirect3D;

	result = m_pDirect3D->Initialize(iScreenWidth, iScreenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_pD3DCamera = new CD3DCamera;

	// Set the initial position of the camera.
	m_pD3DCamera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create and initialize the model object.
	m_pD3DModel = new CD3DModel;

	result = m_pD3DModel->Initialize(m_pDirect3D->GetDevice());
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}

	// Create and initialize the color shader object.
	m_pColorShader = new CColorShader;

	result = m_pColorShader->Initialize(m_pDirect3D->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize the color shader object.", "Error", MB_OK);
		return false;
	}

	return true;
}

void CApplication::Shutdown()
{
	// Release the color shader object.
	if (m_pColorShader)
	{
		m_pColorShader->Shutdown();
		delete m_pColorShader;
		m_pColorShader = 0;
	}

	// Release the model object.
	if (m_pD3DModel)
	{
		m_pD3DModel->Shutdown();
		delete m_pD3DModel;
		m_pD3DModel = 0;
	}

	// Release the camera object.
	if (m_pD3DCamera)
	{
		delete m_pD3DCamera;
		m_pD3DCamera = 0;
	}

	// Release the Direct3D object.
	if (m_pDirect3D)
	{
		m_pDirect3D->Shutdown();
		delete m_pDirect3D;
		m_pDirect3D = 0;
	}
}

bool CApplication::Frame()
{
	bool result;


	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool CApplication::Render()
{
	XMMATRIX WorldMatrix, ViewMatrix, ProjectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_pDirect3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_pD3DCamera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_pDirect3D->GetWorldMatrix(WorldMatrix);
	m_pD3DCamera->GetViewMatrix(ViewMatrix);
	m_pDirect3D->GetProjectionMatrix(ProjectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_pD3DModel->Render(m_pDirect3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_pColorShader->Render(m_pDirect3D->GetDeviceContext(), m_pD3DModel->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix);
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_pDirect3D->EndScene();

	return true;
}