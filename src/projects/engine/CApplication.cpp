//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "CApplication.h"


CApplication::CApplication()
{
	m_pGui = 0;
	m_pDirect3D = 0;
	m_pD3DCamera = 0;
	m_pD3DModel = 0;
	m_pD3DLight = 0;
	//m_pColorShader = 0;
	//m_pTextureShader = 0;
	m_pLightShader = 0;
}

CApplication::~CApplication()
{
}

bool CApplication::Initialize(int iScreenWidth, int iScreenHeight, HWND hWnd)
{
	char ModelFileName[128];
	char TextureFileName[128];
	bool result;

	// Create and initialize the Direct3D object.
	m_pDirect3D = new CDirect3D;

	result = m_pDirect3D->Initialize(iScreenWidth, iScreenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	m_pGui = new CGui();
	result = m_pGui->Initialize(hWnd, m_pDirect3D->GetDevice(), m_pDirect3D->GetDeviceContext());
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize Dear ImGui", "Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_pD3DCamera = new CD3DCamera;

	// Create and initialize the model object.
	m_pD3DModel = new CD3DModel;

	// Set the initial position of the camera.
	//m_pD3DCamera->SetPosition(0.0f, 0.0f, -5.0f);
	m_pD3DCamera->SetPosition(0.0f, 0.0f, -10.0f);

	// Set the file name of the model.
	strcpy_s(ModelFileName, "models/cube.txt");

	// Set the name of the texture file that we will be loading.
	strcpy_s(TextureFileName, "textures/stone01.tga");

	result = m_pD3DModel->Initialize(m_pDirect3D->GetDevice(), m_pDirect3D->GetDeviceContext(), ModelFileName, TextureFileName);
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}

	/*
	// Create and initialize the color shader object.
	m_pColorShader = new CColorShader;

	result = m_pColorShader->Initialize(m_pDirect3D->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize the color shader object.", "Error", MB_OK);
		return false;
	}
	*/

	/*
	// Create and initialize the texture shader object.
	m_pTextureShader = new CTextureShader;

	result = m_pTextureShader->Initialize(m_pDirect3D->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return false;
	}
	*/

	m_pLightShader = new CLightShader;

	result = m_pLightShader->Initialize(m_pDirect3D->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize the light shader object.", "Error", MB_OK);
		return false;
	}

	// Create and initialize the light object.
	m_pD3DLight = new CD3DLight;

	m_pD3DLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pD3DLight->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

void CApplication::Shutdown()
{
	/*
	// Release the color shader object.
	if (m_pColorShader)
	{
		m_pColorShader->Shutdown();
		delete m_pColorShader;
		m_pColorShader = 0;
	}
	*/

	if (m_pGui)
	{
		m_pGui->Shutdown();
		delete m_pGui;
		m_pGui = 0;
	}
	
	if (m_pD3DLight)
	{
		delete m_pD3DLight;
		m_pD3DLight = 0;
	}

	// Release the light shader object.
	if (m_pLightShader)
	{
		m_pLightShader->Shutdown();
		delete m_pLightShader;
		m_pLightShader = 0;
	}

	/*
	// Release the texture shader object.
	if (m_pTextureShader)
	{
		m_pTextureShader->Shutdown();
		delete m_pTextureShader;
		m_pTextureShader = 0;
	}
	*/

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
	XMMATRIX WorldMatrix, ViewMatrix, ProjectionMatrix, RotateMatrix, TranslateMatrix, ScaleMatrix, SrMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_pDirect3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_pD3DCamera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_pDirect3D->GetWorldMatrix(WorldMatrix);
	m_pD3DCamera->GetViewMatrix(ViewMatrix);
	m_pDirect3D->GetProjectionMatrix(ProjectionMatrix);

	RotateMatrix = XMMatrixRotationX(Cube_Rotation_X) *
		XMMatrixRotationY(Cube_Rotation_Y) *
		XMMatrixRotationZ(Cube_Rotation_Z);
    TranslateMatrix = XMMatrixTranslation(-2.0f, 0.0f, 0.0f);  // Build the translation matrix.

    // Multiply them together to create the final world transformation matrix.
	WorldMatrix = XMMatrixRotationX(Cube_Rotation_X) *
		XMMatrixRotationY(Cube_Rotation_Y) *
		XMMatrixRotationZ(Cube_Rotation_Z);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_pD3DModel->Render(m_pDirect3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_pLightShader->Render(m_pDirect3D->GetDeviceContext(), m_pD3DModel->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, m_pD3DModel->GetTexture(),
		m_pD3DLight->GetDirection(), m_pD3DLight->GetDiffuseColor());
	if (!result)
	{
		return false;
	}

	// Render Gui on top.
	m_pGui->Render();

	// Present the rendered scene to the screen.
	m_pDirect3D->EndScene();

	return true;
}

LRESULT CALLBACK CApplication::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return m_pGui->MessageHandler(hwnd, umsg, wparam, lparam);
}