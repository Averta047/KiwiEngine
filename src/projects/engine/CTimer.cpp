//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "CTimer.h"

CTimer::CTimer()
{
}

CTimer::~CTimer()
{
}

bool CTimer::Initialize()
{
	INT64 frequency;

	// Get the cycles per second speed for this system.
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	if(frequency == 0)
	{
		return false;
	}
	
	// Store it in floating point.
	m_fFrequency = (float)frequency;

	// Get the initial start time.
	QueryPerformanceCounter((LARGE_INTEGER*)&m_nStartTime);

	return true;
}

void CTimer::Frame()
{
	INT64 currentTime;
	INT64 elapsedTicks;

	// Query the current time.
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	// Calculate the difference in time since the last time we queried for the current time.
	elapsedTicks = currentTime - m_nStartTime;

	// Calculate the frame time.
	m_fFrameTime = (float)elapsedTicks / m_fFrequency;

	// Restart the timer.
	m_nStartTime = currentTime;
}

float CTimer::GetTime()
{
	return m_fFrameTime;
}