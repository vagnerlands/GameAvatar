#include "CCamera.h"

#define PI 3.1415926535897932384626433832795
#define PIdiv180 (PI/180.0)

CCamera::CCamera() :
	m_position(0.0f, 0.0f, 0.0f),
	m_rightVector(0.0f, 0.0f, 0.0f),
	m_upVector(0.0f, 0.0f, 0.0f),
	m_viewDir(0.0f, 0.0f, 0.0f),
	m_rotation(0.0f, 0.0f, 0.0f)
{

}

void CCamera::SetLookAtMatrix()
{
	glm::vec3 viewPoint = m_position + m_viewDir;

	gluLookAt(m_position.x, m_position.y, m_position.z, viewPoint.x, viewPoint.y, viewPoint.z, m_upVector.x, m_upVector.y, m_upVector.z);
}

void CCamera::RotateX(TFloat Angle)
{
	m_rotation.x += Angle;

	printf("ang=%f totalangX = %f - ", Angle, m_rotation.x);

	//Rotate viewdir around the right vector:
	m_viewDir = glm::normalize(m_viewDir * cos(Angle*PIdiv180)
		+ m_upVector * sin(Angle*PIdiv180));

	//now compute the new UpVector (by cross product)
	m_upVector = glm::cross(m_viewDir, m_rightVector) * -1;
}

void CCamera::RotateY(TFloat Angle)
{
	m_rotation.y += Angle;
	printf("ang=%f totalangY = %f\n\n", Angle, m_rotation.y);

	//Rotate viewdir around the up vector:
	m_viewDir = glm::normalize(m_viewDir * cos(Angle * PIdiv180)
		- m_rightVector * sin(Angle * PIdiv180));

	//now compute the new RightVector (by cross product)
	m_rightVector = glm::cross(m_viewDir, m_upVector);
}

void CCamera::RotateZ(TFloat Angle)
{
	m_rotation.z += Angle;

	//Rotate viewdir around the right vector:
	m_rightVector = glm::normalize(m_rightVector * cos(Angle * PIdiv180)
		+ m_upVector * sin(Angle * PIdiv180));

	//now compute the new UpVector (by cross product)
	m_upVector = glm::cross(m_viewDir, m_rightVector) * -1;
}

void CCamera::MoveForward(TFloat Distance)
{
	m_position = m_position + (m_viewDir*-Distance);
}

void CCamera::MoveUpward(TFloat Distance)
{
	m_position = m_position + (m_upVector*Distance);
}

void CCamera::MoveRight(TFloat Distance)
{
	m_position = m_position + (m_rightVector*Distance);
}

void CCamera::HoverForward(TFloat Distance)
{
	glm::vec3 direction = m_position + (m_viewDir * Distance);
	m_position = glm::vec3(direction.x, m_position.y, direction.z);
}

void CCamera::HoverRight(TFloat Distance)
{
	glm::vec3 direction = m_position + (m_rightVector * Distance);
	m_position = glm::vec3(direction.x, m_position.y, direction.z);
}

void CCamera::SetCameraAttribute(CameraAttributeType attr, TFloat x, TFloat y, TFloat z)
{
	switch (attr)
	{
	case CameraAttributeType::CameraAttribute_Forward:
		m_viewDir.x = x;
		m_viewDir.y = y;
		m_viewDir.z = z;
		break;
	case CameraAttributeType::CameraAttribute_Up:
		m_upVector.x = x;
		m_upVector.y = y;
		m_upVector.z = z;
		break;
	case CameraAttributeType::CameraAttribute_Right:
		m_rightVector.x = x;
		m_rightVector.y = y;
		m_rightVector.z = z;
		break;
	case CameraAttributeType::CameraAttribute_Position:
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
		break;
	}
}

void CCamera::SetCameraAttribute(CameraAttributeType attr, glm::vec3 xyz)
{
	switch (attr)
	{
	case CameraAttributeType::CameraAttribute_Forward:
		m_viewDir = xyz;
		break;
	case CameraAttributeType::CameraAttribute_Up:
		m_upVector = xyz;
		break;
	case CameraAttributeType::CameraAttribute_Right:
		m_rightVector = xyz;
		break;
	case CameraAttributeType::CameraAttribute_Position:
		m_position = xyz;
		break;
	}
}
