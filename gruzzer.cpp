#include "stdafx.h"
#include "gruzzer.h"
#include "mapData.h"
#include "terrain.h"
#include "gruzzerState.h"

HRESULT gruzzer::init(UINT uid, float x, float y)
{
	enemy::init(uid, x, y);
	_subType = eEnemy_Gruzzer;

	{
		image* img = IMAGEMANAGER->findImage("gruzzer_move");
		ANIMANAGER->addArrayFrameAnimation(uid, eMOVE, "gruzzer_move"
										   , 0, img->GetMaxFrameX(), GRUZZER_ANI_SPEED, true);
	}
	{
		image* img = IMAGEMANAGER->findImage("gruzzer_dead");
		ANIMANAGER->addArrayFrameAnimation(uid, eDEAD, "gruzzer_dead"
										   , 0, img->GetMaxFrameX(), GRUZZER_ANI_SPEED, true);
	}


	gruzzerMove* moveState = new gruzzerMove;
	moveState->init(this);

	_state = moveState;
	_state->start();

	_hp = GRUZZER_MAX_HP;
	_speed = GRUZZER_SPEED;

	_dir = eLEFT;
	_dirUD = (eDirection)RND->getFromIntTo(eUP, eDirection_None);
	_collision = {   _x - GRUZZER_WIDTH/ 2.f, _y - GRUZZER_HEIGHT
					,_x + GRUZZER_WIDTH/ 2.f, _y};

	_isAlive = true;

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
	D2DMANAGER->drawRectangle(_collision, false);
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

	_collision = {	 _x - GRUZZER_WIDTH / 2.f, _y - GRUZZER_HEIGHT
					,_x + GRUZZER_WIDTH / 2.f, _y };

	
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

void gruzzer::dead()
{
}
