#include "stdafx.h"
#include "uiScroll.h"


uiScroll::uiScroll()
: _front(nullptr)
, _back(nullptr)
, _isHorizontal(true)
, _value(0.f)
{
	_frontRc = {};
}


uiScroll::~uiScroll()
{
}

HRESULT uiScroll::init(float x, float y, float width, float height, image* front, image* back)
{
	uiObject::init(x, y, width, height);

	_front = front;
	_back = back;

	if(!_front)
		_front = IMGDATABASE->getImage(eImage_UI_Scroll_Front_Default);
	if(!_back)
		_back = IMGDATABASE->getImage(eImage_UI_Scroll_Back_Default);

	_frontRc = _rc;
	_frontWidth = _frontRc.right - _frontRc.left;
	_frontHeight = _frontRc.bottom - _frontRc.top;

	return S_OK;
}

void uiScroll::release()
{
	_front = nullptr;
	_back = nullptr;
}

void uiScroll::update()
{
	if (PtInRectD2D(_frontRc, _ptMouse))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			_start = _ptMouse;
			_isPickingScroll = true;
		}
	}

	if(_isPickingScroll)
	{
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			_isPickingScroll = true;
			float dist = 0.f;
			if (_isHorizontal)
			{
				dist = _ptMouse.x - _start.x;
				_frontRc.left += dist;
				_frontRc.right += dist;

				if (_frontRc.left < _rc.left)
				{
					_frontRc.left = _rc.left;
					_frontRc.right = _frontRc.left + _frontWidth;
					_value = 0.f;
				}
				else if (_rc.right < _frontRc.right)
				{
					_frontRc.right = _rc.right;
					_frontRc.left = _frontRc.right - _frontWidth;
					_value = 1.f;
				}
				else
				{
					_value = (_frontRc.left - _rc.left) / (_width - _frontWidth);
				}
			}
			else
			{
				dist = _ptMouse.y - _start.y;
				_frontRc.top += dist;
				_frontRc.bottom += dist;
				if (_frontRc.top < _rc.top)
				{
					_frontRc.top = _rc.top;
					_frontRc.bottom = _frontRc.top + _frontHeight;
					_value = 0.f;
				}
				else if (_rc.bottom < _frontRc.bottom)
				{
					_frontRc.bottom = _rc.bottom;
					_frontRc.top = _frontRc.bottom - _frontHeight;
					_value = 1.f;
				}
				else
				{
					if(0.f < _height - _frontHeight)
						_value = (_frontRc.top - _rc.top) / (_height - _frontHeight);
				}
			}

			_start = _ptMouse;
		}
		else
		{
			_isPickingScroll = false;
		}
	}

	setValue(_value);

	if(!_isPickingScroll)
		uiObject::update();

	DEVTOOL->pushBackDebugText(format(L"scroll Value : %.2f", _value));
}

void uiScroll::render()
{
	_back->render(_rc, 1.f, true);
	_front->render(_frontRc, 1.f, true);
}

void uiScroll::setScrollSize(float size)
{
	_scrollSize = size;
	if (_isHorizontal)
		_frontWidth = _scrollSize;
	else
		_frontHeight = _scrollSize;
}

void uiScroll::setScrollSize()
{
	if (_parent)
	{
		if (_isHorizontal)
		{
			_frontWidth = _scrollSize;
		}
		else
		{
			_frontHeight = _scrollSize;
		}
	}
}

void uiScroll::setValue(float value)
{
	_value = value;
	if(value < 0.f)
		_value = 0.f;
	else if( 1.f < _value)
		_value = 1.f;

	POINTF pf = {};
	if (_isHorizontal)
	{
		pf.x = (value * (_width - _frontWidth)) + _rc.left + _frontWidth / 2.f;
		pf.y = (_height / 2.f ) + _rc.top;
	}
	else
	{
		pf.x = (_width / 2.f) + _rc.left;
		pf.y = (value * (_height - _frontHeight)) + _rc.top + _frontHeight / 2.f;
	}

	_frontRc = RectMakeCenter( pf.x, pf.y
							  ,_frontWidth
							  ,_frontHeight);

	
}

void uiScroll::onceKeyDownMouseL()
{

}

void uiScroll::onceKeyUpMouseL()
{
}

void uiScroll::stayKeyMouseL()
{
}

void uiScroll::onceKeyDownMouseR()
{
}

void uiScroll::onceKeyUpMouseR()
{
}

void uiScroll::stayKeyMouseR()
{
}
