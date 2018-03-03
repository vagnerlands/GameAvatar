#ifndef _CTERRAINDATABASELOADER_H_
#define _CTERRAINDATABASELOADER_H_

#include <string>
#include "CCommonTypes.h"
#include "CPngLoader.h"
#include <stack>

using namespace std;
using namespace Types;

class CTerrainDatabaseLoader {
public:
	// regular ctor -just initialize m_data
	CTerrainDatabaseLoader();

	~CTerrainDatabaseLoader();

	/**
	* Creates a terrain database loader object
	* this object is able to load terrain according to a given virtual coordinate
	* the size of the loaded area is configurable, but always consider something sligtly 
	* bigger than the display view
	* @param terrainDatabaseSource path to the terrain database location
	* @param bufferSize total size of terrain elevation area around a given coordinate
	*/
	void Initialize(string terrainDatabaseSource, Int32 loadRadius);

	void SetCoordinates(const CCoordinates& coordinates);

	void Update();

	const CMbr& GetAreaOfCoverage() const;

	string GetName() const
	{
		return "Terrain Database Loader";
	}

	Int16** GetTerrain();

	Int16 GetElevationAtLocation(CCoordinates location) ;

	UInt32 GetAreaOfConcernSize() const;

	// factor to be applied on the terrain elevation
	static Int32 s_HEIGHT_FACTOR;

	// factor to be applied on the terrain elevation
	static Int32 s_TRIGGER_LOAD_WHEN_X_PERCENT_REACHED;

private:
	/* Loads a requested area and updates the origin point */
	void loadRequestedArea(const CCoordinates& loadingCoordinates);

	// buffer identification
	enum ETerrainDatabaseLoaderBufferId
	{
		ETerrainBuffer_First,
		ETerrainBuffer_Second,
		ETerrainBuffer_Max,
	};

	// png handler pointing to a heightmap (gray scale file where each pixel represents an elevation)
	CPngLoader m_terrainLoaderHandler;
	// double buffered matrix 2x2 containing elevation points 
	Int16** m_terrainBuffer[ETerrainBuffer_Max];
	// buffer size dynamically assigned by user
	UInt32 m_areaOfConcernSize;
	// which buffer is being used currently for displaying
	ETerrainDatabaseLoaderBufferId m_displayBuffer;
	// which buffer is being used currently for loading
	ETerrainDatabaseLoaderBufferId m_loadingBuffer;
	// coordinate given by user - usually the eye of the player position
	CCoordinates m_coordinatesCenterPoint;
	// calculated based on loading radius and center point
	CCoordinates m_originPoint;
	// requests a loading - first item will be retrieved and processed
	// all others are going to be discarded
	stack<CCoordinates> m_loadingRequest;

	// area of coverage
	CMbr m_areaOfCoverage;
};

inline Int16** 
CTerrainDatabaseLoader::GetTerrain()
{
	return m_terrainBuffer[m_displayBuffer];
}

inline const CMbr& 
CTerrainDatabaseLoader::GetAreaOfCoverage() const
{
	return m_areaOfCoverage;
}

inline UInt32 
CTerrainDatabaseLoader::GetAreaOfConcernSize() const
{
	return m_areaOfConcernSize;
}


#endif //_CTERRAINDATABASELOADER_H_
