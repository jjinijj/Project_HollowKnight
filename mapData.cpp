#include "stdafx.h"
#include "mapData.h"


mapData::mapData()
: _uidCount(1)
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

void mapData::render(UINT layer)
{
	iterVTerrain iter = _terrainsByLayer[layer].begin();
	iterVTerrain end = _terrainsByLayer[layer].end();

	for (; iter != end; ++iter)
		(*iter)->render();
}

void mapData::terrainUp(UINT layer, UINT uid)
{
	int size = _terrainsByLayer[layer].size();
	for (int ii = 0; ii < size; ++ii)
	{
		if (_terrainsByLayer[layer][ii]->getUID() == uid)
		{
			if (ii != 0)
			{
				swap(_terrainsByLayer[layer][ii], _terrainsByLayer[layer][ii - 1]);
			}
			break;
		}
	}
}

void mapData::terrainDown(UINT layer, UINT uid)
{
	int size = _terrainsByLayer[layer].size();
	for (int ii = 0; ii < size; ++ii)
	{
		if (_terrainsByLayer[layer][ii]->getUID() == uid)
		{
			if (ii < _terrainsByLayer[layer].size() - 1)
			{
				swap(_terrainsByLayer[layer][ii], _terrainsByLayer[layer][ii + 1]);
			}
			break;
		}
	}
}

void mapData::changeLayer(UINT sourLayer, UINT destLayer, UID uid)
{
	if( NULL == uid)
		return;

	int idx = getTerrainIndex(sourLayer, uid);
	if (idx < 0)
		return;

	terrain* ter = _terrainsByLayer[sourLayer][idx];
	if (ter)
	{
		_terrainsByLayer[sourLayer].erase(_terrainsByLayer[sourLayer].begin() + idx);
		_terrainsByLayer[destLayer].push_back(ter);
	}
}

void mapData::changeTerrain(UINT layer, int idx, terrain* ter)
{
	//terrain* ter = getTerrain(layer, idx);
}

void mapData::useTrigger(WORD terrainUID)
{
	WORD idx = getUsableTriggerIndex();
	if(TRRIGER_MAX_COUNT != idx)
		_triggerPool[idx] = terrainUID;
}

void mapData::deleteTrigger(WORD terrainUID)
{
	for (int ii = 0; ii < TRRIGER_MAX_COUNT; ++ii)
	{
		if (terrainUID == _triggerPool[ii])
		{
			_triggerPool[ii] = 0;
			break;
		}
	}
}

int mapData::getTriggerIndex(WORD terrinUID)
{
	for (int ii = 0; ii < TRRIGER_MAX_COUNT; ++ii)
	{
		if(terrinUID == _triggerPool[ii])
			return ii;
	}

	return TRRIGER_MAX_COUNT;
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

void mapData::deleteTerrain(UINT layer, UID uid)
{
	terrain* ter = getTerrain(layer, uid);
	if (ter)
	{
		iterVTerrain iter = _terrainsByLayer[layer].begin();
		iterVTerrain end = _terrainsByLayer[layer].end();
		for (; iter != end; ++iter)
		{
			terrain* it = (*iter);
			if (it->getUID() == ter->getUID())
			{
				_terrainsByLayer[layer].erase(iter);
				break;
			}
		}

		iter = _terrains.begin();
		end = _terrains.end();
		for (; iter != end; ++iter)
		{
			terrain* it = (*iter);
			if (it->getUID() == uid)
			{
				_terrains.erase(iter);

				SAFE_RELEASE(it);
				SAFE_DELETE(it);

				break;
			}
		}

		for (int ii = 0; ii < TRRIGER_MAX_COUNT; ++ii)
		{
			if (_triggerPool[ii] == uid)
			{
				_triggerPool[ii] = 0;
				break;
			}
		}
	}
}

void mapData::addTerrainAttribute(UINT layer, UID uid, eAttribute attr)
{
	terrain* ter = getTerrain(layer, uid);
	WORD beforeAttr = ter->getAtrribute();
	if (ter)
	{
		ter->addAttribute(attr);

		switch (attr)
		{
			case eAttr_Collide: { break; }
			case eAttr_Trigger:
			{
				useTrigger(ter->getUID());
				break;
			}
			case eAttr_Breakable: { break; }
			case eAttr_Usable: { break; }
			case eAttr_Trap: { break; }
			case eAttr_Portal: { break; }
			case eAttr_Dialog: { break; }
		}

		// 원래 속성이 아무것도 없었다면 충돌체 생성
		if (beforeAttr == NULL)
		{
			ter->createCollision();
			_colTerrains.push_back(ter);
		}
	}
}

void mapData::removeTerrainAttribute(UINT layer, UID uid, eAttribute attr)
{
	terrain* ter = getTerrain(layer, uid);
	if (ter)
	{
		ter->removeAttribute(attr);

		switch (attr)
		{
			case eAttr_Collide: { break; }
			case eAttr_Trigger:
			{
				deleteTrigger(ter->getUID());
				break;
			}
			case eAttr_Breakable: { break; }
			case eAttr_Usable: { break; }
			case eAttr_Trap: { break; }
			case eAttr_Portal: { break; }
			case eAttr_Dialog: { break; }
		}

		// 속성이 아무것도 없다면 충돌체 제거
		if (ter->getAtrribute() == NULL)
		{
			ter->removeCollision();
			vector<terrain*>::iterator iter = _colTerrains.begin();
			vector<terrain*>::iterator end = _colTerrains.end();
			for (; iter != end; ++iter )
			{
				terrain* terr = (*iter);
				if (terr->getUID() == ter->getUID())
				{
					_colTerrains.erase(iter);
					break;
				}
			}
		}
	}
}

terrain* mapData::getTerrain(UINT layer, UINT uid)
{
	terrain* ter = nullptr;

	int size = _terrainsByLayer[layer].size();
	for (int ii = 0; ii < size; ++ii)
	{
		if (_terrainsByLayer[layer][ii]->getUID() == uid)
		{
			ter = _terrainsByLayer[layer][ii];
			break;
		}
	}

	return ter;
}

terrain* mapData::getTerrain(UINT uid)
{
	terrain* ter = nullptr;
	int size = _terrains.size();
	for (int ii = 0; ii < size; ++ii)
	{
		if (_terrains[ii]->getUID() == uid)
		{
			ter = _terrains[ii];
			break;
		}
	}

	return ter;
}

int mapData::getTerrainIndex(UINT layer, UINT uid)
{
	int size = _terrainsByLayer[layer].size();
	for (int ii = 0; ii < size; ++ii)
	{
		if (_terrainsByLayer[layer][ii]->getUID() == uid)
		{
			return ii;
		}
	}

	return -1;
}

int mapData::getTerrainIndex(UINT uid)
{
	return -1;
}

HRESULT mapData::save(string fileName)
{
	return E_NOTIMPL;
}

HRESULT mapData::load(string fileName)
{
	return E_NOTIMPL;
}

void mapData::addTerrain(UINT layer, terrain* ter)
{
	_terrains.push_back(ter);
	_terrainsByLayer[layer].push_back(ter);

	if (ter->checkAttribute(eAttr_Collide))
		_colTerrains.push_back(ter);

	++_uidCount;
}

void mapData::loadMapDate(string fileName)
{
}

void mapData::loadMapInfo(string fileName)
{
}

void mapData::saveMapDate(string fileName)
{
}

void mapData::saveMapInfo(string fileName)
{
}

WORD mapData::getUsableTriggerIndex()
{
	for (int ii = 0; ii < TRRIGER_MAX_COUNT; ++ii)
	{
		if(0 == _triggerPool[ii])
			return ii;
	}

	return TRRIGER_MAX_COUNT;
}
