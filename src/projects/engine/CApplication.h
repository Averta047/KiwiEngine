//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: Rename this to CRenderer
//
//================================================================//

#pragma once

#include "CGui.h"

#include "CTimer.h"
#include "renderer/CDirect3D11.h"
#include "renderer/CD3DCamera.h"
#include "renderer/CD3DModel.h"
#include "renderer/CD3DLight.h"
#include "renderer/CD3DSprite.h"
#include "renderer/shaders/CLightShader.h"
#include "renderer/shaders/CTextureShader.h"

const bool FULL_SCREEN = true;
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

	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // delete me

private:
	bool Render();

private:
	CGui* m_pGui; // don't keep this

private:
	CTimer* m_pTimer;
	CDirect3D* m_pDirect3D;
	CD3DCamera* m_pD3DCamera;
	CD3DModel* m_pD3DModel;
	CD3DLight* m_pD3DLight;
	CD3DSprite* m_pD3DSprite;
	//CColorShader* m_pColorShader;
	CTextureShader* m_pTextureShader;
	CLightShader* m_pLightShader;
	int m_iNumLights;
};