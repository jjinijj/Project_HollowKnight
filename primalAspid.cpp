#include "stdafx.h"
#include "primalAspid.h"
#include "player.h"
#include "ObjectManager.h"
#include "bulletManager.h"

HRESULT primalAspid::init(POINTF position, unsigned int uid)
{
	enemy::init(position, uid);
	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("primalAspid_move");
		anim->init(img, true, 0, img->GetMaxFrameX(), 5, _dir);
		_animMap.insert(make_pair(eIDLE, anim));

		_imgSize.x = img->GetFrameWidth();
		_imgSize.y = img->GetFrameWidth();
		_imgSizeHalf.x = img->GetFrameWidth() / 2;
		_imgSizeHalf.y = img->GetFrameWidth() / 2;
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("primalAspid_attack");
		anim->init(img, false, 0, img->GetMaxFrameX(), 5, _dir);
		anim->SetEventFrameX(img->GetMaxFrameX());
		_animMap.insert(make_pair(eATTACK, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("primalAspid_dead");
		anim->init(img, false, 0, img->GetMaxFrameX(), 10, _dir);
		_animMap.insert(make_pair(eDEAD, anim));
	}

	_colSize.x = _imgSize.x - 50;
	_colSize.y = _imgSize.y - 50;
	_anim = _animMap[eIDLE];
	_anim->start();
	_hp = 3;
	_speed = 3;

	_type = eENEMY_PRIMALASPID;
	_dirUD = eDIRECTION_NONE;

	_collision = {   (int)_position.x - _colSize.x / 2, (int)_position.y - _colSize.y
					,(int)_position.x + _colSize.x / 2, (int)_position.y};


	return S_OK;
}

void primalAspid::update()
{
	enemy::update();
	move();
}

void primalAspid::move()
{
	if(nullptr == _target )
		return;


	// 따라가기
	if ( eMOVE_APPROACH == _state )
	{
		if ( CheckInRange(MakePointF(_target->getPositionX(), _target->getPositionY()), MakePointF(_position.x, _position.y), CLOSEST_RANGE) )
		{
			changeState(eATTACK);
		}
		else
		{
			_angle = atan2f(_target->getPositionY() - _position.y, _target->getPositionX() - _position.x);
			_position.x += cosf(_angle) * _speed;
			_position.y += sinf(_angle) * _speed;
		}
	}
	// 도망
	else if ( eMOVE_FARWAY == _state )
	{
		if ( !CheckInRange(MakePointF(_target->getPositionX(), _target->getPositionY()), MakePointF(_position.x, _position.y), SIGHT_RANGE) )
		{
			_state = eIDLE;
		}
		else
		{
			_angle = atan2f(_target->getPositionY() - _position.y, _target->getPositionX() - _position.x);
			_position.x -= cosf(_angle) * _speed;
			_position.y -= sinf(_angle) * _speed;
		}
	}
	// 대기
	else if(eIDLE == _state )
	{
		if ( CheckInRange(MakePointF(_target->getPositionX(), _target->getPositionY()), MakePointF(_position.x, _position.y), SIGHT_RANGE) )
		{
			_angle = atan2f(_target->getPositionY() - _position.y, _target->getPositionX() - _position.x);

			_position.x += cosf(_angle) * _speed;
			_position.y += sinf(_angle) * _speed;

			_state = eMOVE_APPROACH;
		}
	}
	// 공격
	else if ( eATTACK == _state )
	{
		if ( !_anim->IsPlayingAnimation() )
		{
			changeState(eIDLE);
			_state = eMOVE_FARWAY;
		}
	}

	_collision = {   (int)_position.x - _colSize.x / 2, (int)_position.y - _colSize.y
					,(int)_position.x + _colSize.x / 2, (int)_position.y};

	if(nullptr == _objM)
		return;

	lObject* objList = _objM->getObjectList(eOBJECT_GROUND);

	if(objList->size() == 0)
		return;

	int offsetX = 0;
	int offsetY = 0;
	gameObject* obj = nullptr;
	RECT objCol = {};
	ilObject end = objList->end();

	for ( ilObject iter = objList->begin(); end != iter; ++iter )
	{
		obj		= (*iter);
		objCol	= ( obj->getCollision() );

		if( !CheckIntersectRect(_collision, objCol) )
			continue;

		offsetX = GetIntersectOffsetX_doNotBoard(_collision, objCol);
		offsetY = GetIntersectOffsetY_doNotBoard(_collision, objCol);

		// 상하
		if ( objCol.left <= _collision.left && _collision.right < objCol.right )
		{
			_position.y += offsetY;
		} 
		// 좌우
		else if ( objCol.top <= _collision.top && _collision.bottom <= objCol.bottom )
		{
			_position.x += offsetX;
		}
		// 모서리
		else if ( abs(offsetX) < abs(offsetY) )
		{
			_position.x += offsetX;
		}
		// 모서리
		else
		{
			_position.y += offsetY;
		}
	}

	if(_target->getPositionX() < _position.x )
		_anim->SetFrameY(eLEFT);
	else 
		_anim->SetFrameY(eRIGHT);
}

void primalAspid::dead()
{
	enemy::dead();
	changeState(eDEAD);
}

bool primalAspid::isFire()
{
	if (eATTACK == _state)
	{
		if (_anim->IsEventFrame() && !_anim->isDoEvent())
		{
			return true;
		}
	}
	return false;
}

void primalAspid::bulletFire()
{
	if (eATTACK == _state)
	{
		if (_anim->IsEventFrame() && !_anim->isDoEvent())
		{
			_anim->SetEventFlag(true);
		}
	}
}

POINTF primalAspid::getBulletFirePoint()
{
	POINTF pof;

	if (eRIGHT == _dir)
		pof.x = _position.x + _colSize.x / 2;
	else
		pof.x = _position.x - _colSize.x / 2;

	pof.y = _position.y - _colSize.y / 2;

	return pof;
}
