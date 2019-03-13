#include "stdafx.h"
#include "uiText.h"

uiText::uiText()
: _x(0.f)
, _y(0.f)
, _fontSize(DEFAULT_FONT_SIZE)
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
}

void uiText::render()
{
	D2DMANAGER->drawText(_text.c_str(), _x, _y, _fontSize, _fontColor, true);
}
