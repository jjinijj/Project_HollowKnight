#include "stdafx.h"
#include "mapData.h"


mapData::mapData()
{
	_terrains.clear();
	_colTerrains.clear();
	
	for(int ii = 0 ; ii < MAP_LAYER_COUNT; ++ii)
		_terrainsByLayer[ii].clear();
}

mapData::~mapData()
{
}

HRESULT mapData::init()
{
	return S_OK;
}

void mapData::release()
{
	iterVTerrain iter;
	int size = 0;

	for (int ii = 0; ii < MAP_LAYER_COUNT; ++ii)
	{
		size = _terrainsByLayer[ii].size();
		for (iter = _terrainsByLayer[ii].begin(); _terrainsByLayer[ii].end() != iter; )
		{
			iter = _terrainsByLayer[ii].erase(iter);
		}

		_terrainsByLayer[ii].clear();
	}

	for (iter = _colTerrains.begin(); _colTerrains.end() != iter; )
	{
		iter = _colTerrains.erase(iter);
	}
	_colTerrains.clear();


	for (iter = _terrains.begin(); _terrains.end() != iter; )
	{
		terrain* ter = *iter;
		iter = _terrains.erase(iter);

		SAFE_RELEASE(ter);
		SAFE_DELETE(ter);
	}

	_terrains.clear();
}

void mapData::update()
{
}

void mapData::render()
{
	iterVTerrain iter;
	iterVTerrain end;
	for (int ii = 0; ii < MAP_LAYER_COUNT; ++ii)
	{
		end = _terrainsByLayer[ii].end();
		for (iter = _terrainsByLayer[ii].begin(); iter != end; ++iter)
		{
			(*iter)->render();
		}
	}
}

void mapData::swapTerrain(UINT layer, int dest, int sour)
{
	int size = _terrainsByLayer[layer].size();

	if ( size <= dest || size <= sour || dest < 0 || sour < 0)
		return;

	swap(_terrainsByLayer[layer][dest], _terrainsByLayer[layer][sour]);
}

void mapData::changeLayer(UINT destLayer, UINT sourLayer, int idx)
{
	if(_terrainsByLayer[destLayer].size() <= idx || idx < 0)
		return;

	terrain* ter = _terrainsByLayer[destLayer][idx];
	_terrainsByLayer[destLayer].erase(_terrainsByLayer[destLayer].begin() + idx);

	_terrainsByLayer[sourLayer].push_back(ter);
}


terrain* mapData::addTerrainDrag(UINT layer, float destX, float destY, float sourX, float sourY, float width, float height, eImageUID imgUid)
{
	terrainDrag* drag = new terrainDrag;
	drag->init(_uidCount, destX, destY, sourX, sourY, width, height, imgUid);

	addTerrain(layer, drag);

	return drag;
}

terrain* mapData::addTerrainFrame(UINT layer, float destX, float destY, UINT frameX, UINT frameY, eImageUID imgUid)
{
	terrainFrame* frame = new terrainFrame;
	frame->init(_uidCount, destX, destY, frameX, frameY, imgUid);

	addTerrain(layer, frame);

	return frame;
}

terrain* mapData::addTerrainClear(UINT layer, float destX, float destY, float width, float height)
{
	terrainClear* clear = new terrainClear;
	clear->init(_uidCount, destX, destY, width, height);

	addTerrain(layer, clear);

	return clear;
}

terrain* mapData::getTerrain(UINT layer, int idx)
{
	if (_terrainsByLayer[layer].size() <= idx)
		return nullptr;
	else
		_terrainsByLayer[layer][idx];
}


void mapData::addTerrain(UINT layer, terrain* ter)
{
	_terrains.push_back(ter);
	_terrainsByLayer[layer].push_back(ter);

	if (ter->checkAttribute(ATTR_COLLIDER))
		_colTerrains.push_back(ter);
}
