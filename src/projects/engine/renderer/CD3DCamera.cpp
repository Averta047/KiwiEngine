//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "CD3DCamera.h"

CD3DCamera::CD3DCamera()
{
	m_fPositionX = 0.0f;
	m_fPositionY = 0.0f;
	m_fPositionZ = 0.0f;

	m_fRotationX = 0.0f;
	m_fRotationY = 0.0f;
	m_fRotationZ = 0.0f;
}

CD3DCamera::~CD3DCamera()
{
}

void CD3DCamera::SetPosition(float x, float y, float z)
{
	m_fPositionX = x;
	m_fPositionY = y;
	m_fPositionZ = z;
}


void CD3DCamera::SetRotation(float x, float y, float z)
{
	m_fRotationX = x;
	m_fRotationY = y;
	m_fRotationZ = z;
}

void CD3DCamera::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position.x = m_fPositionX;
	position.y = m_fPositionY;
	position.z = m_fPositionZ;

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_fRotationX * 0.0174532925f;
	yaw = m_fRotationY * 0.0174532925f;
	roll = m_fRotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_ViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}