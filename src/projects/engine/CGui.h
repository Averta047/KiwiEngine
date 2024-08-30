#pragma once

#include <d3d11.h>

inline float Cube_Rotation_X = 0.0f;
inline float Cube_Rotation_Y = 0.0f;
inline float Cube_Rotation_Z = 0.0f;

class CGui
{
public:
	CGui();
	~CGui();

public:
	bool Initialize(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void Shutdown();

	void Render();

	bool MessageHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

public:
	ID3D11Device* GetDevice() { return m_pDevice; };
	ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext; };
	HWND GetWindow() { return m_hWnd; };

private:
	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
};