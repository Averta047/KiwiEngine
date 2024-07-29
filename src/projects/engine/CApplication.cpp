//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "CApplication.h"


CApplication::CApplication()
{
	m_pDirect3D = 0;
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

	return true;
}

void CApplication::Shutdown()
{
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
	// Clear the buffers to begin the scene.
	m_pDirect3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);


	// Present the rendered scene to the screen.
	m_pDirect3D->EndScene();

	return true;
}