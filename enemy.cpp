#include "stdafx.h"
#include "enemy.h"
#include "actorState.h"

enemy::enemy()
: _isAlive(false)
, _hp(0)
, _speed(0)
, _target(nullptr)
, _dir(eDirection_None)
, _dirUD(eDirection_None)
{
	_colSize = {0.f, 0.f};
	_collision = {};
}

enemy::~enemy()
{
}

HRESULT enemy::init(UINT uid, float x, float y)
{
	actorBase::init(uid, x, y);
	_type = eActor_Enemy;

	return S_OK;
}

void enemy::update()
{
	actorBase::update();
}

void enemy::release()
{
	actorBase::release();
}

void enemy::render()
{
	actorBase::render();
}

bool enemy::isFireState()
{
	return false;
}

void enemy::bulletFire()
{
}

POINTF enemy::getBulletFirePoint()
{
	return POINTF();
}

void enemy::takeDamage(int damage)
{
	_hp -= damage;
	if(_hp < 0)
		_isAlive = false;
}
