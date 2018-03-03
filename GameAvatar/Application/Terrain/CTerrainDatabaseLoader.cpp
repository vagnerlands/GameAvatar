#include "CTerrainDatabaseLoader.h"
#include <iostream>
#include <fstream>
Int32 CTerrainDatabaseLoader::s_HEIGHT_FACTOR = 35;
Int32 CTerrainDatabaseLoader::s_TRIGGER_LOAD_WHEN_X_PERCENT_REACHED = 3;

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
	m_originPoint(CCoordinates(0, 0))
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
		m_terrainBuffer[bufferId] = new Int16*[m_areaOfConcernSize];
		for (UInt32 rows = 0; rows < m_areaOfConcernSize; ++rows)
		{
			m_terrainBuffer[bufferId][rows] = new Int16[m_areaOfConcernSize];
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
	// if squared distance of the last loaded CENTER POINT by the new COORDINATES is greater than the squared
	// hysteresis distance, then triggers a new loading
	if (pow(abs(m_coordinatesCenterPoint.m_latitude - coordinates.m_latitude), 2.0) 
			+ pow(abs(m_coordinatesCenterPoint.m_longitude - coordinates.m_longitude), 2.0) 
		> pow(hysteresis, 2.0))
	{
		// adds a new request with a new origin point
		m_loadingRequest.push(CCoordinates(coordinates.m_latitude - halfRadius, coordinates.m_longitude - halfRadius));
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

		// starts loading process
		loadRequestedArea(loadingCoordinates);
	}
}

void
CTerrainDatabaseLoader::loadRequestedArea(const CCoordinates& loadingCoordinates)
{
	// half radius, to find the origin point
	const Float halfRadius = (Float)(m_areaOfConcernSize / 2);
	// retrieve database handler
	CPngLoader::SPngFile* terrainData = m_terrainLoaderHandler.GetFileContent();
	// checks if data is okay
	if (terrainData->status)
	{
		// retrieve data boundaries
		Int32 rows = terrainData->height;
		Int32 cols = terrainData->width;

		Int32 columnInitialOffset = 0;
		if (loadingCoordinates.m_longitude < 0.F)
			columnInitialOffset = abs(loadingCoordinates.m_longitude);

		Int32 rowInitialOffset = 0;
		if (loadingCoordinates.m_latitude < 0.F)
			rowInitialOffset = abs(loadingCoordinates.m_latitude);
		
		//ofstream dumpFile;
		//dumpFile.open("debug_.txt");

		cout << "Loading coords " << loadingCoordinates.m_latitude << " " << loadingCoordinates.m_longitude << endl;
		// finds intersection between the current loaded area and the new area
		// whatever is in, shall be copied, what is out, shall be loaded if within the 
		// boundaries
		for (Int32 __c = columnInitialOffset; __c < m_areaOfConcernSize; ++__c)
		{
			Int32 currentColumnCell = static_cast<Int32>(__c + m_originPoint.m_longitude);
			for (Int32 __r = rowInitialOffset; __r < m_areaOfConcernSize; ++__r)
			{
				Int32 currentRowCell = static_cast<Int32>(__r + m_originPoint.m_latitude);
				// calculate the delta offset on latitude (row)
				Int32 rowOffset = 0;
				// calculate the delta offset on longitude (row)
				Int32 columnOffset = 0;
				// if this point intersects with old portion, then we copy the same value
				// to the respective cell in the new buffer
				/*if ((currentRowCell > loadingCoordinates.m_latitude) && (currentRowCell < loadingCoordinates.m_latitude + m_areaOfConcernSize)
					&& (currentColumnCell > loadingCoordinates.m_longitude) && (currentColumnCell < loadingCoordinates.m_longitude + m_areaOfConcernSize))
				{
					// calculate the delta offset on latitude (row)
					rowOffset = static_cast<Int32>(trunc(m_originPoint.m_latitude) - trunc(loadingCoordinates.m_latitude)) + __r;
					// calculate the delta offset on longitude (row)
					columnOffset = static_cast<Int32>(trunc(m_originPoint.m_longitude) - trunc(loadingCoordinates.m_longitude)) + __c;
					// for the new buffer - copy the data considering the offset
					m_terrainBuffer[m_loadingBuffer][columnOffset][rowOffset] = m_terrainBuffer[m_displayBuffer][__c][__r];
				}
				else*/
				{
					// calculate the delta offset on latitude (row)
					rowOffset = static_cast<Int32>(loadingCoordinates.m_latitude + __r);
					// calculate the delta offset on longitude (row)
					columnOffset = static_cast<Int32>(loadingCoordinates.m_longitude + __c);
					// otherwise, we retrieve from this other resource
					// take in consideration that now this is "in memory"
					// but this data could be anywhere, could be dependent for a disk load or sql request
					m_terrainBuffer[m_loadingBuffer][__c][__r] = (Int16)(terrainData->pPixels[columnOffset][rowOffset * 4] & 0x000000ff) - s_HEIGHT_FACTOR;

					//dumpFile << "Column [" << columnOffset << "] Row [" << rowOffset << "] : " << m_terrainBuffer[m_loadingBuffer][__c][__r] << endl;
				}				
			}
			//dumpFile << endl;
		}

		// CRITICAL SECTION - shall be protected by mutex!

		// swap buffer

		//dumpFile.close();

		m_loadingBuffer = m_displayBuffer; // loading buffer
		m_displayBuffer = (ETerrainBuffer_First == m_displayBuffer ? ETerrainBuffer_Second : ETerrainBuffer_First); // display buffer
		// update origin point
		m_originPoint = loadingCoordinates;
		// calculate the current area of coverage
		m_areaOfCoverage = CMbr(m_originPoint, m_originPoint + CCoordinates(m_areaOfConcernSize, m_areaOfConcernSize));
		// update center point coordinates
		m_coordinatesCenterPoint = CCoordinates(m_originPoint.m_latitude + halfRadius, m_originPoint.m_longitude + halfRadius);
		cout << " <!> new Loading Center lat[" << m_coordinatesCenterPoint.m_latitude << "] lon[" << m_coordinatesCenterPoint.m_longitude << "]" << endl;

		// END OF CRITICAL SECTION
	}
}


Int16 
CTerrainDatabaseLoader::GetElevationAtLocation(CCoordinates location) 
{
	// this approach will use truncation to find the elevation of the terrain
	// all we need is something fast and deterministic...
	
	// set the elevation as invalid
	Int16 retVal = s_INVALID_ELEVATION;

	// checks is the current loaded terrain buffer contains
	// the given location
	if (m_areaOfCoverage.Contains(location))
	{
		// set the column value based on longitude
		Int32 indexColumn = static_cast<Int32>(abs(location.m_longitude - m_originPoint.m_longitude));
		// set the column value based on latitude
		Int32 indexRow    = static_cast<Int32>(abs(location.m_latitude - m_originPoint.m_latitude));
		// retrieve the truncated value from the buffer (at this point, no chances of memory violation)
		retVal = m_terrainBuffer[m_displayBuffer][indexColumn][indexRow];
	}

	return retVal;
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