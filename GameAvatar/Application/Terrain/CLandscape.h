#ifndef _CLANDSCAPE_H_
#define _CLANDSCAPE_H_

#include <string>
#include <list>
#include "CCommonTypes.h"
#include "CTerrainDatabaseLoader.h"

class CTerrainMesh;
class CCamera;

using namespace std;
using namespace Types;

class CLandscape {
public:
	// regular ctor - just initialize m_data
	CLandscape(const UInt32 levelOfDetailBandwidth, const UInt32 tileResolution);

	string GetName() const
	{
		return "Landscape";
	}

	void Initialize();
	void Update(const CCamera& cameraView);
	void SetCameraLocation(const glm::vec3& cameraLoc);
	UInt32 GetDisplaySize() const;
	// debug function - to be removed
	Int16** GetTerrain();
	const glm::vec3* GetTerrainMeshVertexes(Int32 indX, Int32 indY) const;
	const glm::vec3* GetTerrainMeshNormals(Int32 indX, Int32 indY) const;
	const glm::vec2* GetTerrainMeshTextureCoordinates(Int32 indX, Int32 indY) const;

private:

	// performs all tasks related to terrain creation
	CTerrainDatabaseLoader m_terrainLoader;
	// object of terrain mesh
	// TerrainMesh will prepare vertexes for the terrain
	// considering the current loaded area
	// It should prepare once per loading
	CTerrainMesh* m_pTerrainMesh;


};


#endif //_CLANDSCAPE_H_
