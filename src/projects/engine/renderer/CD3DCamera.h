//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include <directxmath.h>
using namespace DirectX;

class CD3DCamera
{
public:
	CD3DCamera();
	~CD3DCamera();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	XMFLOAT3 GetPosition() { return XMFLOAT3(m_fPositionX, m_fPositionY, m_fPositionZ); }
	XMFLOAT3 GetRotation() { return XMFLOAT3(m_fRotationX, m_fRotationY, m_fRotationZ); }

	void Render();
	void GetViewMatrix(XMMATRIX& ViewMatrix) { ViewMatrix = m_ViewMatrix; };

private:
	float m_fPositionX, m_fPositionY, m_fPositionZ;
	float m_fRotationX, m_fRotationY, m_fRotationZ;
	XMMATRIX m_ViewMatrix;
};