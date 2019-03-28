#include "stdafx.h"
#include "enemy.h"
#include "actorState.h"
#include "mapData.h"
#include "terrain.h"

enemy::enemy()
: _isAlive(false)
, _isAppear(false)
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

	_isAlive = true;
	_isAppear = true;

	_renderAlpha = 1.0f;

	return S_OK;
}

void enemy::update()
{
	if (0 < _renderAlphaCnt)
	{
		_renderAlpha = _renderAlphaCnt % 10 * 0.1f;
		++_renderAlphaCnt;

		if (30 < _renderAlphaCnt)
		{
			_renderAlphaCnt = 0;
			_renderAlpha = 1.f;
		}
	}

	actorBase::update();
	updateCollision();
}

void enemy::release()
{
	actorBase::release();
	_target = nullptr;
	_mapData = nullptr;
}

void enemy::render()
{
	actorBase::render();

	if(DEVTOOL->checkDebugMode(DEBUG_SHOW_TEXT))
		D2DMANAGER->drawText(format(L"%d", _hp).c_str(), _collision.left, _collision.top - 10, false);
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

void enemy::dead()
{
	_isAlive = false;
	_colWidth = 0.f;
	_colHeight = 0.f;

	_hp = 0;

	SOUNDMANAGER->play("enemy_death_sword");
}

void enemy::takeDamage(int damage)
{
	_hp -= damage;
	if (_hp <= 0)
		dead();

	_renderAlphaCnt = 9;
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
