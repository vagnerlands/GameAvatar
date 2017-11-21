#ifndef _CVIEWELEMENTMODEL_H_
#define _CVIEWELEMENTMODEL_H_

#include "IViewElement.h"

class CViewElementModel : public IViewElement
{
public:

	CViewElementModel(TFloat posX, TFloat posY, TFloat posZ, TFloat width, TFloat height, TFloat volume, string modelName, Types::EDrawDirective drawDirective = Types::DrawDirective_Triangles);

	virtual ~CViewElementModel();

	virtual void VPreRender();

	virtual void VRender();

	virtual void VPostRender();

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

	virtual bool loadShader(string shaderId);

	SModelData m_data;

	string m_modelId;

	cwc::glShader* m_pProgramShader;

	Types::EDrawDirective m_drawDirective;
};

#endif //_CVIEWELEMENTMODEL_H_
