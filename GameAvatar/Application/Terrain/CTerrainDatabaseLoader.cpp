#include "CTerrainDatabaseLoader.h"

Int32 CTerrainDatabaseLoader::s_HEIGHT_FACTOR = 5;
Int32 CTerrainDatabaseLoader::s_TRIGGER_LOAD_WHEN_X_PERCENT_REACHED = 10;

// LOADING PROCEDURE
// 1. FIND AREA OF CONCERN
// 2. CHECK AGAINST CURRENT LOADED AREA
// 3. LOADS NEW CONTENT (NEW AREA)
// 4. COPY OLD CONTENT TO NEW BUFFER (ACCORDING TO BUFFER BOUNDARIES)
// 5. UPDATE CURRENT AREA OF CONCERN
// 6. SWAP BUFFER

CTerrainDatabaseLoader::CTerrainDatabaseLoader() : 
	m_displayBuffer(ETerrainBuffer_First),
	m_loadingBuffer(ETerrainBuffer_Second),
	m_originPoint(CCoordinates(-1000, -1000))
{

}

void
CTerrainDatabaseLoader::Initialize(string terrainDatabaseSource, Int32 loadRadius)
{
	// set the given loading radius
	m_areaOfConcernSize = loadRadius;
	// loads a png file to this handler
	m_terrainLoaderHandler.Load(terrainDatabaseSource.c_str());


	// for each terrain buffer - allocate elevation terrain
	for (UInt32 bufferId = ETerrainBuffer_First; bufferId < ETerrainBuffer_Max; ++bufferId)
	{
		// allocate columns
		m_terrainBuffer[bufferId] = new UInt16*[m_areaOfConcernSize];
		for (UInt32 rows = 0; rows < m_areaOfConcernSize; ++rows)
		{
			m_terrainBuffer[bufferId][rows] = new UInt16[m_areaOfConcernSize];
		}
	}
}

void
CTerrainDatabaseLoader::SetCoordinates(const CCoordinates& coordinates)
{
	// half radius, to find the origin point
	const Float halfRadius = (Float)(m_areaOfConcernSize / 2);
	// shall trigger a new loading request once a certain hysteresis is passed
	const Float hysteresis = (Float)(m_areaOfConcernSize / s_TRIGGER_LOAD_WHEN_X_PERCENT_REACHED);
	// given coordinates
	m_coordinatesCenterPoint = coordinates;
	// if modular distance from the current center point is greater than x% from the 
	// origin point, this shall trigger a loading process
	if ((abs(m_coordinatesCenterPoint.m_latitude - m_originPoint.m_latitude) > hysteresis)
		|| (abs(m_coordinatesCenterPoint.m_longitude - m_originPoint.m_longitude) > hysteresis))
	{
		// adds a new request with a new origin point
		m_loadingRequest.push(CCoordinates(m_coordinatesCenterPoint.m_latitude - halfRadius, m_coordinatesCenterPoint.m_longitude - halfRadius));
	}
}

void 
CTerrainDatabaseLoader::Update()
{
	// is there any pending requests for loading
	if (!m_loadingRequest.empty())
	{
		// takes the top item from the stack
		CCoordinates loadingCoordinates = m_loadingRequest.top();
		// immediatelly clears the stack - this will destroy all copies 
		m_loadingRequest = stack<CCoordinates>();

		if (loadingCoordinates.m_latitude < 0) 
			loadingCoordinates.m_latitude = 0;
		if (loadingCoordinates.m_longitude < 0) 
			loadingCoordinates.m_longitude = 0;
		// starts loading process
		loadRequestedArea(loadingCoordinates);
	}
}

void
CTerrainDatabaseLoader::loadRequestedArea(const CCoordinates& loadingCoordinates)
{
	// retrieve database handler
	CPngLoader::SPngFile* terrainData = m_terrainLoaderHandler.GetFileContent();
	// checks if data is okay
	if (terrainData->status)
	{
		// retrieve data boundaries
		Int32 rows = terrainData->height;
		Int32 cols = terrainData->width;

		// finds intersection between the current loaded area and the new area
		// whatever is in, shall be copied, what is out, shall be loaded if within the 
		// boundaries
		for (Int32 __c = 0; __c < m_areaOfConcernSize; ++__c)
		{
			for (Int32 __r = 0; __r < m_areaOfConcernSize; ++__r)
			{
				// if this point intersects with old portion, then we copy the same value
				if ((__r + m_originPoint.m_latitude >= loadingCoordinates.m_latitude) && (__r + m_originPoint.m_latitude <= loadingCoordinates.m_latitude + m_areaOfConcernSize)
					&& (__c + m_originPoint.m_longitude >= loadingCoordinates.m_longitude) && (__c + m_originPoint.m_longitude <= loadingCoordinates.m_longitude + m_areaOfConcernSize))
				{
					m_terrainBuffer[m_loadingBuffer][__c][__r] = m_terrainBuffer[m_displayBuffer][__c][__r];
				}
				else
				{
					// otherwise, we retrieve from this other resource
					// take in consideration that now this is "in memory"
					// but this data could be anywhere, could be dependent for a disk load or sql request
					m_terrainBuffer[m_loadingBuffer][__c][__r] = (Int16)(terrainData->pPixels[__r][__c * 4] & 0x000000ff) / s_HEIGHT_FACTOR;
				}
			}
		}

		// CRITICAL SECTION - shall be protected by mutex!

		// swap buffer
		m_loadingBuffer = m_displayBuffer;
		m_displayBuffer = (ETerrainBuffer_First == m_displayBuffer ? ETerrainBuffer_Second : ETerrainBuffer_First);
		// update origin point
		m_originPoint = loadingCoordinates;

		// END OF CRITICAL SECTION
	}
}



CTerrainDatabaseLoader::~CTerrainDatabaseLoader()
{
	// deallocation in the opposite order of the allocation
	for (UInt32 bufferId = ETerrainBuffer_First; bufferId < ETerrainBuffer_Max; ++bufferId)
	{
		if (m_terrainBuffer[bufferId] != nullptr)
		{
			for (UInt32 rows = 0; rows < m_areaOfConcernSize; ++rows)
			{
				delete[] m_terrainBuffer[bufferId][rows];
				m_terrainBuffer[bufferId][rows] = nullptr;
			}
			delete[] m_terrainBuffer[bufferId];
			m_terrainBuffer[bufferId] = nullptr;
		}
	}
}