#include "CHumanView.h"

CHumanView::CHumanView()
{
	// empty implementation
}

CHumanView::~CHumanView()
{
	// empty implementation
}

void CHumanView::VOnRender()
{
	for (ViewElementList::iterator it = m_elements.begin(); 
			it != m_elements.end(); 
			++it)
	{
		// should be displayed or not
		if ((*it)->VIsVisible())
		{
			// prepares the OpenGL state machine before starting to draw
			(*it)->VPreRender();
			// actually draws the shape
			(*it)->VRender();
		}
	}
}

void CHumanView::VPushElement(shared_ptr<IViewElement> pElement)
{
	m_elements.push_back(pElement);
}

void CHumanView::VPopElement(shared_ptr<IViewElement> pElement)
{
	m_elements.remove(pElement);
}
