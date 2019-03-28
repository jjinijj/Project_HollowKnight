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

	queue<arcBullet*>	 _arcBulletQ;		// ������ �׸��� bullet pool
	queue<linearBullet*> _linearbulletQ;	// ���� bullet pool

	list<bullet*> _bulletList;				// ���� ������� bullets
	bullet* _playerBullet;					// �÷��̾� bullet;

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
	// bullet �浹 Ȯ��
	bool checkHitSomething(bullet* bt);

	// bullet �浹 Ȯ��
	bool checkPlayerBulletHitSomething();

	// �� �ȿ� �ֳ�?
	bool checkBulletIntheField(bullet* bt);

	// q�� ������
	iterB returnBulletQ(iterB it);
};

