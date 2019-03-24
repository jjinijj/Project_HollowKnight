#pragma once
#include "actorBase.h"


class player;
class mapData;
class enemy: public actorBase
{
public:

protected:
	bool _isAlive;
	bool _isAppear;

	int _hp;
	int _speed;

	player* _target;
	mapData* _mapData;

public:
	enemy();
	~enemy();

	HRESULT init(UINT uid, float x, float y);
	void update();
	void release();
	void render();
	
	virtual void setTarget(player* target)  { _target = target; }
	virtual void mapDataLink(mapData* data) { _mapData = data;  }

	ACTORPACK* makePack();
	void loadPack(ACTORPACK* pack);

	virtual bool isFireState();
	virtual void bulletFire();
	virtual POINTF getBulletFirePoint();

	virtual void dead();

	void takeDamage(int damage);
	void setDisappear() {_isAppear = false;}

	bool isAlive()  { return _isAlive; }
	bool isAppear() { return _isAppear;}
	eEnemy_Type getEnemyType() { return (eEnemy_Type)_subType; }

protected:
	virtual void fixPosition();
};