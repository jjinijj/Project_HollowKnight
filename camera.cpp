#include "stdafx.h"
#include "camera.h"


camera::camera()
	: _x(0.f)
	, _y(0.f)
	, _width(0)
	, _height(0)
	, _snap(0.f)
{
	_scope = {};
}


camera::~camera()
{
}

HRESULT camera::init()
{
	RECTD2D rc = {0, 0, WINSIZEX, WINSIZEY};

	setViewSize(MAPSIZEX, MAPSIZEY);
	setScope(rc);
	setMoveSnap(10.f);

	_moveFunc = bind(&camera::move, this);

	return S_OK;
}

HRESULT camera::init(POINTFLOAT viewSize, RECTD2D scope, float snap)
{
	setViewSize(viewSize.x, viewSize.y);
	setScope(scope);
	setMoveSnap(snap);

	_moveFunc = bind(&camera::move, this);

	return S_OK;
}

HRESULT camera::init(POINTFLOAT viewSize, RECTD2D scope, float snap, float* targetX, float* targetY)
{
	setViewSize(viewSize.x, viewSize.y);
	setScope(scope);
	setMoveSnap(snap);

	_targetX = targetX;
	_targetY = targetY;

	_moveFunc = bind(&camera::moveTraceTarget, this);

	return S_OK;
}

void camera::release()
{
}

void camera::update()
{
	_moveFunc();


}

void camera::render()
{
}

void camera::setViewSize(float viewWidth, float viewHeight)
{
	_width = viewWidth;
	_height = viewHeight;

	fixCameroPos();
}

void camera::setScope(RECTD2D scope)
{
	_scope = scope;
	_scopeWidth = scope.right - scope.left;
	_scopeHeight = scope.bottom - scope.top;

	fixCameroPos();
}

bool camera::isRangeInCamera(float left, float top, int width, int height)
{
	float destX = left;
	float destY = top;
	getPosInCamera(&destX, &destY);

	if (_scope.right <= destX)			return false;
	if (destX + width <= _scope.left)	return false;
	if (_scope.bottom <= destY)			return false;
	if (destY + height <= _scope.top)	return false;

	return true;
}

void camera::getPosInCamera(float* x, float* y)
{
	*x = *x - _x;
	*y = *y - _y;
}

void camera::fixCameroPos()
{
	if (_x < 0)								_x = 0;
	else if (_width < _x + _scopeWidth)		_x = _width - _scopeWidth;
	if (_y < 0)								_y = 0;
	else if (_height < _y + _scopeHeight)	_y = _height - _scopeHeight;
}

void camera::move()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_x -= _snap;
		if (_x < 0.f)
			_x = 0.f;
	}

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_x += _snap;
		if (_width < _x + _scopeWidth)
			_x = _width - _scopeWidth;
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		_y -= _snap;
		if (_y < 0.f)
			_y = 0.f;
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		_y += _snap;
		if (_height < _y + _scopeHeight)
			_y = _height - _scopeHeight;
	}
}

void camera::moveTraceTarget()
{
	if (!_targetX || !_targetY)
		return;

	_x = *_targetX - (_scopeWidth / 2);
	_y = *_targetY - (_scopeHeight / 2);

	fixCameroPos();
}
