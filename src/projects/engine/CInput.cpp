//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "CInput.h"


CInput::CInput()
{
}

CInput::~CInput()
{
}

void CInput::Initialize()
{
	int i;

	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void CInput::KeyDown(unsigned int input)
{
	m_keys[input] = true;
	return;
}

void CInput::KeyUp(unsigned int input)
{
	m_keys[input] = false;
	return;
}

bool CInput::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}