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

void gameObject::setPosition(float x, float y)
{
	_x = x;
	_y = y;
}
