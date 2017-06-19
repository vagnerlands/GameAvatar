#ifndef _CVIEWELEMENTCUBE_H_
#define _CVIEWELEMENTCUBE_H_

#include "IViewElement.h"

class CViewElementCube : public IViewElement
{
public:

	CViewElementCube(TInt32 posX, TInt32 posY, TInt32 width, TInt32 height);

	virtual ~CViewElementCube();

	virtual void VPreRender();

	virtual void VRender();

	// debug purposes
	virtual char* GetName()
	{
		return "Cube";
	}
	virtual GameViewElement VGetType()
	{
		return GameViewElement_Cube;
	}
protected:
	virtual void applyTexture(string textId);

	virtual void loadModel(string modelId);

	SModelData m_data;
};

#endif //_CVIEWELEMENTCUBE_H_
