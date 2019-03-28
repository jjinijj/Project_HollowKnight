#include "stdafx.h"
#include "dropObject.h"
#include "animation.h"


dropObject::dropObject()
{
}


dropObject::~dropObject()
{
}

HRESULT dropObject::init()
{
	return E_NOTIMPL;
}

HRESULT dropObject::init(const UID uid, const float destX, const float destY)
{
	return E_NOTIMPL;
}

void dropObject::release()
{
}

void dropObject::update()
{
	move();
	updateCollision();

	if(_ani)
		_ani->frameUpdate(TIMEMANAGER->getElapsedTime());
}

void dropObject::render()
{
	if(_img)
		_img->aniRender(_x, _y, _ani, 1.0f, false);
}

void dropObject::updateCollision()
{
	_rc = { _x, _y, _x + _width, _y + _height };
	_collision = { _x, _y, _x + _colWidth, _y + _colHeight };
}

void dropObject::move()
{
}

void dropObject::pushObject(float offsetX, float offsetY)
{

	_x += offsetX;
	_y += offsetY;
	
	updateCollision();
}

dropCoin::dropCoin()
{
}

dropCoin::~dropCoin()
{
}

HRESULT dropCoin::init()
{
	return S_OK;
}

HRESULT dropCoin::init(const UID uid)
{
	ANIMANAGER->addArrayFrameAnimation(uid, 0, "coin", 0, 3, 5, true);
	
	_uid = uid;
	_type = eDrop_Coin;

	return S_OK;
}

void dropCoin::release()
{
}

void dropCoin::update()
{
	dropObject::update();

}

void dropCoin::render()
{
	dropObject::render();
}

void dropCoin::start(float x, float y)
{
	_x = x;
	_y = y;

	_ani = ANIMANAGER->findAnimation(_uid, 0);
	_ani->start();
	assert(nullptr != _ani);
	
	_img = _ani->getImage();
	assert(nullptr != _img);

	_width = _img->GetFrameWidth();
	_height = _img->GetFrameHeight();

	_colWidth = _width;
	_colHeight = _height;

	_isMoveable = true;

	_vecX = RND->getFromIntTo(1, 5);
	_vecY = RND->getFromIntTo(5, 20);

	int value = RND->getInt(2);
	if(0 == value)
		_vecX *= -1;

	_gravity = 0.5f;
	_value = 1;
}

void dropCoin::move()
{
	if(!_isMoveable)
		return;
	_x += _vecX;
	_y -= _vecY;

	_vecY -= _gravity;
}

void dropCoin::clear()
{
}

void dropCoin::intersectWithObject(gameObject* obj)
{
	if(nullptr ==  obj)
		return;


	RECTD2D objCol = obj->getCollision();

	float offsetX = GetIntersectOffsetX(_collision, objCol);
	float offsetY = GetIntersectOffsetY(_collision, objCol);

	// 상하
	if (objCol.left <= _collision.left && _collision.right < objCol.right)
	{
		pushObject(0.f, offsetY);

		if (_y < obj->getPosY())
		{
			_y = objCol.top - _colHeight;
			_isMoveable = false;
		}
		else
		{
			_vecY = 0;
		}
	}
	// 좌우
	else if (objCol.top <= _collision.top && _collision.bottom <= objCol.bottom)
	{
		pushObject(offsetX, 0.f);
		pushObject(offsetX, 0.f);
		if (_x < obj->getPosX())
		{
			if( 0 < _vecX)
				_vecX *= -1;
		}
		else
		{
			if (_vecX < 0)
				_vecX *= -1;
		}
	}
	// 모서리
	else if (abs(offsetX) < abs(offsetY))
	{
		pushObject(offsetX, 0.f);
		pushObject(offsetX, 0.f);
		if (1 < abs(_vecY))
			_vecY -= 1;
		if (_x < obj->getPosX())
		{
			if (0 < _vecX)
				_vecX *= -1;
		}
		else
		{
			if (_vecX < 0)
				_vecX *= -1;
		}
	}
	// 모서리
	else
	{
		if (_y < obj->getPosY())
		{
			_isMoveable = false;
		}
		else
		{
			if (_vecY < 0)
				_vecY = 0;
		}
	}
}





dropItem::dropItem()
{
}

dropItem::~dropItem()
{
}

HRESULT dropItem::init()
{
	return E_NOTIMPL;
}

HRESULT dropItem::init(const UID uid, const float destX, const float destY)
{
	return E_NOTIMPL;
}

void dropItem::release()
{
}

void dropItem::update()
{
}

void dropItem::render()
{
}
