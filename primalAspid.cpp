#include "stdafx.h"
#include "primalAspid.h"
#include "mapData.h"
#include "terrain.h"
#include "primalAspidState.h"

HRESULT primalAspid::init(UINT uid, float x, float y)
{
	enemy::init(uid, x, y);
	_subType = eEnemy_Primalaspid;
	
	_width = PRIMALASPID_WIDTH;
	_height = PRIMALASPID_HEIGHT;
	_colWidth = PRIMALASPID_WIDTH / 1.5f;
	_colHeight = PRIMALASPID_HEIGHT / 1.5f;

	_name = L"primalAspid";

	{
		image* img = IMAGEMANAGER->findImage("primalAspid_move");
		ANIMANAGER->addArrayFrameAnimation(_uid, eIDLE, "primalAspid_move"
										   ,0, img->GetMaxFrameX(), PRIMALASPID_ANI_SPEED, true);
	}

	{
		image* img = IMAGEMANAGER->findImage("primalAspid_move");
		ANIMANAGER->addArrayFrameAnimation(_uid, eMOVE_APPROACH, "primalAspid_move"
										   ,0, img->GetMaxFrameX(), PRIMALASPID_ANI_SPEED, true);
	}

	{
		image* img = IMAGEMANAGER->findImage("primalAspid_move");
		ANIMANAGER->addArrayFrameAnimation(_uid, eMOVE_FARWAY, "primalAspid_move"
										   ,0, img->GetMaxFrameX(), PRIMALASPID_ANI_SPEED, true);
	}

	{
		image* img = IMAGEMANAGER->findImage("primalAspid_attack");
		ANIMANAGER->addArrayFrameAnimation(_uid, eATTACK, "primalAspid_attack"
										   ,0, img->GetMaxFrameX(), img->GetMaxFrameX(), PRIMALASPID_ANI_SPEED, false);
	}

	{

		image* img = IMAGEMANAGER->findImage("primalAspid_dead");
		ANIMANAGER->addArrayFrameAnimation(_uid, eDEAD, "primalAspid_dead"
										   ,0, img->GetMaxFrameX(), 5, false);
	}


	_dir	= (eDirection)RND->getInt(eUP);
	_dirUD	= eDirection_None;
	_speed = static_cast<float>(PRIMALASPID_MOVE_SPEED);

	_target = PLAYER;

	primalState* state = new primalIdle;
	state->init(this);
	_state = state;
	_state->start();

	updateCollision();
	
	return S_OK;
}

void primalAspid::release()
{
	enemy::release();
}

void primalAspid::update()
{
	enemy::update();
}

void primalAspid::render()
{
	enemy::render();
}

void primalAspid::attack()
{
	_isFire = true;
}

bool primalAspid::checkTargetInViewRange()
{
	if (CheckInRange( MakePointF(_target->getPosX(), _target->getPosY())
					 , MakePointF(_x, _y), SIGHT_RANGE) )
		return true;
	else
		return false;
}

bool primalAspid::checkTargetInAttackRange()
{
	if (CheckInRange( MakePointF(_target->getPosX(), _target->getPosY())
					, MakePointF(_x, _y), CLOSEST_RANGE) )
		return true;
	else
		return false;
}

void primalAspid::moveToTarget()
{
	_angle = atan2f(_target->getPosX() - _y, _target->getPosX() - _x);
	_x += cosf(_angle) * (_speed * TIMEMANAGER->getElapsedTime());
	_y += sinf(_angle) * (_speed * TIMEMANAGER->getElapsedTime());
	fixPosition();
}

void primalAspid::moveFromTarget()
{
	_angle = atan2f(_target->getPosX() - _y, _target->getPosX() - _x);
	_x -= cosf(_angle) * (_speed * TIMEMANAGER->getElapsedTime());
	_y -= sinf(_angle) * (_speed * TIMEMANAGER->getElapsedTime());
	fixPosition();
}

void primalAspid::fixPosition()
{
	enemy::fixPosition();

	if(_target->getPosX() < _x )
		_dir = eLEFT;
	else
		_dir = eRIGHT;
}

void primalAspid::dead()
{
	enemy::dead();

	primalState* ps = new primalDead;
	ps->init(this);

	changeState(ps);
}

void primalAspid::bulletFire()
{
	_isFire = false;
}

POINTF primalAspid::getBulletFirePoint()
{
	POINTF pof;

	if (eRIGHT == _dir)
		pof.x = _x + _colWidth / 2.f;
	else
		pof.x = _x - _colWidth / 2.f;

	pof.y = _y - _colHeight / 2.f;

	return pof;
}
