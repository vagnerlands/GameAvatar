#ifndef _CVIEWELEMENTTERRAINMESH_H_
#define _CVIEWELEMENTTERRAINMESH_H_

#include "IViewElement.h"

class CTerrainDatabaseLoader;
class CLandscape;

class CViewElementTerrainMesh : public IViewElement
{
public:

	CViewElementTerrainMesh(Float posX, Float posY, Float posZ, Float width, Float height, Float volume);

	virtual ~CViewElementTerrainMesh();

	virtual void VPreRender() override;

	virtual void VRender() override;

	virtual void VPostRender() override;

	// debug purposes
	virtual char* GetName() 
	{
		return "TerrainMesh";
	}
	virtual GameViewElement VGetType() override
	{
		return GameViewElement_TerrainMesh;
	}

	// used basically by Landscape displaying views
	virtual void VSetLandscapeObject(CLandscape* pLandscape)
	{
		m_pLandscape = pLandscape;
	}

protected:
	virtual void applyTexture(string textId);

	virtual bool loadShader(string shaderId);

	cwc::glShader* m_pProgramShader;
	// keeps a reference of the same object existing in CGameCockpit
	CLandscape* m_pLandscape;
	
};

#endif //_CVIEWELEMENTTERRAINMESH_H_
