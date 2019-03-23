#pragma once
#include "enemy.h"

class player;
class bulletManager;
class primalAspid : public enemy
{
public:

	enum 
	{
		// 플레이어를 발견할 수 있는 거리
		SIGHT_RANGE = 800,

		// 플레이어와 최대 인접하는 거리. 이 거리 안에서 공격을 하고 도망갈거임
		CLOSEST_RANGE = 200,

		PRIMALASPID_WIDTH = 100,
		PRIMALASPID_HEIGHT = 100,

		PRIMALASPID_ANI_SPEED = 5,

		BULLET_SIZE = 50,
	};

	enum eSTATE
	{
		eIDLE,
		eMOVE_APPROACH,
		eMOVE_FARWAY,
		eATTACK,
		eDEAD,

		eSTATE_NONE,
		eSTATE_COUNT = eSTATE_NONE,
	};

	player* _target;
	float _angle;

	bool _isFire;

public:
	HRESULT init(UINT uid, float x, float y);
	void release();
	void update();
	void render();

	void attack();

	bool checkTargetInViewRange();
	bool checkTargetInAttackRange();

	void moveToTarget();
	void moveFromTarget();
	void dead();

	void bulletFire();
	POINTF getBulletFirePoint();

	void setPlayerLink(player* target) { _target = target; }
	
	bool isFire()						{ return _isFire; }

private:
	void fixPosition();
};