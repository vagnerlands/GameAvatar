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

void CCamera::prepareProjection3D()
{
	// prepares PERSPECTIVE PROJECTION
	glMatrixMode(GL_PROJECTION);
	//Reset the drawing perspective
	glLoadIdentity(); 
	static GLfloat frustumParams[] = { -1.f, 1.f, -1.f, 1.f, 5.f, 10000.f };
	// prepares perspective projection
	glFrustum(frustumParams[0],
		frustumParams[1],
		frustumParams[2],
		frustumParams[3],
		frustumParams[4],
		frustumParams[5]);
}

void CCamera::prepareProjection2D()
{
	glMatrixMode(GL_MODELVIEW);
	// prepares ORTHO PROJECTION
	glMatrixMode(GL_PROJECTION);
	//Reset the drawing perspective
	glLoadIdentity(); 
	// prepares orthogonal projection
	glOrtho(-(s_SCREEN_WIDTH / 2.0), (s_SCREEN_WIDTH / 2.0), -(s_SCREEN_HEIGHT / 2.0), (s_SCREEN_HEIGHT / 2.0), -600.0, 600.0);
}

void CCamera::SetLookAtMatrix()
{
	glm::vec3 viewPoint = m_position + m_viewDir;

	gluLookAt(m_position.x, m_position.y, m_position.z, viewPoint.x, viewPoint.y, viewPoint.z, m_upVector.x, m_upVector.y, m_upVector.z);
}

void CCamera::RotateX(Float Angle)
{
	m_rotation.x += Angle;

	printf("ang=%f totalangX = %f - ", Angle, m_rotation.x);

	//Rotate viewdir around the right vector:
	m_viewDir = glm::normalize(m_viewDir * cos(Angle*PIdiv180)
		+ m_upVector * sin(Angle*PIdiv180));

	//now compute the new UpVector (by cross product)
	m_upVector = glm::cross(m_viewDir, m_rightVector) * -1;
}

void CCamera::RotateY(Float Angle)
{
	m_rotation.y += Angle;
	printf("ang=%f totalangY = %f\n\n", Angle, m_rotation.y);

	//Rotate viewdir around the up vector:
	m_viewDir = glm::normalize(m_viewDir * cos(Angle * PIdiv180)
		- m_rightVector * sin(Angle * PIdiv180));

	//now compute the new RightVector (by cross product)
	m_rightVector = glm::cross(m_viewDir, m_upVector);
}

void CCamera::RotateZ(Float Angle)
{
	m_rotation.z += Angle;

	//Rotate viewdir around the right vector:
	m_rightVector = glm::normalize(m_rightVector * cos(Angle * PIdiv180)
		+ m_upVector * sin(Angle * PIdiv180));

	//now compute the new UpVector (by cross product)
	m_upVector = glm::cross(m_viewDir, m_rightVector) * -1;
}

void CCamera::MoveForward(Float Distance)
{
	m_position = m_position + (m_viewDir*-Distance);
}

void CCamera::MoveUpward(Float Distance)
{
	m_position = m_position + (m_upVector*Distance);
}

void CCamera::MoveRight(Float Distance)
{
	m_position = m_position + (m_rightVector*Distance);
}

void CCamera::HoverForward(Float Distance)
{
	glm::vec3 direction = m_position + (m_viewDir * Distance);
	m_position = glm::vec3(direction.x, m_position.y, direction.z);
}

void CCamera::HoverRight(Float Distance)
{
	glm::vec3 direction = m_position + (m_rightVector * Distance);
	m_position = glm::vec3(direction.x, m_position.y, direction.z);
}

void CCamera::SetCameraAttribute(CameraAttributeType attr, Float x, Float y, Float z)
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
