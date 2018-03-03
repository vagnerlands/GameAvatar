#include "CTerrainMesh.h"
#include "CTerrainDatabaseLoader.h"
#include "CCamera.h"

CTerrainMesh::CTerrainMesh(CTerrainDatabaseLoader* pTerrainDbLoader, 
	const UInt32 levelOfDetailBandwidth, 
	const UInt32 tileResolution,
	const UInt32 displayAreaSize) :
	m_terrainDbLoader(*pTerrainDbLoader),
	m_tileSize(tileResolution),
	m_bufferId(0),
	m_displayAreaSize(displayAreaSize)
{
	m_levels.m_bandWidth = levelOfDetailBandwidth;
	for (Int32 _i = 0; _i < 32; ++_i)
	{
		m_levels.m_levelOfDetailTable[_i] = levelOfDetailBandwidth * _i;
	}
}

void CTerrainMesh::Update(const CCamera& cameraView)
{
	// half radius, to find the origin point
	const Float halfRadius = (Float)(m_displayAreaSize / 2.f);
	// retrieving the current camera location for terrain mesh preparation
	// only the integer part - not important the decimal to avoid nuances issues
	CCoordinates location = CCoordinates(static_cast<Int32>(cameraView.GetCameraAttribute(Types::CameraAttribute_Position).z - halfRadius), static_cast<Int32>(cameraView.GetCameraAttribute(Types::CameraAttribute_Position).x - halfRadius));
	// check if coverage area is valid - if so, proceed
	Int16 pointElevation = m_terrainDbLoader.GetElevationAtLocation(CCoordinates(location.m_latitude, location.m_longitude));

	if ((location == m_lastPreparedLocation) 
		|| (pointElevation == s_INVALID_ELEVATION))
		return;

	for (Int32 i = 0; i < m_displayAreaSize; ++i)
	{
		for (Int32 j = 0; j < m_displayAreaSize; ++j)
		{

			Int16 pointElevationA = m_terrainDbLoader.GetElevationAtLocation(CCoordinates(location.m_latitude + i, location.m_longitude + j));
			Int16 pointElevationB = m_terrainDbLoader.GetElevationAtLocation(CCoordinates(location.m_latitude + i, location.m_longitude + j + 1));
			Int16 pointElevationC = m_terrainDbLoader.GetElevationAtLocation(CCoordinates(location.m_latitude + i + 1, location.m_longitude + j));
			Int16 pointElevationD = m_terrainDbLoader.GetElevationAtLocation(CCoordinates(location.m_latitude + i + 1, location.m_longitude + j + 1));

			glm::vec3 pointA(static_cast<Int32>(location.m_longitude + j), pointElevationA, static_cast<Int32>(location.m_latitude + i));
			glm::vec3 pointB(static_cast<Int32>(location.m_longitude + j + 1), pointElevationB, static_cast<Int32>(location.m_latitude + i));
			glm::vec3 pointC(static_cast<Int32>(location.m_longitude + j), pointElevationC, static_cast<Int32>(location.m_latitude + i + 1));
			glm::vec3 pointD(static_cast<Int32>(location.m_longitude + j + 1), pointElevationD, static_cast<Int32>(location.m_latitude + i + 1));

			// so not optmized - must be calculated once
			glm::vec3 polygon1Normal = glm::normalize(glm::cross(pointB - pointA, pointC - pointA));
			glm::vec3 polygon2Normal = glm::normalize(glm::cross(pointA - pointB, pointC - pointB));
			glm::vec3 polygon3Normal = glm::normalize(glm::cross(pointA - pointC, pointB - pointC));
			//glm::vec3 polygon4Normal = glm::normalize(glm::cross(pointC - pointD, pointB - pointD));

			// ----
			m_rawVertexes[m_bufferId][i][j][0] = pointA;
			m_rawVertexes[m_bufferId][i][j][1] = pointB;
			m_rawVertexes[m_bufferId][i][j][2] = pointC;
			m_rawVertexes[m_bufferId][i][j][3] = pointD;

			m_rawNormals[m_bufferId][i][j][0] = glm::normalize(polygon1Normal + polygon2Normal + polygon3Normal);

			m_rawTexCoords[m_bufferId][i][j][0] = glm::vec2(i / 99.f, j / 99.f);
			m_rawTexCoords[m_bufferId][i][j][1] = glm::vec2(i / 99.f, (j+1) / 99.f);
			m_rawTexCoords[m_bufferId][i][j][2] = glm::vec2((i+1) / 99.f, j / 99.f);
			m_rawTexCoords[m_bufferId][i][j][3] = glm::vec2((i+1) / 99.f, (j+1) / 99.f);

		}
	}
	m_lastPreparedLocation = location;
	m_bufferId = (m_bufferId + 1) % 2;

}
