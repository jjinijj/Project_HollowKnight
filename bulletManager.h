#pragma once
#include "singletonBase.h"
#include"bullet.h"
#include <queue>
#include <list>


class player;
class bulletManager : public singletonBase<bulletManager>
{
private:
	enum 
	{
		BULLET_COUNT = 50,
	};

	queue<arcBullet*>	 _arcBulletQ;		// 포물선 그리는 bullet pool
	queue<linearBullet*> _linearbulletQ;	// 직선 bullet pool
	
	list<bullet*> _bulletList;				// 현재 사용중인 bullets
	list<bullet*>::iterator _iter;			// iterator

	bullet* _playerBullet;					// 플레이어 bullet;

	player* _player;
	enemyManager* _enemyM;
	objectManager* _objM;

public:
	void init();
	void update();
	void render();
	void release();

	// bullet 생성 : q에서 bullet을 꺼냄
	bullet* createBullet(bullet::eBULLET_TYPE type);
	void firePlayerBullet(POINTF pos, float angle);
	bool checkPlayerBullet();
	void setLink(player* p, enemyManager* emM, objectManager* objM) {_player = p, _enemyM = emM; _objM = objM;}


private:
	// bullet 충돌 확인
	bool checkHitSomething(bullet* bt);
	
	// bullet 충돌 확인
	bool checkPlayerBulletHitSomething();

	// 맵 안에 있냐?
	bool checkBulletIntheField(bullet* bt);

	// q에 돌려줌
	list<bullet*>::iterator returnBulletQ(list<bullet*>::iterator it);
};