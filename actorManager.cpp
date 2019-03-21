#include "stdafx.h"
#include "actorManager.h"
#include "npc.h"
#include "enemy.h"
#include "gruzzer.h"



actorManager::actorManager()
{
}


actorManager::~actorManager()
{
}

HRESULT actorManager::init()
{
	return S_OK;
}

void actorManager::release()
{
	{
		iterN iter = _npcMap.begin();
		for (iter; _npcMap.end() != iter; )
			iter = _npcMap.erase(iter);
	}
	{
		iterE iter = _enemyMap.begin();
		for (iter; _enemyMap.end() != iter; )
			iter = _enemyMap.erase(iter);
	}

	iterA iter = _actorMap.begin();
	for (iter; _actorMap.end() != iter; )
	{
		actorBase* actor = iter->second;
		iter = _actorMap.erase(iter);

		SAFE_RELEASE(actor);
		SAFE_DELETE(actor);
	}
	_actorMap.clear();
}

void actorManager::render()
{
	iterA iter = _actorMap.begin();
	iterA end = _actorMap.end();
	for(iter; iter != end; ++iter)
		iter->second->render();
}

void actorManager::update()
{
	iterA iter = _actorMap.begin();
	iterA end = _actorMap.end();
	for (iter; iter != end; ++iter)
		iter->second->update();
}

npc* actorManager::createNPC(float x, float y, WORD type)
{
	return nullptr;
}

enemy* actorManager::createEnemy(float x, float y, WORD type)
{
	enemy* em = nullptr;

	switch (type)
	{
		case eEnemy_Gruzzer:	{ em = new gruzzer; break;}
		//case eEnemy_Tiktik:		{ em = new gruzzer; break;}
		//case eEnemy_Primalaspid:{ em = new gruzzer; break;}
		//case eEnemy_Mawlek:		{ em = new gruzzer; break;}

		default:
			break;
	}

	if (em)
	{
		++_uidCount;
		em->init(_uidCount, x, y);
		em->mapDataLink(_mapData);

		_actorMap.insert(make_pair(em->getUid(), em));
		_enemyMap.insert(make_pair(em->getUid(), em));
	}

	return em;
}

void actorManager::destroyActor(UINT uid)
{
	if(_actorMap.find(uid) == _actorMap.end())
		return;

	actorBase* actor = _actorMap[uid];
	
	_actorMap.erase(uid);
	if (actor->getSubType() == eActor_Enemy)
		_enemyMap.erase(uid);
	else if (actor->getSubType() == eActor_Npc)
		_actorMap.erase(uid);

	SAFE_RELEASE(actor);
	SAFE_DELETE(actor);
}

actorBase* actorManager::getActor(UINT uid)
{
	actorBase* actor = nullptr;
	if(_actorMap.find(uid) != _actorMap.end())
		actor = _actorMap[uid];

	return actor;
}

npc* actorManager::getNPC(UINT uid)
{
	npc* n = nullptr;
	if(_npcMap.find(uid) != _npcMap.end())
		n = _npcMap[uid];

	return n;
}

enemy* actorManager::getEnemy(UINT uid)
{
	enemy* em = nullptr;
	if(_enemyMap.find(uid) != _enemyMap.end())
		em = _enemyMap[uid];

	return em;
}

map<UINT, npc*> actorManager::getNPCs()
{
	return map<UINT, npc*>();
}

map<UINT, enemy*> actorManager::getEnemys()
{
	return map<UINT, enemy*>();
}
