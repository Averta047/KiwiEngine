//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include <directxmath.h>
using namespace DirectX;

class CD3DLight
{
public:
	CD3DLight();
	~CD3DLight();

	void SetDiffuseColor(float red, float green, float blue, float alpha);
	void SetDirection(float x, float y, float z);
	
	XMFLOAT4 GetDiffuseColor() { return m_DiffuseColor; }
	XMFLOAT3 GetDirection() { return m_Direction; }
	
private:
	XMFLOAT4 m_DiffuseColor;
	XMFLOAT3 m_Direction;
};
