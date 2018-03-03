#include "CGameController.h"

CGameController::CGameController()
{
	for (Int32 i = 0; i < 256; i++)
	{
		m_bKey[i] = false;
		m_bKeyStatus[i] = false;
	}
}

CGameController::~CGameController()
{
	// empty implementation
}

void 
CGameController::OnUpdate()
{
	for (Int32 cAsciiId = 0; cAsciiId < 256; cAsciiId++)
	{
		if (m_bKey[cAsciiId])
		{
			// notify that key "c" was pressed
			CUserInputEventManager::instance()->NotifyEvent(static_cast<Byte>(cAsciiId), EKeyStatus::KeyStatus_Pressed);
		} 
		else if ((!m_bKey[cAsciiId]) && (m_bKeyStatus[cAsciiId]))
		{
			CUserInputEventManager::instance()->NotifyEvent(static_cast<Byte>(cAsciiId), EKeyStatus::KeyStatus_Released);
		}
		m_bKeyStatus[cAsciiId] = m_bKey[cAsciiId];
	}
}

bool 
CGameController::VOnMouseMove(const CPoint &mousePos)
{
	if (m_lastPos != mousePos)
	{
		// mouse coordinate difference between one cycle and another
		float deltaX = 0;
		float deltaY = 0;

		deltaX = m_lastPos.x - mousePos.x;
		deltaY = m_lastPos.y - mousePos.y;

		float angle = (int)((atan2(deltaY, deltaX) * 180.0F / 3.14F) + 270) % 360;
		// intensity of the movement (how many pixels the cursor has moved from one cycle to another)
		m_movementIntensityX = deltaX;
		m_movementIntensityY = deltaY;
		// angle of the cursor movement
		m_movementAngle = angle;
		// distance from screen center
		m_distanceFromCenter = sqrt(pow(s_SCREEN_CENTER_X - mousePos.x, 2.0) + pow(s_SCREEN_CENTER_Y - mousePos.y, 2.0));
		// angle from center of the screen
		m_angleFromCenter = (Int32)((atan2(mousePos.y - s_SCREEN_CENTER_Y, mousePos.x - s_SCREEN_CENTER_X) * 180.0f / 3.14F) + 450.0F) % 360;

		/*printf("angle=%d, distCenter = %d, angleCenter = %d\n",
			m_movementAngle,
			m_distanceFromCenter,
			m_angleFromCenter);*/

		m_lastPos = mousePos;
	}

	return true;
}
