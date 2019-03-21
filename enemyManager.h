#pragma once
#include "singletonBase.h"
#include "enemy.h"
#include <list>


enum
{
	// 스테이지에 존재하는 적의 최대 수
	ENEMY_COUNT_IN_STAGE = 5,

	// 보스 등장하기 위한 적의 수
	ENEMY_COUNT_NEED_EXIST_BOSS = 10,

};


typedef std::list<enemy*>					lEnemy;
typedef std::list<enemy*>::iterator			ilEnemy;
typedef std::list<enemy*>::const_iterator	cilEnemy;

class objectManager;
class bulletManager;
class player;
class enemyManager : public singletonBase<enemyManager>
{
private:

	lEnemy	_enemyList;
	lEnemy	_deadEnemyList;
	ilEnemy	_iter;

	objectManager* _objM;
	bulletManager* _bulletM;
	player* _target;

	int _enemyCnt;
	bool _isExistBoss;

public:
	
	HRESULT init();
	void update();
	void render();
	void release();

	void setManagerLink(objectManager* objM, bulletManager* bulletM) {_objM = objM; _bulletM = bulletM;}
	void setPlayerLink(player* target) {_target = target;}

	void createEnemy(eENEMY_TYPE type);

	//void attack(enemy* em);
	void bulletFire(enemy* em);

	void removeEnemy(int uid);
	void hitEnemy(int uid);
	const lEnemy& getEnemyList() {return _enemyList;}

private:
	enemy* findEnemy(int uid);
	bool checkAllLinkedFinish() {return (_objM  != nullptr && _bulletM != nullptr);}
};