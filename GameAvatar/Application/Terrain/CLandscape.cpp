#include "CLandscape.h"
#include "CTerrainDatabaseLoader.h"
#include "CTerrainMesh.h"
#include "CCamera.h"

CLandscape::CLandscape(const UInt32 levelOfDetailBandwidth, const UInt32 tileResolution)
{
	m_pTerrainMesh = new CTerrainMesh(&m_terrainLoader, levelOfDetailBandwidth, tileResolution, 300);
}

void
CLandscape::Initialize()
{
	m_terrainLoader.Initialize("C:/Users/Vagner/Documents/Visual Studio 2015/Projects/GameAvatar/GameAvatar/Resources/MyDatabase Height Map (ASTER 30m).png", 500);
	m_terrainLoader.SetCoordinates(CCoordinates(60, 60));
}

void
CLandscape::SetCameraLocation(const glm::vec3& cameraLoc)
{
	// Z will be latitude while X will be longitude
	m_terrainLoader.SetCoordinates(CCoordinates(cameraLoc.z, cameraLoc.x));
}

UInt32
CLandscape::GetDisplaySize() const
{
	return m_pTerrainMesh->GetDisplayAreaSize();
}

Int16** 
CLandscape::GetTerrain()
{
	return m_terrainLoader.GetTerrain();
}

// debug only
const glm::vec3*
CLandscape::GetTerrainMeshVertexes(Int32 indX, Int32 indY) const
{
	return m_pTerrainMesh->m_rawVertexes[(m_pTerrainMesh->m_bufferId + 1) % 2][indX][indY];
}
// debug only
const glm::vec3*
CLandscape::GetTerrainMeshNormals(Int32 indX, Int32 indY) const
{
	return m_pTerrainMesh->m_rawNormals[(m_pTerrainMesh->m_bufferId + 1) % 2][indX][indY];
}
const glm::vec2*
CLandscape::GetTerrainMeshTextureCoordinates(Int32 indX, Int32 indY) const
{
	return m_pTerrainMesh->m_rawTexCoords[(m_pTerrainMesh->m_bufferId + 1) % 2][indX][indY];
}

void 
CLandscape::Update(const CCamera& cameraView)
{
	// loads new data if requested
	m_terrainLoader.Update();

	// prepare mesh for the current camera view
	m_pTerrainMesh->Update(cameraView);
}
