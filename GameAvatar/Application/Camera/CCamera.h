#ifndef _CCAMERA_H_
#define _CCAMERA_H_

#include "CCommonTypes.h"

using namespace Types;

class CCamera {
public:
	// regular ctor - just initialize m_data
	CCamera();

	void prepareProjection2D();

	void prepareProjection3D();

	void SetLookAtMatrix();

	void RotateX(Float Angle);

	void RotateY(Float Angle);

	void RotateZ(Float Angle);

	void MoveForward(Float Distance);

	void MoveUpward(Float Distance);

	void MoveRight(Float Distance);
	
	void HoverForward(Float Distance);

	void HoverRight(Float Distance);

	void SetCameraAttribute(CameraAttributeType attr, Float x, Float y, Float z);

	void SetCameraAttribute(CameraAttributeType attr, glm::vec3 xyz);

	glm::vec3 GetCameraAttribute(CameraAttributeType attr);

private:
	// view/forward vector
	glm::vec3 m_viewDir;
	// right vector
	glm::vec3 m_rightVector;
	// upright vector
	glm::vec3 m_upVector;
	// physical location of camera in carthesian plan
	glm::vec3 m_position;
	// rotation angle accumulated on xyz
	glm::vec3 m_rotation;
};

inline glm::vec3 
CCamera::GetCameraAttribute(CameraAttributeType attr)
{
	return m_position;
}

#endif //_CCAMERA_H_
