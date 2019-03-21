#include "stdafx.h"
#include "mawlek.h"
#include "player.h"
#include "ObjectManager.h"

HRESULT mawlek::init(POINTF position, unsigned int uid)
{
	enemy::init(position, uid);

	// 몸
	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("mawlek_body");
		anim->init(img, true, 0, 1, 5, _dir);
		_animMap.insert(make_pair(eBODY_IDLE, anim));


		_imgSize.x = img->GetFrameWidth();
		_imgSize.y = img->GetFrameWidth();
		_imgSizeHalf.x = img->GetFrameWidth() / 2;
		_imgSizeHalf.y = img->GetFrameWidth() / 2;

		_colSize.x = img->GetFrameWidth();
		_colSize.y = img->GetFrameHeight();
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("mawlek_body");
		anim->init(img, true, 0, 2, 5, _dir + 2);
		_animMap.insert(make_pair(eBODY_MOVE, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("mawlek_body");
		anim->init(img, false, 0, img->GetMaxFrameX(), 5, _dir + 4);
		anim->SetEventFrameX(img->GetMaxFrameX());
		_animMap.insert(make_pair(eBODY_ATTACK, anim));
	}

	// 팔
	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("mawlek_arms");
		anim->init(img, true, 0, img->GetMaxFrameX(), 5, eRIGHT);
		_animMap_armR.insert(make_pair(eARM_IDLE, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("mawlek_arms");
		anim->init(img, true, 0, img->GetMaxFrameX(), 5, eRIGHT + 2);
		_animMap_armR.insert(make_pair(eARM_MOVE, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("mawlek_arms");
		anim->init(img, false, 0, 3, 5, eRIGHT + 4);
		anim->SetEventFrameX(1);
		_animMap_armR.insert(make_pair(eARM_ATTACK, anim));
	}
	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("mawlek_arms");
		anim->init(img, true, 0, img->GetMaxFrameX(), 5, eLEFT);
		_animMap_armL.insert(make_pair(eARM_IDLE, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("mawlek_arms");
		anim->init(img, true, 0, img->GetMaxFrameX(), 5, eLEFT + 2);
		_animMap_armL.insert(make_pair(eARM_MOVE, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("mawlek_arms");
		anim->init(img, false, 0, 3, 5, eLEFT + 4);
		anim->SetEventFrameX(1);
		_animMap_armL.insert(make_pair(eARM_ATTACK, anim));
	}

	// 머리
	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("mawlek_head");
		anim->init(img, true, 0, img->GetMaxFrameX(), 5, _dir);
		_animMap_head.insert(make_pair(eHEAD_IDLE, anim));
	}

	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("mawlek_head");
		anim->init(img, false, 0, img->GetMaxFrameX(), 5, _dir + 2);
		anim->SetEventFrameX(1);
		_animMap_head.insert(make_pair(eHEAD_ATTAK, anim));
	}

	_anim = _animMap[eBODY_IDLE];
	_anim->start();

	_anim_armL = _animMap_armL[eARM_IDLE];
	_anim_armL->start();
	_anim_armR = _animMap_armR[eARM_IDLE];
	_anim_armR->start();

	_anim_head = _animMap_head[eHEAD_IDLE];
	_anim_head->start();

	_state = eBODY_IDLE;
	_armState = eARM_IDLE;
	_headState = eHEAD_IDLE;
	_mawlekState = eIDLE;

	_hp = 10;
	_speed = 1;
	_time = 0;

	_type = eENEMY_MAWLEK;;

	_dirUD = eDIRECTION_NONE;

	_collision = {   (int)_position.x - _colSize.x / 2, (int)_position.y - _colSize.y
					,(int)_position.x + _colSize.x / 2, (int)_position.y};

	return S_OK;
}

void mawlek::update()
{
	enemy::update();

	if (eDEAD == _mawlekState)
	{
		_dir = (eDIRECTION)(eLEFT - _dir);
		_anim->SetFrameY(eBODY_ATTACK * 2 + _dir);
		return;
	}

	// 애니메이션
	if ( eATTACK3 != _mawlekState)
	{
		if(_anim_armL)
			_anim_armL->update();
		if(_anim_armR)
			_anim_armR->update();
		if(_anim_head )
			_anim_head->update();
	}

	// 이동
	move();

	// 다음 행동 결정 텀
	if (_time < NEXT_STATE_TERM)
	{
		++_time;
	}
	else
	{
		_time = 0;
		// 행동
		if (eIDLE == _mawlekState)
		{
			// 행동 확률
			int value = RND->getInt(100);
			if (value < 50)
				changeMawlekState(eMOVE);
		}
		else if (eMOVE == _mawlekState)
		{
			POINTF targetPos = { _target->getPositionX(), _target->getPositionY() };
			POINTF pos = MakePointF( _position.x, _position.y );

			// 행동 확률
			int value = RND->getInt(100);

			if (CheckInRange(pos, targetPos, ATTACK_RANGE))
			{
				if( value < 50)
					changeMawlekState(eATTACK1);
			}
			else
			{
				if (value < 50)
					changeMawlekState(eATTACK2);
			}

			// 멀리있음? 그러면 무더기로 발사
			if (!CheckInRange(pos, targetPos, ATTACK_RANGE_FAR))
			{
				if (value < 50)
					changeMawlekState(eATTACK3);
			}
		}
	}

	if ( eATTACK1 == _mawlekState )
	{
		if ( _anim_armL->IsEventFrame() && !_anim_armL->isDoEvent())
		{
			attackSide(eLEFT);
			_anim_armL->SetEventFlag(true);
			if ( !CheckIntersectRect(_collisionAtk, _target->getCollisionRECT()) )
			{
				_target->takeDamage();
			}
		}
		if ( _anim_armR->IsEventFrame() && !_anim_armR->isDoEvent() )
		{
			attackSide(eRIGHT);
			_anim_armR->SetEventFlag(true);
			if ( !CheckIntersectRect(_collisionAtk, _target->getCollisionRECT()) )
			{
				_target->takeDamage();
			}
		}

		if ( !_anim_armL->IsPlayingAnimation() && !_anim_armR->IsPlayingAnimation() )
		{
			changeMawlekState(eMOVE);
		}
	}
	else if ( eATTACK2 == _mawlekState )
	{
		

		if ( !_anim_head->IsPlayingAnimation())
		{
			changeMawlekState(eMOVE);
		}
	}
	else if ( eATTACK3 == _mawlekState )
	{


		if ( !_anim->IsPlayingAnimation())
		{
			changeMawlekState(eIDLE);
		}
	}
}

void mawlek::release()
{
	enemy::release();

	for (_ianiMap = _animMap_armL.begin(); _animMap_armL.end() != _ianiMap; ++_ianiMap)
		(*_ianiMap).second->release();
	for (_ianiMap = _animMap_armR.begin(); _animMap_armR.end() != _ianiMap; ++_ianiMap)
		(*_ianiMap).second->release();
	for (_ianiMap = _animMap_head.begin(); _animMap_head.end() != _ianiMap; ++_ianiMap)
		(*_ianiMap).second->release();

	_animMap_armL.clear();
	_animMap_armR.clear();
	_animMap_head.clear();

	_target = nullptr;
}

void mawlek::render()
{
	if ( eATTACK3 != _mawlekState && eDEAD != _mawlekState )
	{
		if ( _anim_armL )
			_anim_armL->render(_collision.left - 110, _collision.top + 20);
		if ( _anim_armR )
			_anim_armR->render(_collision.right - 100, _collision.top + 20);
		if ( _anim_head )
			_anim_head->render(_position.x - 70, _collision.top + 30);
	}

	D2DMANAGER->drawRectangle( D2DMANAGER->_defaultBrush
							  , (float)_collisionAtk.left,  (float)_collisionAtk.top
							  , (float)_collisionAtk.right, (float)_collisionAtk.bottom);

	enemy::render();
}

void mawlek::dead()
{
	enemy::dead();
	changeMawlekState(eDEAD);

}

void mawlek::move()
{
	if( eIDLE == _mawlekState || eATTACK3 == _mawlekState || eDEAD == _mawlekState)
		return;

	if(eRIGHT == _dir )
		_position.x += _speed;
	else
		_position.x -= _speed;

	_collision = {   (int)_position.x - _colSize.x / 2, (int)_position.y - _colSize.y
					,(int)_position.x + _colSize.x / 2, (int)_position.y};


	if(nullptr == _objM)
		return;

	lObject* objList = _objM->getObjectList(eOBJECT_GROUND);

	if(objList->size() == 0)
		return;

	int offsetX = 0;
	gameObject* obj = nullptr;
	RECT objCol = {};
	ilObject end = objList->end();

	for ( ilObject iter = objList->begin(); end != iter; ++iter )
	{
		obj = ( *iter );
		objCol = ( obj->getCollision() );

		if ( !CheckIntersectRect(_collision, objCol) )
			continue;

		offsetX = GetIntersectOffsetX_doNotBoard(_collision, objCol);
		_position.x += offsetX;

		_anim->SetFrameY(2 * _state + _dir);
		_anim_head->SetFrameY(2 * _headState + _dir);

		_dir = (eDIRECTION)(eLEFT - _dir);
	}
}

bool mawlek::isFire()
{
	if (_mawlekState == eATTACK2)
	{
		if (_anim_head->IsEventFrame() && !_anim_head->isDoEvent())
		{
			return true;
		}
	}
	else if (_mawlekState == eATTACK3)
	{
		if (_anim->IsEventFrame() && !_anim->isDoEvent())
		{
			return true;
		}
	}
	else if (_mawlekState == eDEAD)
	{
		if (_anim->IsEventFrame())
		{
			return true;
		}
	}

	return false;
}

void mawlek::bulletFire()
{
	if (_mawlekState == eATTACK2)
	{
		if (_anim_head->IsEventFrame() && !_anim_head->isDoEvent())
		{
			_anim_head->SetEventFlag(true);
		}
	}
	else if (_mawlekState == eATTACK3)
	{
		if (_anim->IsEventFrame() && !_anim->isDoEvent())
		{
			_anim->SetEventFlag(true);
		}
	}
}

void mawlek::changeArmState(eARMSTATE state)
{
	if( _armState == state )
		return;

	if( state < 0 )
		return;

	if ( _animMap_armL.find(state) == _animMap_armL.end() )
		return;

	if(nullptr != _anim_armL )
		_anim_armL->end();
	if(nullptr != _anim_armR )
		_anim_armR->end();

	_armState = state;
	_anim_armL = _animMap_armL[state];
	_anim_armL->SetFrameY(2 * state + eLEFT);
	_anim_armL->start();

	_anim_armR = _animMap_armR[state];
	_anim_armR->SetFrameY(2 * state + eRIGHT);
	_anim_armR->start();
}

void mawlek::changeHeadState(eHEADSTATE state)
{
	if( _headState == state )
		return;

	if( state < 0 )
		return;

	if ( _animMap_head.find(state) == _animMap_head.end() )
		return;

	if(nullptr != _anim_head )
		_anim_head->end();

	_headState = state;
	_anim_head = _animMap_head[state];
	_anim_head->SetFrameY(_dir + state * 2);
	_anim_head->start();
}

void mawlek::changeBodyState(eBODYSTATE state)
{
	if (_state == state)
		return;

	if (state < 0)
		return;

	if (_animMap.find(state) == _animMap.end())
		return;

	if (nullptr != _anim)
		_anim->end();

	_state = state;
	_anim = _animMap[state];
	_anim->SetFrameY(_dir + state * 2);
	_anim->start();
}

void mawlek::changeMawlekState(eSTATE state)
{
	switch (state)
	{
		case mawlek::eIDLE:
		{
			changeBodyState(eBODY_IDLE);
			changeArmState(eARM_IDLE);
			changeHeadState(eHEAD_IDLE);

			_mawlekState = eIDLE;

			break;
		}

		case mawlek::eMOVE:
		{
			changeArmState((eARMSTATE)RND->getInt(eARM_MOVE));
			changeHeadState(eHEAD_IDLE);
			changeBodyState(eBODY_MOVE);
			
			_mawlekState = eMOVE;

			break;
		}

		case mawlek::eATTACK1:
		{
			_mawlekState = eATTACK1;
			changeArmState(eARM_ATTACK);
			break;
		}

		case mawlek::eATTACK2:
		{
			_mawlekState = eATTACK2;
			changeArmState((eARMSTATE)RND->getInt(eARM_MOVE));
			changeHeadState(eHEAD_ATTAK);
			break;
		}

		case mawlek::eATTACK3:
		{
			_mawlekState = eATTACK3;
			changeBodyState(eBODY_ATTACK);
			break;
		}

		case mawlek::eDEAD:
		{
			_anim = _animMap[eBODY_ATTACK];
			_mawlekState = eDEAD;
			_anim->start();
			break;
		}
	}

}

void mawlek::attackSide(eDIRECTION dir)
{
	if ( eLEFT == dir )
	{
		_collisionAtk = { _collision.left - 110, _collision.top + 20
						, _collision.left, _position.y - 20};
	}
	else
	{
		_collisionAtk = { _collision.right, _collision.top + 20
						, _collision.right + 100, _position.y - 20};
	}
}
