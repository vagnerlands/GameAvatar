#include "CHumanView.h"
#include "CViewLightAmbient.h"
#include "CViewLightSpecular.h"
#include "CViewLightDiffuse.h"

const Float CHumanView::s_DEFAULT_MOVEMENT_DISTANCE = 0.05F;

CHumanView::CHumanView()
{
	m_lightSources.insert(make_pair("ambient", new CViewLightAmbient(0.0f, 0.0f, .16f, .16f)));
	m_lightSources.insert(make_pair("specular", new CViewLightSpecular(160.0f, 0.0f, .16f, .16f)));
	m_lightSources.insert(make_pair("diffuse", new CViewLightSpecular(0.0f, 100.0f, .16f, .16f)));

	m_userInputListenerName = "Human_View";
}

CHumanView::~CHumanView()
{
	while (!m_lightSources.empty())
	{
		// shared pointers will automatically deallocate when the last reference is erased
		m_lightSources.erase(m_lightSources.begin());
	}

	while (!m_elements.empty())
	{
		// shared pointers will automatically deallocate when the last reference is erased
		m_elements.erase(m_elements.begin());
	}
}

void
CHumanView::UpdateCameraLocation()
{
	/*if (m_pGameCtrl->m_bKey['w'])
	{
		m_pCamera->HoverForward(s_DEFAULT_MOVEMENT_DISTANCE);
	}
	else if (m_pGameCtrl->m_bKey['s'])
	{
		m_pCamera->HoverForward(-s_DEFAULT_MOVEMENT_DISTANCE);
	}	

	if (m_pGameCtrl->m_bKey['a'])
	{
		m_pCamera->HoverRight(-s_DEFAULT_MOVEMENT_DISTANCE);
	}
	else if (m_pGameCtrl->m_bKey['d'])
	{
		m_pCamera->HoverRight(s_DEFAULT_MOVEMENT_DISTANCE);
	}

	if (m_pGameCtrl->m_bKey['f'])
	{
		m_pCamera->RotateY(1.0f);
	}
	else if (m_pGameCtrl->m_bKey['g'])
	{
		m_pCamera->RotateY(-1.0f);
	}*/

	// this routine makes the map moves smoothly according to the cursor position
	// on the screen
	if (m_pGameCtrl->m_distanceFromCenter > (s_SCREEN_HEIGHT / 2))
	{
		// movement on the Z axis
		m_pCamera->HoverForward(cos(m_pGameCtrl->m_angleFromCenter * s_PI / 180.0F) * s_DEFAULT_MOVEMENT_DISTANCE);
		// movement on the X axis
		m_pCamera->HoverRight(sin(m_pGameCtrl->m_angleFromCenter * s_PI / 180.0F) * s_DEFAULT_MOVEMENT_DISTANCE);
	}
}

void 
CHumanView::UpdateScenario(const string elementId)
{
	/*
	static string currentLightSource = "specular";
	if (m_pGameCtrl->m_bKey['1']) currentLightSource = "specular";
	if (m_pGameCtrl->m_bKey['2']) currentLightSource = "diffuse";

	if (m_pGameCtrl->m_bKey['z'])
	{
		m_elements[elementId]->VScale(1.001f, 1.001f, 1.001f);
	}
	else if (m_pGameCtrl->m_bKey['x'])
	{
		m_elements[elementId]->VScale(0.999f, 0.999f, 0.999f);
	}


	if (m_pGameCtrl->m_bKey['q'])
	{
		m_elements[elementId]->VTranslateY(-0.5f);
	}
	else if (m_pGameCtrl->m_bKey['e'])
	{
		m_elements[elementId]->VTranslateY(0.5f);
	}
	*/
	/*
	if ((m_pGameCtrl->m_bKey['o']))
	{
		m_lightSources[currentLightSource]->VTranslateZ(0.5f);
	} 
	if ((m_pGameCtrl->m_bKey['l']))
	{
		m_lightSources[currentLightSource]->VTranslateZ(-0.5f);
	} 
	if ((m_pGameCtrl->m_bKey['k']))
	{
		m_lightSources[currentLightSource]->VTranslateX(-0.5f);
	}
	if ((m_pGameCtrl->m_bKey[';']))
	{
		m_lightSources[currentLightSource]->VTranslateX(0.5f);
	}
	if ((m_pGameCtrl->m_bKey['i']))
	{
		m_lightSources[currentLightSource]->VTranslateY(-0.05f);
	}
	if ((m_pGameCtrl->m_bKey['p']))
	{
		m_lightSources[currentLightSource]->VTranslateY(0.05f);
	}
	if ((m_pGameCtrl->m_bKey['.']))
	{
		m_lightSources[currentLightSource]->VSetVisible(false);
	}
	if ((m_pGameCtrl->m_bKey[',']))
	{
		m_lightSources[currentLightSource]->VSetVisible(true);
	}*/
	/*
	if ((m_pGameCtrl->m_bKey['t']))
	{
		lightPos1[2] += 0.1f;
	}
	else if ((m_pGameCtrl->m_bKey['g']))
	{
		lightPos1[2] -= 0.1f;
	}
	else if ((m_pGameCtrl->m_bKey['f']))
	{
		lightPos1[0] += 0.1f;
	}
	else if ((m_pGameCtrl->m_bKey['h']))
	{
		lightPos1[0] -= 0.1f;
	}
	*/

}

void CHumanView::VOnRender()
{
	// updates the camera location according to input and current view rules
	//UpdateCameraLocation();
	// render all light sources first
	for (ViewLightMap::iterator it = m_lightSources.begin();
		it != m_lightSources.end();
		++it)
	{
		// should be displayed or not
		if ((*it).second->VIsVisible())
		{
			(*it).second->VPreRender();
			// actually draws the shape
			(*it).second->VRender();

			(*it).second->VPostRender();
		}
	}
	
	for (ViewElementMap::iterator it = m_elements.begin(); 
			it != m_elements.end(); 
			++it)
	{
		// should be displayed or not
		if ((*it).second->VIsVisible())
		{
			// prepares the OpenGL state machine before starting to draw
			(*it).second->VPreRender();
			// update scenario according to user input
			//UpdateScenario((*it).first);
			// actually draws the shape
			(*it).second->VRender();

			(*it).second->VPostRender();
		}
	}
}

void CHumanView::VPushElement(const string elementId, shared_ptr<IViewElement> pElement)
{
	// inserts the item without checking for duplication
	m_elements.insert(make_pair(elementId, pElement));
}

void CHumanView::VPopElement(const string elementId, shared_ptr<IViewElement> pElement)
{
	// check if item doesn't exist
	if (m_elements.find(elementId) == m_elements.end())
	{
		// then quit function (to avoid exception)
		return;
	}
	// if item really exists - then removes it from the DB
	m_elements.erase(elementId);
}

void 
CHumanView::VProcessUserInput(Byte keyId, Int32 keyStatus)
{
	if (keyStatus == EKeyStatus::KeyStatus_Pressed)
	{
		static string currentLightSource = "specular";
		static string elementId = "terrain";
		if (m_pGameCtrl->m_bKey['1']) currentLightSource = "specular";
		if (m_pGameCtrl->m_bKey['2']) currentLightSource = "diffuse";

		switch (keyId)
		{
		case 'w':
			m_pCamera->HoverForward(s_DEFAULT_MOVEMENT_DISTANCE);
			break;
		case 's':
			m_pCamera->HoverForward(-s_DEFAULT_MOVEMENT_DISTANCE);
			break;
		case 'a':
			m_pCamera->HoverRight(-s_DEFAULT_MOVEMENT_DISTANCE);
			break;
		case 'd':
			m_pCamera->HoverRight(s_DEFAULT_MOVEMENT_DISTANCE);
			break;
		case 'f':
			m_pCamera->RotateY(1.0f);
			break;
		case 'g':
			m_pCamera->RotateY(-1.0f);
			break;
		case 'o':
			m_lightSources[currentLightSource]->VTranslateZ(0.5f);
			break;
		case 'l':
			m_lightSources[currentLightSource]->VTranslateZ(-0.5f);
			break;
		case 'k':
			m_lightSources[currentLightSource]->VTranslateX(-0.5f);
			break;
		case ';':
			m_lightSources[currentLightSource]->VTranslateX(0.5f);
			break;
		case 'i':
			m_lightSources[currentLightSource]->VTranslateY(-0.05f);
			break;
		case 'p':
			m_lightSources[currentLightSource]->VTranslateY(0.05f);
			break;
		case '.':
			m_lightSources[currentLightSource]->VSetVisible(false);
			break;
		case ',':
			m_lightSources[currentLightSource]->VSetVisible(true);
			break;
		case 'z':
			m_elements[elementId]->VScale(1.001f, 1.001f, 1.001f);
			break;
		case 'x':
			m_elements[elementId]->VScale(0.999f, 0.999f, 0.999f);
			break;
		case 'q':
			m_elements[elementId]->VTranslateY(-0.5f);
			break;
		case 'e':
			m_elements[elementId]->VTranslateY(0.5f);
			break;
		}
	}
}
