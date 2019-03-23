#include "stdafx.h"
#include "bullet.h"

HRESULT bullet::init()
{
	return S_OK;
}

HRESULT bullet::init(POINTF pos, float angle, float speed, float radius, const char* moveimgName, const char* pangimgName)
{
	{
		if ( nullptr != moveimgName )
		{
			animation* anim = new animation;
			image* img = IMAGEMANAGER->findImage(moveimgName);
			anim->init(img, true, 0, img->GetMaxFrameX(), 5, 0);

			_anim_move = anim;
		}
	}

	{
		if ( nullptr != pangimgName )
		{
			animation* anim = new animation;
			image* img = IMAGEMANAGER->findImage(pangimgName);
			anim->init(img, false, 0, img->GetMaxFrameX(), 5, 0);

			_anim_pang = anim;
		}
	}

	_position = pos;
	_isAppear = true;
	_speed = speed;
	_radius = radius;
	_angle = angle;

	_anim = _anim_move;
	_anim->start();

	_state = eMOVE;
	_type = eBULLET_NONE;

	return S_OK;
}

void bullet::release()
{
	clear();
}

void bullet::update()
{
	if( ePANG == _state )
	{
		if ( _anim )
		{
			if ( !_anim->IsPlayingAnimation() )
				_isAppear = false;
		}
		else 
			_isAppear = false;
	}

	if(_anim )
		_anim->update();
}

void bullet::render()
{
	if (_isDebugMode)
	{
		RECT collision = {	  (int)(_colPos.x - _radius)
							, (int)(_colPos.y - _radius)
							, (int)(_colPos.x + _radius)
							, (int)(_colPos.y + _radius) };

		D2DMANAGER->drawRectangle(D2DMANAGER->_defaultBrush
								  , (float)collision.left, (float)collision.top
								  , (float)collision.right, (float)collision.bottom);
	}

	if(_anim )
		_anim->render(_position.x, _position.y);
}

void bullet::move()
{
}

void bullet::clear()
{
	if ( _anim_move )
	{
		SAFE_RELEASE(_anim_move);
		SAFE_DELETE(_anim_move);
	}

	if ( _anim_pang )
	{
		SAFE_RELEASE(_anim_pang);
		SAFE_DELETE(_anim_pang);
	}

	if ( _anim != nullptr )
	{
		SAFE_RELEASE(_anim);
		SAFE_DELETE(_anim);
	}

	_position = {};
	_angle = 0.f;
	_speed = 0.f;
	_radius = 0.f;
	_type = eBULLET_NONE;
	_state = eNONE;
	_isAppear = false;
}

void bullet::hitSomething()
{
	if ( _anim_pang )
	{
		_anim->end();
		_anim = _anim_pang;
		_anim->start();

		_state = ePANG;
	}
}






HRESULT linearBullet::init(POINTF pos, float angle, float speed, float radius, const char* moveimgName, const char* pangimgName)
{
	bullet::init(pos, angle, speed, radius, moveimgName, pangimgName);
	_type = eLINEARBULLET;

	return S_OK;
}

void linearBullet::update()
{
	bullet::update();
	move();
}

void linearBullet::move()
{
	if(eMOVE != _state )
		return;

	float x = cosf(_angle) * _speed;
	float y = sinf(_angle) * _speed;

	_position.x += x;
	_position.y += y;

	_colPos.x += x;
	_colPos.y += y;

	bullet::move();
}






HRESULT arcBullet::init(POINTF pos, float angle, float speed, float radius, const char* moveimgName, const char* pangimgName)
{
	bullet::init(pos, angle, speed, radius, moveimgName, pangimgName);
	_type = eARCBULLET;

	_startPos = pos;
	_gravity = 0.5f;
	return S_OK;
}

void arcBullet::update()
{
	bullet::update();

	_time += 0.5f;
	move();
}

void arcBullet::move()
{
	if(eMOVE != _state )
		return;

	_position.x = _startPos.x +  -cosf(_angle) * _time * _speed * 2;
	_position.y = _startPos.y - (sinf(_angle) * _time *( _speed * 2 ) - (float)(_gravity * pow(_time, 2)));
}

void arcBullet::clear()
{
	bullet::clear();

	_startPos = {};
	_time = 0.f;
	_gravity = 0.f;
}
