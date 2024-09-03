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

void CD3DLight::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_AmbientColor = XMFLOAT4(red, green, blue, alpha);
}

void CD3DLight::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_DiffuseColor = XMFLOAT4(red, green, blue, alpha);
}


void CD3DLight::SetDirection(float x, float y, float z)
{
	m_Direction = XMFLOAT3(x, y, z);
}

void CD3DLight::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_SpecularColor = XMFLOAT4(red, green, blue, alpha);
}

void CD3DLight::SetSpecularPower(float power)
{
	m_fSpecularPower = power;
}

void CD3DLight::SetPosition(float x, float y, float z)
{
	m_Position = XMFLOAT4(x, y, z, 1.0f);
}
