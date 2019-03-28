#include "stdafx.h"
#include "uiProgress.h"


uiProgress::uiProgress()
: _front(nullptr)
, _back(nullptr)
, _isHorizontal(true)
, _value(0.f)
{
}


uiProgress::~uiProgress()
{
}

HRESULT uiProgress::init(float x, float y, float width, float height
						,image* front, image* back)
{
	uiObject::init(x, y, width, height);

	_front = front;
	_back = back;
	
	if(!_front)
		_front = IMAGEMANAGER->findImage("uiBG");

	if(!_back)
		_back = IMAGEMANAGER->findImage("uiBG2");

	return S_OK;
}

void uiProgress::release()
{
}

void uiProgress::update()
{
	uiObject::update();


	if(_isChanging)
	{
		_value += (TIMEMANAGER->getElapsedTime() * _dir * 0.1f);

		if (_dir < 0 && _value < _changeValue)
		{
			_value = _changeValue;
			_isChanging = false;
			_dir = 0;
		}
		else if (0 < _dir && _changeValue < _value)
		{
			_value = _changeValue;
			_isChanging = false;
			_dir = 0;
		}
	}

	if ( PtInRectD2D(_frontRc, _ptMouse) )
	{
		if ( KEYMANAGER->isStayKeyDown(VK_LBUTTON) )
		{

		}
	}
	else if ( PtInRectD2D(_rc, _ptMouse) )
	{
		if ( KEYMANAGER->isStayKeyDown(VK_LBUTTON) )
		{

		}
	}


}

void uiProgress::render()
{
	_back->render(_rc.left, _rc.top,  1.0f, true);
	
	if (_isHorizontal)
		_front->render(_rc.left, _rc.top, 0, 0, _value * _width, _height, 1.0f, true);
	else
	{
		float y = _height * _value;
		_front->render( _rc.left, _rc.top + (_height - y)
					   ,0, _height - y
					   ,_width, y, 1.0f, true);
	}
	

	uiObject::render();
}

void uiProgress::setScrollSize(float size)
{
}

void uiProgress::setValue(float value)
{
	_value = value;
}

void uiProgress::changeValue(float value)
{
	_isChanging = true;
	_changeValue = value;

	if(_changeValue < _value)
		_dir = -1;
	else
		_dir = 1;
}

void uiProgress::onceKeyDownMouseL()
{
}

void uiProgress::onceKeyUpMouseL()
{
}

void uiProgress::stayKeyMouseL()
{
}

void uiProgress::onceKeyDownMouseR()
{
}

void uiProgress::onceKeyUpMouseR()
{
}

void uiProgress::stayKeyMouseR()
{
}

