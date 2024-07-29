//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "CInput.h"
#include "CApplication.h"

class CSystem
{
public:
	CSystem();
	~CSystem();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	bool Frame();
	void InitializeWindows(int& iScreenWidth, int& iScreenHeight);
	void ShutdownWindows();

private:
	LPCSTR m_ApplicationName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;

private:
	CInput* m_pInput;
	CApplication* m_pApplication;
};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static CSystem* ApplicationHandle = 0;