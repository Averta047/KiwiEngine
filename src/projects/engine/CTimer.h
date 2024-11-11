//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include <windows.h>

class CTimer
{
public:
	CTimer();
	~CTimer();

	bool Initialize();
	void Frame();

	float GetTime();

private:
	float m_fFrequency;
	INT64 m_nStartTime;
	float m_fFrameTime;
};
