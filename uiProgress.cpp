#include "stdafx.h"
#include "uiProgress.h"


uiProgress::uiProgress()
: _front(nullptr)
, _back(nullptr)
, _isFrontSizeFix(false)
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
	_back->render(_rc, 1.0f, true);
	_front->render(_frontRc, 1.0f, true);

	uiObject::render();
}

void uiProgress::setScrollSize(float size)
{
}

void uiProgress::setValue(float value)
{
}
