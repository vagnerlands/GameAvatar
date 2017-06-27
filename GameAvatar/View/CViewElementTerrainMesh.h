#ifndef _CVIEWELEMENTTERRAINMESH_H_
#define _CVIEWELEMENTTERRAINMESH_H_

#include "IViewElement.h"

class CViewElementTerrainMesh : public IViewElement
{
public:

	CViewElementTerrainMesh(TFloat posX, TFloat posY, TFloat posZ, TFloat width, TFloat height, TFloat volume, string modelName);

	virtual ~CViewElementTerrainMesh();

	virtual void VPreRender();

	virtual void VRender();

	// debug purposes
	virtual char* GetName()
	{
		return "Model";
	}
	virtual GameViewElement VGetType()
	{
		return GameViewElement_TerrainMesh;
	}
protected:
	virtual void applyTexture(string textId);

	virtual bool loadModel(string modelId);

	SModelData m_data;

	string m_modelId;
};

#endif //_CVIEWELEMENTTERRAINMESH_H_
