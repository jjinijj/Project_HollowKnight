#include "stdafx.h"
#include "actorManager.h"
#include "enemyHeaders.h"
#include "npcHeaders.h"


actorManager::actorManager()
: _uidCount(0)
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
	{
		actorBase* actor = iter->second;
		if(CAMERA->inRangeInActiveArea(	 actor->getRect().left
										,actor->getRect().top
										,actor->getWidth()
										,actor->getHeight()))
			actor->update();
	}
}

npc* actorManager::createNPC(float x, float y, WORD type)
{
	npc* n = nullptr;

	switch(type)
	{
		case eNpc_Elderbug:	  {n = new elderbug;  break; }
		case eNpc_Sly:		  { break; }
		case eNpc_Quirrel:	  { break; }
		case eNpc_Iselda:	  { break; }
		case eNpc_Cornifer:	  { break; }
		case eNpc_TheLastStag:{ break; }

		default:
			break;
	}

	if(n)
	{
		++_uidCount;
		n->init(_uidCount, x, y);

		_actorMap.insert(make_pair(n->getUID(), n));
		_npcMap.insert(make_pair(n->getUID(),n));
	}

	return n;
}

enemy* actorManager::createEnemy(float x, float y, WORD type)
{
	enemy* em = nullptr;

	switch (type)
	{
		case eEnemy_Gruzzer:	{ em = new gruzzer; break;}
		case eEnemy_Tiktik:		{ em = new tiktik; break;}
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

		_actorMap.insert(make_pair(em->getUID(), em));
		_enemyMap.insert(make_pair(em->getUID(), em));
	}

	switch (type)
	{
		case eEnemy_Gruzzer:	{ break; }
		case eEnemy_Tiktik:		
		{



			break;
		}
		//case eEnemy_Primalaspid:{ em = new gruzzer; break;}
		//case eEnemy_Mawlek:		{ em = new gruzzer; break;}

		default:
			break;
	}

	return em;
}

npc* actorManager::addNpc(npc* npc)
{
	if (npc)
	{
		_actorMap.insert(make_pair(npc->getUID(), npc));
		_npcMap.insert(make_pair(npc->getUID(), npc));
	}

	return npc;
}

enemy* actorManager::addEnemy(enemy* enemy)
{
	if (enemy)
	{
		enemy->mapDataLink(_mapData);
		_actorMap.insert(make_pair(enemy->getUID(), enemy));
		_enemyMap.insert(make_pair(enemy->getUID(), enemy));
	}

	return enemy;
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
