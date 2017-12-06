#ifndef _CLANDSCAPE_H_
#define _CLANDSCAPE_H_

#include <string>
#include <list>
#include "CCommonTypes.h"

using namespace std;
using namespace Types;

class CTerrainDatabaseLoader;

class CLandscape {
public:
	// regular ctor - just initialize m_data
	CLandscape();

	string getName() const
	{
		return "Landscape";
	}

public:
	void SetLevelOfDetailBandWidth(Float levelOfDetailBandWidth);
	void SetTileResolution(UInt32 tileResolution);

private:
	// will hold information 
	CTerrainDatabaseLoader* m_terrainDatabaseLoader;
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
	// tile resolution, shall be a x^2
	UInt32 m_tileSize;


};

inline void 
CLandscape::SetLevelOfDetailBandWidth(Float levelOfDetailBandWidth)
{
	m_levels.m_bandWidth = levelOfDetailBandWidth;
}

inline void
CLandscape::SetTileResolution(UInt32 tileResolution)
{
	m_tileSize = tileResolution;
}

#endif //_CLANDSCAPE_H_
