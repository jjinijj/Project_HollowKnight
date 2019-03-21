#pragma once
#include "singletonBase.h"


class actorBase;
class enemy;
class npc;
class mapData;
class actorManager : public singletonBase<actorManager>
{

private:
	typedef map<UINT, actorBase*>::iterator iterA;
	typedef map<UINT, enemy*>::iterator		iterE;
	typedef map<UINT, npc*>::iterator		iterN;
private:   
	map<UINT, actorBase*>	_actorMap;
	map<UINT, enemy*>		_enemyMap;
	map<UINT, npc*>			_npcMap;

	mapData* _mapData;

	UINT _uidCount;

public:
	actorManager();
	~actorManager();

	HRESULT init();
	void release();
	void render();
	void update();

	void mapDataLink(mapData* data) {_mapData = data;}

	npc* createNPC(float x, float y, WORD type);
	enemy* createEnemy(float x, float y, WORD type);

	void destroyActor(UINT uid);

	actorBase* getActor(UINT uid);
	npc* getNPC(UINT uid);
	enemy* getEnemy(UINT uid);

	map<UINT, npc*>		getNPCs();
	map<UINT, enemy*>	getEnemys();
};

