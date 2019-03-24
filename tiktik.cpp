#include "stdafx.h"
#include "tiktik.h"
#include "mapData.h"
#include "terrain.h"
#include "tiktikState.h"

HRESULT tiktik::init(UINT uid, float x, float y)
{
	enemy::init(uid, x, y);
	_subType = eEnemy_Tiktik;
	_width = TIKTIK_WIDTH;
	_height = TIKTIK_HIEGHT;
	_colWidth = TIKTIK_WIDTH / 1.5f;
	_colHeight = TIKTIK_WIDTH / 1.5f;

	_name = L"tiktik";

	{
		image* img = IMAGEMANAGER->findImage("tiktik_move");
		ANIMANAGER->addArrayFrameAnimation( _uid, eMOVE_ON, "tiktik_move"
										   ,0, img->GetMaxFrameX()
										   ,TIKTIK_ANI_SPEED, true);
	}
	{
		image* img = IMAGEMANAGER->findImage("tiktik_move_under");
		ANIMANAGER->addArrayFrameAnimation(_uid, eMOVE_UNDER, "tiktik_move_under"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, true);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_move_sideup");
		ANIMANAGER->addArrayFrameAnimation(_uid, eMOVE_SIDE_UP, "tiktik_move_sideup"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, true);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_move_sidedown");
		ANIMANAGER->addArrayFrameAnimation(_uid, eMOVE_SIDE_DOWN, "tiktik_move_sidedown"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, true);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_climbup_stu");
		ANIMANAGER->addArrayFrameAnimation(_uid, eCLIMB_SIDE_TO_ON, "tiktik_climbup_stu"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, false);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_climbup_uts");
		ANIMANAGER->addArrayFrameAnimation(_uid, eCLIME_UNDER_TO_SIDE, "tiktik_climbup_uts"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, false);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_climbdown_stu");
		ANIMANAGER->addArrayFrameAnimation(_uid, eCLIMB_SIDE_TO_UNDER, "tiktik_climbdown_stu"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, false);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_climbdown_ots");
		ANIMANAGER->addArrayFrameAnimation(_uid, eCLIME_ON_TO_SIDE, "tiktik_climbdown_ots"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, false);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_dead");
		ANIMANAGER->addArrayFrameAnimation(_uid, eDEAD, "tiktik_dead"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, false);
	}

	_hp = TIKTIK_MAX_HP;
	_speed = static_cast<float>(TIKTIK_MOVE_SPEED);


	tiktikMove* state = new tiktikMoveOn;
	state->init(this);

	_state = state;
	_state->start();

	updateCollision();

	return S_OK;
}

void tiktik::update()
{
	enemy::update();
}

void tiktik::release()
{
	enemy::release();
	_area = nullptr;
}

void tiktik::render()
{
	enemy::render();
}

void tiktik::dead()
{
	enemy::dead();
	changeState(eDEAD);
}

ACTORPACK* tiktik::makePack()
{
	ACTORPACK* pack = enemy::makePack();
	if(pack)
	{
		pack->value = _areaUid;
	}

	return pack;
}

void tiktik::loadPack(ACTORPACK* pack)
{
	if(pack)
	{
		enemy::loadPack(pack);
		_areaUid = pack->value;
	}
}

void tiktik::mapDataLink(mapData* data)
{
	enemy::mapDataLink(data);

	if(NULL == _areaUid)
		return;

	terrain* ter = _mapData->getTerrain(_areaUid);
	if(ter)
	{
		_area = ter;
		_areaCol = _area->getCollision();
		fixPosition();
	}
}

void tiktik::moveOn()
{
	bool isChange = false;
	if ( eRIGHT == _dir )
	{
		_x += _speed;
		isChange =  ( _area->getCollision().right < _x );
	}
	else
	{
		_x -= _speed;
		isChange =  ( _x < _area->getCollision().left );
	}

	if(isChange)
	{
		changeState(eCLIME_ON_TO_SIDE);
		_dirUD = eDOWN;
	}
}

void tiktik::moveUnder()
{
	bool isChange = false;
	if ( eRIGHT == _dir )
	{
		_x += _speed;
		isChange = _area->getCollision().right < _x;
	}
	else
	{
		_x -= _speed;
		isChange = _x < _area->getCollision().left;
	}

	if ( isChange )
	{
		changeState(eCLIME_UNDER_TO_SIDE);
		_dirUD = eUP;
	}
}

void tiktik::moveSideUp()
{
	bool isChange = false;

	_y -= _speed;
	isChange =  _y < _area->getCollision().top + (_colHeight / 2);

	if ( isChange )
	{
		if ( eRIGHT == _dir )
			_dir = eLEFT;
		else
			_dir = eRIGHT;

		changeState(eCLIMB_SIDE_TO_ON);
	}
}

void tiktik::moveSiedDown()
{
	bool isChange = false;
	_y += _speed;
	isChange = _area->getCollision().bottom + (_colHeight / 2) < _y;

	if ( isChange )
	{
		if ( eRIGHT == _dir )
			_dir = eLEFT;
		else
			_dir = eRIGHT;

		changeState(eCLIMB_SIDE_TO_UNDER);
	}
}

void tiktik::climbSideToOn()
{
	if(eRIGHT == _dir)
		_x += _speed;
	else
		_x -= _speed;

	_y -= _speed;
}

void tiktik::climbSideToDown()
{
	if(eRIGHT == _dir)
		_x += _speed;
	else
		_x -= _speed;

	_y += _speed;
}

void tiktik::climbOnToSide()
{
	if(eRIGHT == _dir)
		_x += _speed;
	else
		_x -= _speed;

	_y += _speed;
}

void tiktik::climbUnderToSide()
{
	if(eRIGHT == _dir)
		_x += TIKTIK_MOVE_SPEED;
	else
		_x -= TIKTIK_MOVE_SPEED;

	_y -= TIKTIK_MOVE_SPEED;
}


//void tiktik::dead()
//{
//	enemy::dead();
//	changeState(eDEAD);
//}

void tiktik::changeState(eSTATE state)
{
	tiktikMove* tiktikState = nullptr;

	switch (state)
	{
		case eCLIMB_SIDE_TO_ON:		{ tiktikState = new tiktikClimbSideToOn;	tiktikState->init(this); break; }
		case eCLIMB_SIDE_TO_UNDER:	{ tiktikState = new tiktikClimbSideToDown;	tiktikState->init(this); break; }
		case eCLIME_ON_TO_SIDE:		{ tiktikState = new tiktikClimbOnToSide;	tiktikState->init(this); break; }
		case eCLIME_UNDER_TO_SIDE:	{ tiktikState = new tiktikClimbUnderToSide;	tiktikState->init(this); break; }
		case eDEAD:					{ tiktikState = new tiktikDead;				tiktikState->init(this); break; }			
			break;
	}


	enemy::changeState(tiktikState);
}

void tiktik::fixPosition()
{
	if (!CheckIntersectRect(_collision, _areaCol))
	{
		_y = _areaCol.top;
	}
	else
	{
		float offsetX = GetIntersectOffsetX_doNotBoard(_collision, _areaCol);
		float offsetY = GetIntersectOffsetY_doNotBoard(_collision, _areaCol);

		// ªÛ«œ
		if (_areaCol.left <= _collision.left && _collision.right < _areaCol.right )
		{
			_y += offsetY;
		} 
		// ¡¬øÏ
		else if ( _areaCol.top <= _collision.top && _collision.bottom <= _areaCol.bottom )
		{
			_x += offsetX;
		}
	}

	updateCollision();
}
