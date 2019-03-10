#include "stdafx.h"
#include "iTween.h"


iTween::iTween()
: _isMoving(false)
, _isLoop(false)
, _x(nullptr)
, _y(nullptr)
, _speed(0.f)
{
	_start = {};
	_end = {};
	_direct = {};
}

iTween::~iTween()
{
}

HRESULT iTween::init(POINTF start, POINTF end, float* x, float* y, float speed, bool isLoop)
{
	_start	= start; 
	_end	= end; 
	_x		= x;
	_y		= y; 
	_speed	= speed; 
	_isLoop = isLoop;

	if(_start.x < _end.x )
		_direct.x = 1.f;
	else if( _end.x < _start.x )
		_direct.x = -1.f;
	else
		_direct.x = 0.f;

	if( _start.y < _end.y )
		_direct.y = 1.f;
	else if( _end.y < _start.y )
		_direct.y = -1.f;
	else 
		_direct.y = 0.f;

	return S_OK;
}

void iTween::release()
{
	_x = nullptr;
	_y = nullptr;
}

void iTween::update()
{
}

void iTween::start()
{
	_isMoving = true;
}

void iTween::stop()
{
	_isMoving = false;
}

void iTween::reverse()
{
	swap(_start, _end);
	if(_start.x < _end.x )
		_direct.x = 1.f;
	else if( _end.x < _start.x )
		_direct.x = -1.f;
	else
		_direct.x = 0.f;

	if( _start.y < _end.y )
		_direct.y = 1.f;
	else if( _end.y < _start.y )
		_direct.y = -1.f;
	else 
		_direct.y = 0.f;
}


//=======================================================
// 선형 이동
//=======================================================
void iTween_Linear::update()
{
	if( !_isMoving )
		return;

	*_x += (_direct.x * _speed);
	*_y += (_direct.y * _speed);

	if ( _direct.x < 0.f )
	{
		if ( *_x <= _end.x )
		{
			*_x = _end.x;
			_isMoving = false;
		}
	}
	else if ( 0.f < _direct.x )
	{
		if ( _end.x <= *_x )
		{
			*_x = _end.x;
			_isMoving = false;
		}
	}

	if ( _direct.y < 0.f )
	{
		if ( *_y <= _end.y )
		{
			*_y = _end.y;
			_isMoving = false;
		}
	}
	else if ( 0.f < _direct.y )
	{
		if ( _end.y <= *_y )
		{
			*_y = _end.y;
			_isMoving = false;
		}
	}
}
