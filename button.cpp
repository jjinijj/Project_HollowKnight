#include "stdafx.h"
#include "button.h"


button::button()
: _state(eButton_None)
, _isToggle(false)
, _isToggleOn(false)
, _fontSize(10)
, _width(0.f)
, _height(0.f)
, _x(0.f)
, _y(0.f)
{
	for (int ii = 0; ii < eButton_Count; ++ii)
	{
		_imgs[ii] = nullptr;
		_text[ii].clear();
	}

	_rc = {};

	_onFunction = NULL;
	_offFunction = NULL;
}

button::~button()
{
}

HRESULT button::init( const char* upImgName, const char* pressImgName
					 , float destX, float destY
					 , float width, float height
					 , function<void(void)> func)
{
	
	_isToggle = false;
	_state = eButton_Up;

	_imgs[eButton_Up] = IMAGEMANAGER->findImage(upImgName);
	_imgs[eButton_Press] = IMAGEMANAGER->findImage(pressImgName);

	_x = destX;
	_y = destY;
	_width = width;
	_height = height;
	_rc = { destX, destY, destX + width, destY + height };

	_onFunction = std::move(func);

	return S_OK;
}

HRESULT button::init( const char* upImgName, const char* pressImgName, const char* downImgName
					 , float destX, float destY
					 , float width, float height
					 , function<void(void)> toggleOnFunc
					 , function<void(void)> toggleOffFunc)
{

	init(upImgName, pressImgName, destX, destY, width, height, toggleOnFunc);

	_isToggle = true;
	_isToggleOn = false;
	_imgs[eButton_Down] = IMAGEMANAGER->findImage(downImgName);

	_offFunction = std::move(toggleOffFunc);

	return S_OK;
}

void button::release()
{
	for (int ii = 0; ii < eButton_Count; ++ii)
	{
		_imgs[ii] = nullptr;
		_text[ii].clear();
	}

	_onFunction = NULL;
	_offFunction = NULL;
}

void button::update()
{
	if (PtInRectD2D(_rc, _ptMouse))
	{
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			_state = eButton_Press;
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON) && eButton_Press == _state)
		{
			_state = eButton_Up;
			if (_isToggle)
			{
				if (_isToggleOn)
				{
					_offFunction();
					_isToggleOn = false;
				}
				else
				{
					_onFunction();
					_isToggleOn = true;
					_state = eButton_Down;
				}
			}
			else
			{
				_onFunction();
			}
		}
	}
	else
	{
		if (_isToggle)
		{
			if(_isToggleOn)
				_state = eButton_Down;
			else
				_state = eButton_Up;
		}
		else
		{
			_state = eButton_Up;
		}
	}
}

void button::render()
{
	_imgs[_state]->render(_rc, 1.0f);

	//WCHAR str[128] ;
	//swprintf_s(str, L"%d %d", _state, _isToggleOn);
	D2DMANAGER->drawText(_text[_state].c_str()
						 ,_rc.left
						 ,_rc.top + (_height - _fontSize) / 2.5f
						 ,_fontSize, RGB(255, 255, 255));
}

void button::setText(wstring text)
{
	for(int ii = 0 ; ii < eButton_Count; ++ii)
		_text[ii] = text;
}

void button::setText(wstring text, int fontSize)
{
	setText(text);
	_fontSize = fontSize;
}

void button::setText(wstring text, eButtonState state)
{
	_text[state] = text;
}

void button::setText(wstring text, eButtonState state, int fontSize)
{
	_text[state] = text;
	_fontSize = fontSize;
}

void button::setState(eButtonState state)
{
	_state = state;

	if (_isToggle)
	{
		if(eButton_Up == state)
			_isToggleOn = false;
		else if(eButton_Down == state)
			_isToggleOn = true;
	}
}

void button::rePosition(float x, float y)
{
	_x = x;
	_y = y;

	_rc = {_x, _y, _x + _width, _y + _height};
}

void button::reSize(float width, float height)
{
	_width = width;
	_height = height;

	_rc = { _x, _y, _x + _width, _y + _height};
}
