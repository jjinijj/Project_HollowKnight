#include "stdafx.h"
#include "mapData.h"
#include "terrain.h"
#include "enemy.h"
#include "npcHeaders.h"
#include "enemyHeaders.h"
#include <string.h>
#include <queue>


mapData::mapData()
: _uidCount(1)
{
	_terrains.clear();
	_colTerrains.clear();
	
	for(int ii = 0 ; ii < eLayer_Count; ++ii)
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
	clear();
}

void mapData::clear()
{
	_uidCount = 1;
	_terrains.clear();
	_colTerrains.clear();

	for(int ii = 0 ; ii < eLayer_Count; ++ii)
		_terrainsByLayer[ii].clear();

	iterVTerrain iter;
	int size = 0;

	for (int ii = 0; ii < eLayer_Count; ++ii)
	{
		size = _terrainsByLayer[ii].size();
		for (iter = _terrainsByLayer[ii].begin(); _terrainsByLayer[ii].end() != iter; )
			iter = _terrainsByLayer[ii].erase(iter);

		_terrainsByLayer[ii].clear();
	}

	for (iter = _colTerrains.begin(); _colTerrains.end() != iter; )
		iter = _colTerrains.erase(iter);

	_colTerrains.clear();

	for (iter = _collisionTerrains.begin(); _collisionTerrains.end() != iter; )
		iter = _collisionTerrains.erase(iter);

	_collisionTerrains.clear();


	for (iter = _terrains.begin(); _terrains.end() != iter; )
	{
		terrain* ter = *iter;
		iter = _terrains.erase(iter);

		SAFE_RELEASE(ter);
		SAFE_DELETE(ter);
	}

	{
		iterMNpc iter = _npcs.begin();
		for (iter; _npcs.end() != iter; )
			iter = _npcs.erase(iter);
	}
	{
		iterEnemy iter = _enemys.begin();
		for (iter; _enemys.end() != iter; )
			iter = _enemys.erase(iter);
	}
	{
		itervActor iter = _actors.begin();
		for(iter; _actors.end() != iter; )
		{
			actorBase* actor = *iter;
			iter = _actors.erase(iter);

			SAFE_RELEASE(actor);
			SAFE_DELETE(actor);
		}
	}

	_terrains.clear();
	_actors.clear();
}

void mapData::update()
{
}

void mapData::render()
{
	if (_background)
	{
		_background->render(1.0f, false);
	}

	renderBack();
	renderActors();
	renderFront();
}

void mapData::renderBack()
{
	if (_background)
	{
		_background->render(1.0f, false);
	}

	iterVTerrain iter;
	iterVTerrain end;
	for (int ii = 0; ii < eLayer_NearFront; ++ii)
	{
		end = _terrainsByLayer[ii].end();
		for (iter = _terrainsByLayer[ii].begin(); iter != end; ++iter)
		{
			(*iter)->render();
		}
	}
}

void mapData::renderFront()
{
	iterVTerrain iter;
	iterVTerrain end;
	for (int ii = eLayer_NearFront; ii < eLayer_Count; ++ii)
	{
		end = _terrainsByLayer[ii].end();
		for (iter = _terrainsByLayer[ii].begin(); iter != end; ++iter)
		{
			(*iter)->render();
		}
	}
}

void mapData::renderActors()
{
	{
		iterMNpc iter = _npcs.begin();
		iterMNpc end = _npcs.end();

		for(iter; end != iter; ++iter)
			iter->second->render();
	}
	{
		iterEnemy iter = _enemys.begin();
		iterEnemy end = _enemys.end();

		for (iter; end != iter; ++iter)
			(*iter)->render();
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

npc* mapData::addNpc(float destX, float destY, eImageUID imgUid)
{
	npc* actor = nullptr;
	int value = (imgUid - eImage_Npc_Elderbug);
	if (eNpc_Elderbug <= value && value < eNpc_Count)
	{
		// npc는 하나씩만.
		if (_npcs.find(value) != _npcs.end())
			return actor;

		float width = 0.f;
		float height = 0.f;

		switch (value)
		{
			case eNpc_Elderbug:
			{
				actor = new elderbug;
				width = elderbug::ELDERBUG_WIDTH;
				height = elderbug::ELDERBUG_HEIGHT;

				break;
			}
			case eNpc_Sly:			{ break; }
			case eNpc_Quirrel:		{ break; }
			case eNpc_Iselda:		{ break; }
			case eNpc_Cornifer:	
			{
				actor = new cornifer;
				width = cornifer::CORNIFER_WIDTH;
				height = cornifer::CORNIFER_HEIGHT;
				break;
			}
			case eNpc_TheLastStag:	{ break; }
		}

		if(actor)
		{
			float x = destX + width / 2.f;
			float y = destY + height;

			actor->init(_uidCount, x, y);
			_npcs.insert(make_pair(value, actor));
			_actors.push_back(actor);

			++_uidCount;
		}
	}

	return actor;
}

enemy* mapData::addEnemy(float destX, float destY, eImageUID imgUid)
{
	enemy* actor = nullptr;
	int value = (imgUid - eImage_Enemy_StartId);
	if (eEnemy_Gruzzer <= value && value < eEnemy_Count)
	{
		float width = 0.f;
		float height = 0.f;

		image* img = IMGDATABASE->getImage(imgUid);
		assert(nullptr != img);

		width = img->GetFrameWidth();
		height = img->GetFrameHeight();

		switch (value)
		{
			case eEnemy_Gruzzer:
			{
				actor = new gruzzer;
				break;
			}
			case eEnemy_Tiktik: 
			{
				tiktik* tik = new tiktik;
				
				float x = destX + width / 2.f;
				float y = destY + height;

				RECTD2D rc = {x, y, x + width, y + height};

				iterVTerrain iter = _terrains.begin();
				iterVTerrain end = _terrains.end();
				for(iter; end != iter; ++iter)
				{
					if (CheckIntersectRect(rc, (*iter)->getCollision()))
					{
						tik->terrainLink((*iter)->getUID());
						break;
					}
				}

				actor = tik;
				break; 
			}

			case eEnemy_Primalaspid:
			{
				actor = new primalAspid;
				break;
			}
		}

		if (actor)
		{
			float x = destX + width / 2.f;
			float y = destY + height;

			actor->init(_uidCount, x, y);
			_enemys.push_back(actor);
			_actors.push_back(actor);

			++_uidCount;
		}
	}

	return actor;
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

		iter = _colTerrains.begin();
		end = _colTerrains.end();
		for (; iter != end; ++iter)
		{
			terrain* it = (*iter);
			if (it->getUID() == ter->getUID())
			{
				_colTerrains.erase(iter);
				break;
			}
		}

		iter = _collisionTerrains.begin();
		end = _collisionTerrains.end();
		for (; iter != end; ++iter)
		{
			terrain* it = (*iter);
			if (it->getUID() == ter->getUID())
			{
				_collisionTerrains.erase(iter);
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

void mapData::deleteActor(UINT uid)
{
	bool isFind = false;

	// npc
	{
		actorBase* actor = nullptr;
		iterMNpc iter = _npcs.begin();
		iterMNpc end = _npcs.end();

		for (iter; end != iter; ++iter)
		{
			actor = iter->second;
			if (actor->getUID() == uid)
			{
				_npcs.erase(iter);
				isFind = true;
				break;
			}
		}
	}

	// enemy
	if (!isFind)
	{
		actorBase* actor = nullptr;
		iterEnemy iter = _enemys.begin();
		iterEnemy end = _enemys.end();

		for (iter; end != iter; ++iter)
		{
			actor = (*iter);
			if (actor->getUID() == uid)
			{
				_enemys.erase(iter);
				isFind= true;
				break;
			}
		}
	}

	{
		actorBase* actor = nullptr;
		itervActor iter = _actors.begin();
		itervActor end = _actors.end();
		for(iter; end != iter; ++iter)
		{
			actor = (*iter);
			if (actor->getUID() == uid)
			{
				_actors.erase(iter);

				SAFE_RELEASE(actor);
				SAFE_DELETE(actor);

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
			case eAttr_Breakable:	{ break; }
			case eAttr_Usable:		{ break; }
			case eAttr_Trap:		{ break; }
			case eAttr_Portal:		{ break; }
			case eAttr_Dialog:		{ break; }
			case eAttr_Chair:		{ break; }
		}

		// 원래 속성이 아무것도 없었다면 충돌체 생성
		if (beforeAttr == NULL)
		{
			ter->createCollision();
			_colTerrains.push_back(ter);
			_collisionTerrains.push_back(ter);
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
			case eAttr_Breakable:	{ break; }
			case eAttr_Usable:		{ break; }
			case eAttr_Trap:		{ break; }
			case eAttr_Portal:		{ break; }
			case eAttr_Dialog:		{ break; }
			case eAttr_Chair:		{ break; }
		}

		// 속성이 아무것도 없다면 충돌체 제거
		if (ter->getAtrribute() == NULL)
		{
			ter->removeCollision();
			{
				vector<terrain*>::iterator iter = _colTerrains.begin();
				vector<terrain*>::iterator end = _colTerrains.end();
				for (; iter != end; ++iter)
				{
					terrain* terr = (*iter);
					if (terr->getUID() == ter->getUID())
					{
						_colTerrains.erase(iter);
						break;
					}
				}
			}

			{
				vector<terrain*>::iterator iter = _collisionTerrains.begin();
				vector<terrain*>::iterator end = _collisionTerrains.end();
				for (; iter != end; ++iter)
				{
					terrain* terr = (*iter);
					if (terr->getUID() == ter->getUID())
					{
						_collisionTerrains.erase(iter);
						break;
					}
				}
			}
		}
	}
}

void mapData::setNextPortal(UINT uid)
{
	map<UINT, eSceneName>::iterator iter = _portalMap.find(uid);

	if (iter != _portalMap.end())
	{
		int idx = (iter->second);
		while (true)
		{
			idx += 1;
			if (eSceneName_None <= idx)
			{
				_portalMap.erase(iter);
				break;
			}
			else if(SCENEMANAGER->isInGameScene((eSceneName)idx))
			{
				iter->second = (eSceneName)idx;
				break;
			}
		}
	}
	else
	{
		_portalMap.insert(make_pair(uid, eSceneName_DirtMouth));
	}
}

void mapData::setNextGenPos(UINT uid)
{
	map<UINT, eSceneName>::iterator iter = _genPosMap.find(uid);

	if (iter != _genPosMap.end())
	{
		int idx = (iter->second);
		while (true)
		{
			idx += 1;
			if (eSceneName_None <= idx)
			{
				_genPosMap.erase(iter);
				break;
			}
			else if(SCENEMANAGER->isInGameScene((eSceneName)idx))
			{
				iter->second = (eSceneName)idx;
				break;
			}
		}
	}
	else
	{
		_genPosMap.insert(make_pair(uid, eSceneName_Title));
	}
}

eSceneName mapData::getLinkedSceneName(UINT uid)
{
	map<UINT, eSceneName>::iterator iter = _portalMap.begin();
	map<UINT, eSceneName>::iterator end = _portalMap.end();

	for (iter; end != iter; ++iter)
	{
		if (iter->first == uid)
		{
			return iter->second;
		}
	}

	return eSceneName_None;
}

eSceneName mapData::getLinkedGenPosSceneName(UINT uid)
{
	map<UINT, eSceneName>::iterator iter = _genPosMap.begin();
	map<UINT, eSceneName>::iterator end = _genPosMap.end();

	for (iter; end != iter; ++iter)
	{
		if (iter->first == uid)
		{
			return iter->second;
		}
	}

	return eSceneName_None;
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

POINTF mapData::getPlayerStartPosition()
{
	POINTF pos = {0.f, 0.f};
	eSceneName beforeSceneName = SCENEMANAGER->getBeforeSceneName();
	if(eSceneName_None == beforeSceneName)
		beforeSceneName = eSceneName_Title;

	UINT uid = NULL;
	
	map<UINT, eSceneName>::iterator iter = _genPosMap.begin();
	map<UINT, eSceneName>::iterator end = _genPosMap.end();
	for(iter; end != iter; ++iter)
	{
		if(beforeSceneName != iter->second)
			continue;

		uid = iter->first;
		break;
	}

	if (NULL != uid)
	{
		int size = _collisionTerrains.size();
		for(int ii = 0; ii < size; ++ii)
		{
			terrain* ter = _collisionTerrains[ii];
			if (ter->getUID() == uid)
			{
				pos.x = ter->getPosX() + (ter->getCollision().right - ter->getCollision().left) * 0.5f;
				pos.y = ter->getPosY() + (ter->getCollision().bottom - ter->getCollision().top) * 0.5f;
				break;
			}
		}
	}

	return pos;
}

actorBase* mapData::getActor(UINT uid)
{
	actorBase* actor = nullptr;
	
	// npc
	{
		iterMNpc iter = _npcs.begin();
		iterMNpc end = _npcs.end();

		for (iter; end != iter; ++iter)
		{
			if (iter->second->getUID() == uid)
			{
				actor = iter->second;
				break;
			}
		}
	}

	// enemy
	if(!actor)
	{
		iterEnemy iter = _enemys.begin();
		iterEnemy end = _enemys.end();

		for(iter; end != iter; ++iter)
		{
			if((*iter)->getUID() == uid)
			{
				actor = (*iter);
				break;
			}
		}
	}

	return actor;
}

HRESULT mapData::save(string fileName)
{
	saveMapDate(fileName);
	saveMapInfo(fileName);
	saveActorData(fileName);

	return S_OK;
}

HRESULT mapData::load(string fileName)
{
	clear();

	int terrainCnt[eLayer_Count];
	int actorCnt = 0;

	bool check = false;

	check = loadMapInfo(fileName, terrainCnt, actorCnt);
	if(check)
		check =	loadMapDate(fileName, terrainCnt);
	if(check)
		loadActorData(fileName, actorCnt);

	if (fileName == SCENEMANAGER->getSceneFileName(eSceneName_Iselda_Store))
		_background = IMAGEMANAGER->findImage("IseldaStoreBG");
	else
		_background = nullptr;

	return S_OK;
}

void mapData::addTerrain(UINT layer, terrain* ter)
{
	_terrains.push_back(ter);
	_terrainsByLayer[layer].push_back(ter);

	if (ter->checkAttribute(eAttr_Collide))
		_colTerrains.push_back(ter);
	if(NULL != ter->getAtrribute())
		_collisionTerrains.push_back(ter);

	++_uidCount;
}

bool mapData::loadMapDate(string fileName, int* terrainCnt)
{
	string address;
	address.clear();

	address.append(format("data/%s_data.txt",fileName.c_str()));
	
	int count = 0;
	for(int ii = 0 ; ii < eLayer_Count; ++ii )
		count += terrainCnt[ii];
	TARRAINPACK* packs = new TARRAINPACK[count];

	HANDLE file;
	DWORD read;
	file = CreateFile(address.c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(!file)
		return false;

	ReadFile(file, packs, sizeof(TARRAINPACK) * count, &read, NULL);
	CloseHandle(file);

	int size = 0;
	for ( int layer = 0; layer < eLayer_Count; ++layer )
	{
		for ( int ii = 0; ii < terrainCnt[layer]; ++ii )
		{
			terrain* ter = nullptr;
			switch ( packs[size + ii].type )
			{
				case eTerrain_Frame: { ter = new terrainFrame; break;	}
				case eTerrain_Drag:	 { ter = new terrainDrag; break;	}
				case eTerrain_Clear: { ter = new terrainClear; break;	}
				default: continue;
			}

			ter->loadPack(&packs[size + ii]);
			addTerrain(layer, ter);
		}
		size += terrainCnt[layer];
	}

	SAFE_DELETE_ARRAY(packs);

	return true;
}

bool mapData::loadMapInfo(string fileName, int* terrainCnt, int& actorCnt)
{
	string address;
	char info[1024] = "";
	queue<int> infos;
	address.clear();

	address.append(format("data/%s_info.txt",fileName.c_str()));

	HANDLE file;
	DWORD read;
	file = CreateFile(address.c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, info, 512, &read, NULL);
	CloseHandle(file);

	char* temp;
	char* token;
	int iTemp = 0;
	token = strtok_s(info, ",", &temp);
	
	while (nullptr != token)
	{
		iTemp = atoi(token);
		infos.push(iTemp);

		token = strtok_s(NULL, ",", &temp);
	}

	// uidCount
	if (!infos.empty())
	{
		_uidCount = infos.front();
		infos.pop();
	}


	// 레이어당 지형의 수
	for(int ii = 0; ii < eLayer_Count && !infos.empty(); ++ii)
	{
		terrainCnt[ii] = infos.front();
		infos.pop();
	}

	for(int ii = 0; ii < TRRIGER_MAX_COUNT; ++ii)
	{
		if (!infos.empty())
		{
			_triggerPool[ii] = infos.front();
			infos.pop();
		}
		else
			_triggerPool[ii] = NULL;
	}

	// 액터 수
	if(!infos.empty())
	{
		actorCnt = infos.front();
		infos.pop();

	}

	if (!infos.empty())
	{
		int potalCnt = infos.front();
		infos.pop();

		// 포탈
		if (!infos.empty())
		{
			for(int ii = 0 ; ii < potalCnt; ++ii)
			{
				int key = infos.front();
				infos.pop();
				int value = infos.front();
				infos.pop();

				_portalMap.insert(make_pair(key, (eSceneName)value));
			}
		}
	}

	if (!infos.empty())
	{
		int genCnt = infos.front();
		infos.pop();

		// 시작 위치
		if (!infos.empty())
		{
			for(int ii = 0; ii < genCnt; ++ii)
			{
				int key = infos.front();
				infos.pop();
				int value = infos.front();
				infos.pop();

				_genPosMap.insert(make_pair(key, (eSceneName)value));
			}
			return true;
		}
	}
	else
		return true;

	return false;
}

bool mapData::loadActorData(string fileName, int actorCnt)
{
	string address;
	address.clear();

	address.append(format("data/%s_Actor_data.txt", fileName.c_str()));

	ACTORPACK* packs = new ACTORPACK[actorCnt];

	HANDLE file;
	DWORD read;
	file = CreateFile(address.c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, packs, sizeof(ACTORPACK) * actorCnt, &read, NULL);
	CloseHandle(file);

	for (int ii = 0; ii < actorCnt; ++ii)
	{
		ACTORPACK* pack = &packs[ii];
		if(eActor_Npc == pack->type)
		{
			npc* actor = nullptr;

			switch (pack->subType)
			{
				case eNpc_Elderbug:			{ actor = new elderbug;	break;	}
				case eNpc_Sly:				{ break; }
				case eNpc_Quirrel:			{ break; }
				case eNpc_Iselda:			{ break; }
				case eNpc_Cornifer:			{ actor = new cornifer; break; }
				case eNpc_TheLastStag:		{ break; }
			}

			if(actor)
			{
				actor->init(pack->uid, pack->x, pack->y);
				actor->loadPack(pack);
				_actors.push_back(actor);
				_npcs.insert(make_pair(actor->getSubType(), actor));
			}
		}
		else if(eActor_Enemy == pack->type)
		{
			enemy* actor = nullptr;
			ACTORPACK* pack = &packs[ii];

			switch (pack->subType)
			{
				case eEnemy_Gruzzer:		{ actor = new gruzzer;		break; }
				case eEnemy_Tiktik:			{ actor = new tiktik;		break; }
				case eEnemy_Primalaspid:	{ actor = new primalAspid;	break; }
				case eEnemy_Mawlek:			{ break; }
			}

			if (actor)
			{
				actor->init(pack->uid, pack->x, pack->y);
				actor->loadPack(pack);
				_actors.push_back(actor);
				_enemys.push_back(actor);
			}
		}
	}

	SAFE_DELETE_ARRAY(packs);

	return true;
}

void mapData::saveMapInfo(string fileName)
{
	string address;
	string info;
	address.clear();
	info.clear();

	address.append(format("data/%s_info.txt", fileName.c_str()));

	info.append(format("%d,", _uidCount));
	for (int ii = 0; ii < eLayer_Count; ++ii)
		info.append(format("%d,", _terrainsByLayer[ii].size()));
	for (int ii = 0; ii < TRRIGER_MAX_COUNT; ++ii)
		info.append(format("%d,", _triggerPool[ii]));

	info.append(format("%d,", _actors.size()));
	
	// 포탈 갯수
	info.append(format("%d,", _portalMap.size()));
	// 포탈 정보
	{
		map<UINT, eSceneName>::iterator iter = _portalMap.begin();
		map<UINT, eSceneName>::iterator end = _portalMap.end();
		for (iter; end != iter; ++iter)
			info.append(format("%d, %d,", iter->first, iter->second)); 
	}

	// 위치 젠 갯수
	info.append(format("%d,", _genPosMap.size()));
	// 위치 젠 정보
	{
		map<UINT, eSceneName>::iterator iter = _genPosMap.begin();
		map<UINT, eSceneName>::iterator end = _genPosMap.end();
		for (iter; end != iter; ++iter)
			info.append(format("%d, %d,", iter->first, iter->second));
	}


	HANDLE file;
	DWORD write;
	file = CreateFile(address.c_str(), GENERIC_WRITE, NULL, NULL,
					  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, info.c_str(), info.size(), &write, NULL);
	CloseHandle(file);
}

void mapData::saveMapDate(string fileName)
{
	string address;
	address.clear();

	address.append(format("data/%s_data.txt",fileName.c_str()));

	TARRAINPACK* packs = new TARRAINPACK[_terrains.size()];
	int size = 0;
	int count = 0;
	for ( int ii = 0; ii < eLayer_Count; ++ii )
	{
		size = _terrainsByLayer[ii].size();
		for ( int idx = 0; idx < size; ++idx )
		{
			packs[count].clear();
			packs[count] = *_terrainsByLayer[ii][idx]->makePack();
			++count;
		}
	}

	HANDLE file;
	DWORD write;

	file = CreateFile(address.c_str(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, packs, sizeof(TARRAINPACK) * count, &write, NULL);
	CloseHandle(file);

	SAFE_DELETE_ARRAY(packs);
}

void mapData::saveActorData(string fileName)
{
	string address;
	address.clear();

	address.append(format("data/%s_Actor_data.txt", fileName.c_str()));

	int size = _actors.size();
	ACTORPACK* packs = new ACTORPACK[_actors.size()];
	for (int ii = 0; ii < size; ++ii)
	{
		packs[ii].clear();
		packs[ii] = *_actors[ii]->makePack();
	}

	HANDLE file;
	DWORD write;

	file = CreateFile(address.c_str(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, packs, sizeof(ACTORPACK) * size, &write, NULL);
	CloseHandle(file);

	SAFE_DELETE_ARRAY(packs);
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


