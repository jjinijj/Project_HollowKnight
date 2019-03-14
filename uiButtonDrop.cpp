#include "stdafx.h"
#include "uiButtonDrop.h"


uiButtonDrop::uiButtonDrop()
{
}


uiButtonDrop::~uiButtonDrop()
{
}

HRESULT uiButtonDrop::init(const char* upImgName, const char* pressImgName, float destX, float destY, float width, float height)
{
	uiButton::init( upImgName, pressImgName
				   ,destX, destY, width, height);
	return S_OK;
}

HRESULT uiButtonDrop::init(const char* upImgName, const char* pressImgName, const char * downImgName, float destX, float destY, float width, float height)
{
	uiButton::init(upImgName, pressImgName, downImgName
				   ,destX, destY
				   ,width, height);

	return S_OK;
}

void uiButtonDrop::release()
{
	uiButton::release();
}

void uiButtonDrop::update()
{
	uiButton::update();
}

void uiButtonDrop::render()
{
	uiButton::render();
}

void uiButtonDrop::insertChild(uiObject* ui)
{
	uiObject::insertChild(ui);

	float height = _height;
	for (int ii = 0; ii < _childCount; ++ii)
	{
		_childs[ii]->setLocalPosition(0.f, height);
		height += _childs[ii]->getHeight();
	}
}

void uiButtonDrop::onceKeyDownMouseL()
{
	uiButton::onceKeyDownMouseL();
	if (eButton_Down != _state)
	{
		for (int ii = 0; ii < _childCount; ++ii)
		{
			_childs[ii]->setViewing(true);
			_childs[ii]->setActive(true);
		}
	}
}

void uiButtonDrop::onceKeyUpMouseL()
{
	uiButton::onceKeyUpMouseL();
	if (eButton_Up == _state)
	{
		for (int ii = 0; ii < _childCount; ++ii)
		{
			_childs[ii]->setViewing(false);
			_childs[ii]->setActive(false);
		}
	}
}
