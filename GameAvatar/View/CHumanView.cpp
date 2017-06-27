#include "CHumanView.h"
#include "CViewLightAmbient.h"
#include "CViewLightSpecular.h"
#include "CViewLightDiffuse.h"

CHumanView::CHumanView()
{
	m_lightSources.insert(make_pair("ambient", new CViewLightAmbient(0.0f, 0.0f, .16f, .16f)));
	m_lightSources.insert(make_pair("specular", new CViewLightSpecular(0.0f, 0.0f, .16f, .16f)));
	m_lightSources.insert(make_pair("diffuse", new CViewLightSpecular(0.0f, 0.0f, .16f, .16f)));
}

CHumanView::~CHumanView()
{
	// empty implementation
}

void 
CHumanView::UpdateScenario(const string elementId)
{
	static string currentLightSource = "specular";
	if (m_pGameCtrl->m_bKey['1']) currentLightSource = "specular";
	if (m_pGameCtrl->m_bKey['2']) currentLightSource = "diffuse";

	if (m_pGameCtrl->m_bKey['w'])
	{
		m_elements[elementId]->VTranslateZ(0.005f);
	} 
	else if (m_pGameCtrl->m_bKey['s'])
	{
		m_elements[elementId]->VTranslateZ(-0.005f);
	}

	if (m_pGameCtrl->m_bKey['z'])
	{
		m_elements[elementId]->VScale(1.001f, 1.001f, 1.001f);
	}
	else if (m_pGameCtrl->m_bKey['x'])
	{
		m_elements[elementId]->VScale(0.999f, 0.999f, 0.999f);
	}

	if (m_pGameCtrl->m_bKey['a'])
	{
		m_elements[elementId]->VTranslateX(0.005f);
	}
	else if (m_pGameCtrl->m_bKey['d'])
	{
		m_elements[elementId]->VTranslateX(-0.005f);
	}

	if (m_pGameCtrl->m_bKey['f'])
	{
		m_elements[elementId]->VRotateY(0.05f);
	}
	else if (m_pGameCtrl->m_bKey['g'])
	{
		m_elements[elementId]->VRotateY(-0.05f);
	}

	if (m_pGameCtrl->m_bKey['q'])
	{
		m_elements[elementId]->VTranslateY(0.005f);
	}
	else if (m_pGameCtrl->m_bKey['e'])
	{
		m_elements[elementId]->VTranslateY(-0.005f);
	}

	if ((m_pGameCtrl->m_bKey['o']))
	{
		m_lightSources[currentLightSource]->VTranslateZ(0.005f);
	} else if ((m_pGameCtrl->m_bKey['l']))
	{
		m_lightSources[currentLightSource]->VTranslateZ(-0.005f);
	} else if ((m_pGameCtrl->m_bKey['k']))
	{
		m_lightSources[currentLightSource]->VTranslateX(0.005f);
	}
	else if ((m_pGameCtrl->m_bKey[';']))
	{
		m_lightSources[currentLightSource]->VTranslateX(-0.005f);
	}
	else if ((m_pGameCtrl->m_bKey['i']))
	{
		m_lightSources[currentLightSource]->VTranslateY(0.005f);
	}
	else if ((m_pGameCtrl->m_bKey['p']))
	{
		m_lightSources[currentLightSource]->VTranslateY(-0.005f);
	}
	else if ((m_pGameCtrl->m_bKey['.']))
	{
		m_lightSources[currentLightSource]->VSetVisible(false);
	}
	else if ((m_pGameCtrl->m_bKey[',']))
	{
		m_lightSources[currentLightSource]->VSetVisible(true);
	}
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
			UpdateScenario((*it).first);
			// actually draws the shape
			(*it).second->VRender();
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
