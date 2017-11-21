#ifndef _CVIEWELEMENTHUD_H_
#define _CVIEWELEMENTHUD_H_

#include "IViewElement2D.h"

class CViewElementHUD : public IViewElement2D
{
public:

	CViewElementHUD(Types::CPoint m_start, Types::CPoint m_end, string hudName);

	virtual ~CViewElementHUD();

	virtual void VPreRender();

	virtual void VRender();

	virtual void VPostRender();

	virtual void VRenderBitmap(TFloat x, TFloat y, TInt32 fontSize, TInt32 maxWidth, TInt32 maxHeight, void *font, const TByte *string);

	// debug purposes
	virtual char* GetName()
	{
		return "HUD";
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

	string m_msgList;
};

#endif //_CVIEWELEMENTMODEL_H_
