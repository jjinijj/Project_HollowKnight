#pragma once
#include "actorBase.h"


class player;
class mapData;
class enemy: public actorBase
{
public:


	enum eDirection
	{
		eRIGHT,
		eLEFT,
		eUP,
		eDOWN,

		eDirection_None,
		eDirection_Count = eDirection_None,
	};

protected:
	bool _isAlive;

	int _hp;
	int _speed;

	player* _target;
	mapData* _mapData;

	eDirection _dir;
	eDirection _dirUD;

public:
	enemy();
	~enemy();

	HRESULT init(UINT uid, float x, float y);
	void update();
	void release();
	void render();
	
	void setTarget(player* target)  { _target = target; }
	void mapDataLink(mapData* data) { _mapData = data;  }

	virtual bool isFireState();
	virtual void bulletFire();
	virtual POINTF getBulletFirePoint();

	void takeDamage(int damage);

	bool isAlive() { return _isAlive; }
	eEnemy_Type getEnemyType() { return (eEnemy_Type)_subType; }
};