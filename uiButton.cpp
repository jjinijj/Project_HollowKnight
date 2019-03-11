#include "stdafx.h"
#include "uiButton.h"


uiButton::uiButton()
: _state(eButton_None)
, _isToggle(false)
, _isToggleOn(false)
, _fontSize(10)

{
	for (int ii = 0; ii < eButton_Count; ++ii)
	{
		_imgs[ii] = nullptr;
		_text[ii].clear();
	}

	_onFunction = NULL;
	_pressFunction = NULL;
	_offFunction = NULL;
}

uiButton::~uiButton()
{
}

HRESULT uiButton::init( const char* upImgName, const char* pressImgName
					 , float destX, float destY
					 , float width, float height)
{
	
	uiObject::init(destX, destY, width, height);

	_isToggle = false;
	_state = eButton_Up;

	_imgs[eButton_Up] = IMAGEMANAGER->findImage(upImgName);
	_imgs[eButton_Press] = IMAGEMANAGER->findImage(pressImgName);
	
	return S_OK;
}

HRESULT uiButton::init( const char* upImgName, const char* pressImgName, const char* downImgName
					 , float destX, float destY
					 , float width, float height)
{

	init(upImgName, pressImgName, destX, destY, width, height);

	_isToggle = true;
	_isToggleOn = false;
	_imgs[eButton_Down] = IMAGEMANAGER->findImage(downImgName);

	return S_OK;
}

void uiButton::release()
{
	for (int ii = 0; ii < eButton_Count; ++ii)
	{
		_imgs[ii] = nullptr;
		_text[ii].clear();
	}

	_onFunction = NULL;
	_pressFunction = NULL;
	_offFunction = NULL;
}

void uiButton::update()
{
	uiObject::update();
}

void uiButton::render()
{
	_imgs[_state]->render(_rc, 1.0f);

	//WCHAR str[128] ;
	//swprintf_s(str, L"%d %d", _state, _isToggleOn);
	D2DMANAGER->drawText(_text[_state].c_str()
						 ,_rc.left
						 ,_rc.top + (_height - _fontSize) / 2.5f
						 ,_fontSize, RGB(255, 255, 255));
}

void uiButton::setText(wstring text)
{
	for(int ii = 0 ; ii < eButton_Count; ++ii)
		_text[ii] = text;
}

void uiButton::setText(wstring text, int fontSize)
{
	setText(text);
	_fontSize = fontSize;
}

void uiButton::setText(wstring text, eButtonState state)
{
	_text[state] = text;
}

void uiButton::setText(wstring text, eButtonState state, int fontSize)
{
	_text[state] = text;
	_fontSize = fontSize;
}

void uiButton::setOnClickFunction(function<void(void)> func)
{
	_onFunction = std::move(func);
}

void uiButton::sePressFunction(function<void(void)> func)
{
	_pressFunction = std::move(func);
}

void uiButton::setOnClickUPFunction(function<void(void)> func)
{
	_offFunction = std::move(func);
}

void uiButton::setState(eButtonState state)
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

void uiButton::reSize(float width, float height)
{
	setWidth(width);
	setHeight(height);

	_rc = { _x, _y, _x + _width, _y + _height};
}

void uiButton::releaseInputKey()
{
	if (_isToggle)
	{
		if (_isToggleOn)
			_state = eButton_Down;
		else
			_state = eButton_Up;
	}
	else
	{
		_state = eButton_Up;
	}
}

void uiButton::onceKeyDownMouseL()
{
	_state = eButton_Press;
}

void uiButton::onceKeyUpMouseL()
{
	if(eButton_Press != _state)
		return;
	
	_state = eButton_Up;
	if (_isToggle)
	{
		if (_isToggleOn)
		{
			_isToggleOn = false;
			if(_offFunction )
				_offFunction();
		}
		else
		{
			_isToggleOn = true;
			if(_onFunction )
				_onFunction();
			_state = eButton_Down;
		}
	}
	else
	{
		if( _onFunction )
			_onFunction();
	}
}

void uiButton::stayKeyMouseL()
{
	if( _pressFunction )
		_pressFunction();
}

void uiButton::onceKeyDownMouseR()
{
}

void uiButton::onceKeyUpMouseR()
{
}

void uiButton::stayKeyMouseR()
{
}
