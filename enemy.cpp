#include "stdafx.h"
#include "enemy.h"
#include "actorState.h"
#include "mapData.h"
#include "terrain.h"

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

void enemy::fixPosition()
{
	updateCollision();

	vector<terrain*>* vCol = _mapData->getColliderTerrains();
	vector<terrain*>::iterator iter = vCol->begin();
	vector<terrain*>::iterator end = vCol->end();

	int offsetX = 0;
	int offsetY = 0;
	terrain* ter = nullptr;
	RECTD2D terRc = {};

	for ( iter; iter != end; ++iter )
	{
		ter		= (*iter);
		terRc	= ter->getCollision();

		if( !CheckIntersectRect(_collision, terRc) )
			continue;

		offsetX = GetIntersectOffsetX_doNotBoard(_collision, terRc);
		offsetY = GetIntersectOffsetY_doNotBoard(_collision, terRc);

		// 상하
		if (terRc.left <= _collision.left && _collision.right < terRc.right )
		{
			_dirUD = (eDirection)(eDOWN - _dirUD + eUP);

			_y += offsetY;
		} 
		// 좌우
		else if ( terRc.top <= _collision.top && _collision.bottom <= terRc.bottom )
		{
			_dir = (eDirection)(eLEFT - _dir);

			_x += offsetX;
		}
		// 모서리
		else if ( abs(offsetX) < abs(offsetY) )
		{
			_dir = (eDirection)(eLEFT - _dir);
			_x += offsetX;
		}
		// 모서리
		else
		{
			_dirUD = (eDirection)(eDOWN - _dirUD + eUP);
			_y += offsetY;
		}
	}
}
