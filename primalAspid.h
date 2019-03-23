#pragma once
#include "enemy.h"

class player;
class bulletManager;
class primalAspid : public enemy
{
public:

	enum 
	{
		// �÷��̾ �߰��� �� �ִ� �Ÿ�
		SIGHT_RANGE = 800,

		// �÷��̾�� �ִ� �����ϴ� �Ÿ�. �� �Ÿ� �ȿ��� ������ �ϰ� ����������
		CLOSEST_RANGE = 200,

		PRIMALASPID_WIDTH = 171,
		PRIMALASPID_HEIGHT = 126,

		PRIMALASPID_ANI_SPEED = 13,

		PRIMALASPID_MOVE_SPEED = 250,

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

	bool isFireState()						{ return _isFire; }

private:
	void fixPosition();
};