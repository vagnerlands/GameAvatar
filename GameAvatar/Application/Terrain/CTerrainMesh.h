#ifndef _CTERRAINMESH_H
#define _CTERRAINMESH_H

#include "CCommonTypes.h"

using namespace std;
using namespace Types;

class CTerrainDatabaseLoader;
class CCamera;

class CTerrainMesh {
public:
	// regular ctor - just initialize m_data
	CTerrainMesh(CTerrainDatabaseLoader* pTerrainDbLoader, const UInt32 levelOfDetailBandwidth, const UInt32 tileResolution, const UInt32 displayAreaSize);

	void Update(const CCamera& cameraView);

	string GetName() const
	{
		return "TerrainMesh";
	}

	UInt32 GetDisplayAreaSize() const;

	//*****//
	//DEBUG//
	// size = 3 * 3 * 100 * 100 * 2
	glm::vec3 m_rawVertexes[2][500][500][4];
	glm::vec3 m_rawNormals[2][500][500][2];
	glm::vec2 m_rawTexCoords[2][500][500][4];
	Int32 m_bufferId;
	CCoordinates m_lastPreparedLocation;
	//*****//


private:
	// empty constructor to avoid copying it
	CTerrainMesh();
	// non editable terrain loader
	CTerrainDatabaseLoader& m_terrainDbLoader;
	
	// tiles after preparation and ready for display
	// a tile may be disposed in case:
	// 1. no longer is the area of concern
	// 2. farther than view range
	// 3. changed LoD (in this case, it will be removed from this list and added to the "tilesToPrepare")
	std::list<shared_ptr<STileInfo>> m_tilesInView;
	// tiles shall be inserted here first, once they are ready to present, they move to the list "tilesInView"
	std::list<shared_ptr<STileInfo>> m_tilesToPrepare;
	// Level of details table
	Types::SLoDInfo m_levels;
	// tile resolution, shall be a 2^x, i.e, 2, 4, 8, 16, 32, 64, ...
	UInt32 m_tileSize;
	// MBR of the last prepared Mesh - technically, we only have to prepare again
	// when a new area is ready.
	// For this, we must not prepare the entire area, but only the delta - taking into account
	// the already calculated areas
	CMbr m_lastPreparedMeshArea;

	// Display area size
	UInt32 m_displayAreaSize;

};

inline UInt32 
CTerrainMesh::GetDisplayAreaSize() const
{
	return m_displayAreaSize;
}


#endif //_CTERRAINMESH_H
