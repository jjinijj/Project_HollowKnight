#pragma once
#include "singletonBase.h"


class actorBase;
class enemy;
class npc;
class actorManager : public singletonBase<actorManager>
{

private:
	map<UINT, actorBase*> _actorMap;
	map<UINT, enemy*> _enemyMap;
	map<UINT, npc*> _npcMap;

public:
	actorManager();
	~actorManager();


	HRESULT init();
	void release();
	void render();
	void update();

	npc* createNPC(UINT uid, float x, float y, WORD type);
	enemy* createEnemy(float x, float y, WORD type);

	void destroyActor(UINT uid);

	npc* getNPC(UINT uid);
	enemy* getEnemy(UINT uid);

	map<UINT, npc*>		getNPCs();
	map<UINT, enemy*>	getEnemys();
};

