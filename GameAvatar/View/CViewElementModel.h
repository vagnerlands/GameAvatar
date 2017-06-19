#ifndef _CVIEWELEMENTMODEL_H_
#define _CVIEWELEMENTMODEL_H_

#include "IViewElement.h"

class CViewElementModel : public IViewElement
{
public:

	CViewElementModel(TFloat posX, TFloat posY, TFloat width, TFloat height, TFloat volume);

	virtual ~CViewElementModel();

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

#endif //_CVIEWELEMENTMODEL_H_
