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

	void SetAmbientColor(float red, float green, float blue, float alpha);
	void SetDiffuseColor(float red, float green, float blue, float alpha);
	void SetDirection(float x, float y, float z);
	void SetSpecularColor(float red, float green, float blue, float alpha);
	void SetSpecularPower(float power);
	void SetPosition(float x, float y, float z);

	XMFLOAT4 GetAmbientColor() { return m_AmbientColor; }
	XMFLOAT4 GetDiffuseColor() { return m_DiffuseColor; }
	XMFLOAT3 GetDirection() { return m_Direction; }
	XMFLOAT4 GetSpecularColor() { return m_SpecularColor; }
	float GetSpecularPower() { return m_fSpecularPower; }
	XMFLOAT4 GetPosition() { return m_Position; }

private:
	XMFLOAT4 m_AmbientColor;
	XMFLOAT4 m_DiffuseColor;
	XMFLOAT3 m_Direction;
	XMFLOAT4 m_SpecularColor;
	float m_fSpecularPower;
	XMFLOAT4 m_Position;
};
