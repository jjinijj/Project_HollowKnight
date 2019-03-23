#include "stdafx.h"
#include "gameObject.h"


gameObject::gameObject()
: _uid(NULL)
, _x(0.f)
, _y(0.f)
{
	_collision = {};
	_rc = {};
}

gameObject::~gameObject()
{
}

HRESULT gameObject::init()
{
	return S_OK;
}

HRESULT gameObject::init(const UID uid, const float x, const float y)
{
	init();

	_uid = uid;
	_x = x;
	_y = y;

	return S_OK;
}

void gameObject::updateCollision()
{
	_collision = {   _x - _colWidth / 2.f, _y - _colHeight
					,_x + _colWidth / 2.f, _y};

	_rc = {  _x - _width / 2.f, _y - _height
			,_x + _width / 2.f, _y};
}

void gameObject::setPosition(float x, float y)
{
	_x = x;
	_y = y;
}
