#include "stdafx.h"
#include "actorManager.h"
#include "npc.h"
#include "enemy.h"


actorManager::actorManager()
{
}


actorManager::~actorManager()
{
}

HRESULT actorManager::init()
{
	return E_NOTIMPL;
}

void actorManager::release()
{
}

void actorManager::render()
{
}

void actorManager::update()
{
}

npc* actorManager::createNPC(float x, float y, WORD type)
{
	switch (type)
	{
		default:
			break;
	}


	return nullptr;
}

enemy* actorManager::createEnemy(float x, float y, WORD type)
{
	return nullptr;
}

void actorManager::destroyActor(UINT uid)
{
}

npc* actorManager::getNPC(UINT uid)
{
	return nullptr;
}

enemy* actorManager::getEnemy(UINT uid)
{
	return nullptr;
}

map<UINT, npc*> actorManager::getNPCs()
{
	return map<UINT, npc*>();
}

map<UINT, enemy*> actorManager::getEnemys()
{
	return map<UINT, enemy*>();
}
