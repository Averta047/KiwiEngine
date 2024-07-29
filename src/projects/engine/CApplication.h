//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "renderer/CDirect3D11.h"

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
};