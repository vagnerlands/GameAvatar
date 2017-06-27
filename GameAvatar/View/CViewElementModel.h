#ifndef _CVIEWELEMENTMODEL_H_
#define _CVIEWELEMENTMODEL_H_

#include "IViewElement.h"

class CViewElementModel : public IViewElement
{
public:

	CViewElementModel(TFloat posX, TFloat posY, TFloat width, TFloat height, TFloat volume, string modelName);

	virtual ~CViewElementModel();

	virtual void VPreRender();

	virtual void VRender();

	// debug purposes
	virtual char* GetName()
	{
		return "Model";
	}
	virtual GameViewElement VGetType()
	{
		return GameViewElement_Model;
	}
protected:
	virtual void applyTexture(string textId);

	virtual bool loadModel(string modelId);

	bool loadShader(string shaderId);

	SModelData m_data;

	string m_modelId;
};

#endif //_CVIEWELEMENTMODEL_H_