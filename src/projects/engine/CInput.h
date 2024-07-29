//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

class CInput
{
public:
	CInput();
	~CInput();

	void Initialize();

	void KeyDown(unsigned int input);
	void KeyUp(unsigned int input);

	bool IsKeyDown(unsigned int key);

private:
	bool m_keys[256];
};
