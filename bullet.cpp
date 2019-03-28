#include "stdafx.h"
#include "bullet.h"
#include "bulletState.h"

HRESULT bullet::init()
{
	return S_OK;
}

HRESULT bullet::init( UINT uid
					 ,float x, float y
					 ,float angle, float speed, float radius
					 ,const char* moveimgName, const char* pangimgName)
{
	actorBase::init(uid, x, y);
	_width = radius;
	_height = radius;
	_colWidth = radius;
	_colHeight = radius;
	_lifeTime = 0.f;

	_type = eActor_Bullet;

	{
		image* img = IMAGEMANAGER->findImage(moveimgName);
		ANIMANAGER->addArrayFrameAnimation( _uid, eMOVE, moveimgName
										   , 0, img->GetMaxFrameX(), 5, true);
	}

	{
		image* img = IMAGEMANAGER->findImage(pangimgName);
		ANIMANAGER->addArrayFrameAnimation( _uid, ePANG, pangimgName
										   , 0, img->GetMaxFrameX(), 5, false);
	}

	_isAppear = true;
	_speed = speed;
	_radius = radius;
	_angle = angle;

	_colPos = {_x, _y};

	_dir = (angle < PI / 2) ? eRIGHT : eLEFT;

	return S_OK;
}

void bullet::release()
{
	actorBase::release();
	clear();
}

void bullet::update()
{
	actorBase::update();
	updateCollision();
	_lifeTime += TIMEMANAGER->getElapsedTime();
}

void bullet::render()
{
	actorBase::render();

	//if (_isDebugMode)
	//{
	//	RECT collision = {	  (int)(_colPos.x - _radius)
	//						, (int)(_colPos.y - _radius)
	//						, (int)(_colPos.x + _radius)
	//						, (int)(_colPos.y + _radius) };
	//
	//	D2DMANAGER->drawRectangle(D2DMANAGER->_defaultBrush
	//							  , (float)collision.left, (float)collision.top
	//							  , (float)collision.right, (float)collision.bottom);
	//}
}

ACTORPACK* bullet::makePack()
{
	return nullptr;
}

void bullet::loadPack(ACTORPACK * pack)
{
}

void bullet::move()
{

}

void bullet::clear()
{
	_x = 0.f;
	_y = 0.f;
	_angle = 0.f;
	_speed = 0.f;
	_radius = 0.f;
	_isAppear = false;
	_lifeTime = 0.f;

	SAFE_RELEASE(_state);
	SAFE_RELEASE(_nextState);

	SAFE_DELETE(_state);
	SAFE_DELETE(_nextState);
}

void bullet::hitSomething()
{
	bulletState* bs = new bulletPang;
	bs->init(this);
	
	changeState(bs);
}

bool bullet::isPang()
{
	return (_state->getState() == ePANG);
}

void bullet::updateCollision()
{
	_collision = {_colPos.x - _colWidth, _colPos.y - _colHeight
				 ,_colPos.x + _colWidth, _colPos.y + _colHeight};
	
	_rc =	{_x - _width, _y - _height
			,_x + _width, _y + _height};
}





HRESULT linearBullet::init( UINT uid
						   ,float x, float y
						   ,float angle, float speed, float radius
						   ,const char* moveimgName, const char* pangimgName)
{
	bullet::init(uid, x, y, angle, speed, radius, moveimgName, pangimgName);
	_subType = eLINEARBULLET;

	bulletState* bs = new bulletMove;
	bs->init(this);

	_state = bs;
	_state->start();

	return S_OK;
}

void linearBullet::update()
{
	bullet::update();
}

void linearBullet::move()
{
	float x = cosf(_angle) * _speed;
	float y = sinf(_angle) * _speed;

	_x += x;
	_y += y;

	_colPos.x += x;
	_colPos.y += y;

	bullet::move();
}







HRESULT arcBullet::init( UINT uid
						,float x, float y
						,float angle, float speed, float radius
						,const char* moveimgName, const char* pangimgName)
{
	bullet::init(uid, x, y, angle, speed, radius, moveimgName, pangimgName);
	_subType = eARCBULLET;

	_startPos = {x, y};
	_gravity = 0.5f;

	bulletState* bs = new bulletMove;
	bs->init(this);

	_state = bs;
	_state->start();

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
	_x = _startPos.x +  -cosf(_angle) * _time * _speed * 2;
	_y = _startPos.y - (sinf(_angle) * _time *( _speed * 2 ) - (float)(_gravity * pow(_time, 2)));
}

void arcBullet::clear()
{
	bullet::clear();

	_startPos = {};
	_time = 0.f;
	_gravity = 0.f;
}
