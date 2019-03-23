#include "stdafx.h"
#include "enemy.h"
#include "actorState.h"

enemy::enemy()
: _isAlive(false)
, _hp(0)
, _speed(0)
, _target(nullptr)
{
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
	updateCollision();
}

void enemy::release()
{
	actorBase::release();
}

void enemy::render()
{
	actorBase::render();
}

ACTORPACK* enemy::makePack()
{
	ACTORPACK* pack = actorBase::makePack();
	
	if (pack)
	{
	}

	return pack;
}

void enemy::loadPack(ACTORPACK* pack)
{
	if (pack)
	{
		actorBase::loadPack(pack);
	}
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

void enemy::updateCollision()
{
	_collision = {   _x - _colWidth / 2.f, _y - _colHeight
					,_x + _colWidth / 2.f, _y};
}
