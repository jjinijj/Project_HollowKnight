#include "stdafx.h"
#include "uiText.h"

uiText::uiText()
: _fontSize(DEFAULT_FONT_SIZE)
, _fontColor(DEFAULT_FONT_COLOR)
, _fontType(d2dManager::eFontType_None)
{
	_text.clear();
}

uiText::~uiText()
{
}

HRESULT uiText::init(float x, float y)
{
	uiObject::init();
	_x = x;
	_y = y;

	return S_OK;
}

void uiText::release()
{
	_text.clear();
}

void uiText::update()
{
	uiObject::update();
}

void uiText::render()
{
	D2DMANAGER->drawText(_text.c_str()
						 , getWorldPosition().x, getWorldPosition().y
						 , _fontSize, _fontColor, true);
}
