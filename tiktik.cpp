#include "stdafx.h"
#include "tiktik.h"
#include "ObjectManager.h"

HRESULT tiktik::init(POINTF position, unsigned int uid)
{
	enemy::init(position, uid);

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("tiktik_move");
		anim->init(img, true, 0, img->GetMaxFrameX(), 10, _dir);
		_animMap.insert(make_pair(eMOVE_ON, anim));

		_imgSize.x = img->GetFrameWidth();
		_imgSize.y = img->GetFrameWidth();
		_imgSizeHalf.x = img->GetFrameWidth() / 2;
		_imgSizeHalf.y = img->GetFrameWidth() / 2;
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("tiktik_move_under");
		anim->init(img, true, 0, img->GetMaxFrameX(), 10, _dir);
		_animMap.insert(make_pair(eMOVE_UNDER, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("tiktik_move_sideup");
		anim->init(img, true, 0, img->GetMaxFrameX(), 10, _dir);
		_animMap.insert(make_pair(eMOVE_SIDE_UP, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("tiktik_move_sidedown");
		anim->init(img, true, 0, img->GetMaxFrameX(), 10, _dir);
		_animMap.insert(make_pair(eMOVE_SIDE_DOWN, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("tiktik_climbup_stu");
		anim->init(img, false, 0, img->GetMaxFrameX(), 10, _dir);
		_animMap.insert(make_pair(eCLIMB_SIDE_TO_ON, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("tiktik_climbup_uts");
		anim->init(img, false, 0, img->GetMaxFrameX(), 10, _dir);
		_animMap.insert(make_pair(eCLIME_UNDER_TO_SIDE, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("tiktik_climbdown_stu");
		anim->init(img, false, 0, img->GetMaxFrameX(), 10, _dir);
		_animMap.insert(make_pair(eCLIMB_SIDE_TO_UNDER, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("tiktik_climbdown_ots");
		anim->init(img, false, 0, img->GetMaxFrameX(), 10, _dir);
		_animMap.insert(make_pair(eCLIME_ON_TO_SIDE, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("tiktik_dead");
		anim->init(img, false, 0, img->GetMaxFrameX(), 20, _dir);
		_animMap.insert(make_pair(eDEAD, anim));
	}

	_colSize.x = _imgSize.x - 20;
	_colSize.y = _imgSize.y - 20;
	_anim = _animMap[eMOVE_ON];
	_anim->start();
	_hp = 2;
	_speed = 1;

	_type = eENEMY_TIKTIK;
	_collision = {   (int)_position.x - _colSize.x / 2, (int)_position.y - _colSize.y
					,(int)_position.x + _colSize.x / 2, (int)_position.y};
	setActiveArea();

	return S_OK;
}

void tiktik::update()
{
	enemy::update();

	if (_isAlive)
		move();
}

void tiktik::move()
{
	if(nullptr == _activeArea )
		return;

	switch ( _state )
	{
		case eMOVE_ON:
		{
			bool isChange = false;
			if ( eRIGHT == _dir )
			{
				_position.x += _speed;
				isChange =  ( _activeArea->getCollision().right < _position.x );
			}
			else
			{
				_position.x -= _speed;
				isChange =  ( _position.x < _activeArea->getCollision().left );
			}

			if(isChange)
			{
				changeState(eCLIME_ON_TO_SIDE);
				_dirUD = eDOWN;
			}

			break;
		}

		case eMOVE_UNDER:
		{
			bool isChange = false;
			if ( eRIGHT == _dir )
			{
				_position.x += _speed;
				isChange = _activeArea->getCollision().right < _position.x;
			}
			else
			{
				_position.x -= _speed;
				isChange = _position.x < _activeArea->getCollision().left;
			}

			if ( isChange )
			{
				changeState(eCLIME_UNDER_TO_SIDE);
				_dirUD = eUP;
			}

			break;
		}


		case eMOVE_SIDE_UP:
		{
			bool isChange = false;

			_position.y -= _speed;
			isChange =  _position.y < _activeArea->getCollision().top + (_colSize.y / 2);

			if ( isChange )
			{
				if ( eRIGHT == _dir )
					_dir = eLEFT;
				else
					_dir = eRIGHT;

				changeState(eCLIMB_SIDE_TO_ON);
			}

			break;
		}

		case eMOVE_SIDE_DOWN:
		{
			bool isChange = false;
			_position.y += _speed;
			isChange = _activeArea->getCollision().bottom + (_colSize.y / 2) < _position.y;

			if ( isChange )
			{
				if ( eRIGHT == _dir )
					_dir = eLEFT;
				else
					_dir = eRIGHT;

				changeState(eCLIMB_SIDE_TO_UNDER);
			}

			break;
		}

		case eCLIMB_SIDE_TO_ON:
		{
			if(eRIGHT == _dir)
				_position.x += _speed;
			else
				_position.x -= _speed;

			_position.y -= _speed;


			if ( _anim )
				if ( !_anim->IsPlayingAnimation() )
					changeState(eMOVE_ON);
			break;
		}

		case eCLIMB_SIDE_TO_UNDER:
		{
			if(eRIGHT == _dir)
				_position.x += _speed;
			else
				_position.x -= _speed;

			_position.y += _speed;

			if( _anim )
				if(!_anim->IsPlayingAnimation())
					changeState(eMOVE_UNDER);
			break;
		}

		case eCLIME_ON_TO_SIDE:
		{
			if(eRIGHT == _dir)
				_position.x += _speed;
			else
				_position.x -= _speed;

			_position.y += _speed;

			if ( _anim )
				if ( !_anim->IsPlayingAnimation() )
					changeState(eMOVE_SIDE_DOWN);

			break;
		}

		case eCLIME_UNDER_TO_SIDE:
		{
			if(eRIGHT == _dir)
				_position.x += _speed;
			else
				_position.x -= _speed;

			_position.y -= _speed;

			if( _anim )
				if(!_anim->IsPlayingAnimation())
					changeState(eMOVE_SIDE_UP);
			break;
		}
	}
}

void tiktik::dead()
{
	enemy::dead();
	changeState(eDEAD);
}

void tiktik::setActiveArea()
{
	if(nullptr == _objM )
		return;

	lObject* objList = _objM->getObjectList(eOBJECT_GROUND);

	if(objList->size() == 0)
		return;

	bool isFloating = true;
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

		int offsetY = _collision.bottom - objCol.top;
		_position.y -= offsetY;

		_activeArea = obj;

		break;
	}
}