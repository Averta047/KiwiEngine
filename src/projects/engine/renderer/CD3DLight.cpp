//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "CD3DLight.h"


CD3DLight::CD3DLight()
{
}

CD3DLight::~CD3DLight()
{
}


void CD3DLight::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_DiffuseColor = XMFLOAT4(red, green, blue, alpha);
}


void CD3DLight::SetDirection(float x, float y, float z)
{
	m_Direction = XMFLOAT3(x, y, z);
}
