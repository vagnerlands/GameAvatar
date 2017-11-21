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

	void RotateX(TFloat Angle);

	void RotateY(TFloat Angle);

	void RotateZ(TFloat Angle);

	void MoveForward(TFloat Distance);

	void MoveUpward(TFloat Distance);

	void MoveRight(TFloat Distance);
	
	void HoverForward(TFloat Distance);

	void HoverRight(TFloat Distance);

	void SetCameraAttribute(CameraAttributeType attr, TFloat x, TFloat y, TFloat z);

	void SetCameraAttribute(CameraAttributeType attr, glm::vec3 xyz);

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


#endif //_CCAMERA_H_
