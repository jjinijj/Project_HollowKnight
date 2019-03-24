#include "stdafx.h"
#include "gruzzer.h"
#include "mapData.h"
#include "terrain.h"
#include "gruzzerState.h"

HRESULT gruzzer::init(UINT uid, float x, float y)
{
	enemy::init(uid, x, y);
	_subType = eEnemy_Gruzzer;
	_width = GRUZZER_WIDTH;
	_height = GRUZZER_HEIGHT;
	_colWidth = GRUZZER_WIDTH * 0.9f;
	_colHeight = GRUZZER_HEIGHT * 0.9f;

	_name = L"gruzzer";

	{
		image* img = IMAGEMANAGER->findImage("gruzzer_move");
		ANIMANAGER->addArrayFrameAnimation(uid, eMOVE, "gruzzer_move"
										   , 0, img->GetMaxFrameX(), GRUZZER_ANI_SPEED, true);
	}
	{
		image* img = IMAGEMANAGER->findImage("gruzzer_dead");
		ANIMANAGER->addArrayFrameAnimation(uid, eDEAD, "gruzzer_dead"
										   , 0, img->GetMaxFrameX(), GRUZZER_ANI_SPEED, false);
	}


	gruzzerMove* moveState = new gruzzerMove;
	moveState->init(this);

	_state = moveState;
	_state->start();

	_hp = GRUZZER_MAX_HP;
	_speed = GRUZZER_SPEED;

	_dir = (eDirection)RND->getInt(eUP);
	_dirUD = (eDirection)RND->getFromIntTo(eUP, eDirection_None);
	_isAlive = true;

	updateCollision();

	return S_OK;
}

void gruzzer::release()
{
	enemy::release();
}

void gruzzer::update()
{
	enemy::update();
}

void gruzzer::render()
{
	enemy::render();
}

void gruzzer::move()
{
	if( eRIGHT == _dir)
		_x += _speed;
	else
		_x -= _speed;

	if(eUP == _dirUD )
		_y -= _speed;
	else
		_y += _speed;

	fixPosition();
}

void gruzzer::dead()
{
	enemy::dead();
	
	gruzzerDead* deadState = new gruzzerDead;
	deadState->init(this);

	changeState(deadState);
}
