#pragma once
#include "singletonBase.h"
#include <queue>


class actorBase;
class enemy;
class npc;
class bullet;
class arcBullet;
class linearBullet;
class mapData;
class objectManager;
class actorManager : public singletonBase<actorManager>
{

public:
	enum 
	{
		BULLET_COUNT = 50,
	};

private:
	typedef map<UINT, actorBase*>::iterator iterA;
	typedef map<UINT, enemy*>::iterator		iterE;
	typedef map<UINT, npc*>::iterator		iterN;

	typedef list<bullet*>::iterator			iterB;
private:   
	map<UINT, actorBase*>	_actorMap;
	map<UINT, enemy*>		_enemyMap;
	map<UINT, npc*>			_npcMap;

	queue<arcBullet*>	 _arcBulletQ;		// 포물선 그리는 bullet pool
	queue<linearBullet*> _linearbulletQ;	// 직선 bullet pool

	list<bullet*> _bulletList;				// 현재 사용중인 bullets
	bullet* _playerBullet;					// 플레이어 bullet;

	mapData*		_mapData;
	objectManager*	_objM;

	UINT _uidCount;

public:
	actorManager();
	~actorManager();

	HRESULT init();
	void release();
	void render();
	void update();

	void mapDataLink(mapData* data) { _mapData = data; }
	void objMLink(objectManager* m) { _objM = m; }

	void firePlayerBullet(POINTF pos, float angle);
	void fireEnemyBullet(enemy* em);
	bool checkPlayerBullet();

	void hitEnemy(enemy* em, UINT damage);
	void hitEnemy(UINT uid, UINT damage);

	npc* createNPC(float x, float y, WORD type);
	enemy* createEnemy(float x, float y, WORD type);
	bullet* createBullet(WORD type);

	npc* addNpc(npc* npc);
	enemy* addEnemy(enemy* enemy);


	void destroyActor(UINT uid);

	actorBase* getActor(UINT uid);
	npc* getNPC(UINT uid);
	enemy* getEnemy(UINT uid);

	map<UINT, npc*>		getNPCs() { return _npcMap; }
	map<UINT, enemy*>	getEnemys() { return _enemyMap;}


private:
	// bullet 충돌 확인
	bool checkHitSomething(bullet* bt);

	// bullet 충돌 확인
	bool checkPlayerBulletHitSomething();

	// 맵 안에 있냐?
	bool checkBulletIntheField(bullet* bt);

	// q에 돌려줌
	iterB returnBulletQ(iterB it);
};

