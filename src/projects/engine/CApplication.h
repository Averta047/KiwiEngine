//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "renderer/CDirect3D11.h"
#include "renderer/CD3DCamera.h"
#include "renderer/CD3DModel.h"
#include "renderer/shaders/CColorShader.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class CApplication
{
public:
	CApplication();
	~CApplication();

	bool Initialize(int iScreenWidth, int iScreenHeight, HWND hWnd);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	CDirect3D* m_pDirect3D;
	CD3DCamera* m_pD3DCamera;
	CD3DModel* m_pD3DModel;
	CColorShader* m_pColorShader;
};