#include "stdafx.h"
#include "uiPanel.h"


uiPanel::uiPanel()
: _img(nullptr)
{
}

uiPanel::~uiPanel()
{
}

HRESULT uiPanel::init(float x, float y, float width, float height, image* img)
{
	uiObject::init(x, y, width, height);
	_img = img;

	return S_OK;
}

void uiPanel::release()
{
	_img = nullptr;
}

void uiPanel::update()
{
	uiObject::update();
}

void uiPanel::render()
{
	D2DMANAGER->drawRectangle(_rc);
	if(_img)
		_img->render(_x, _y, 1.f);

	uiObject::render();
}

void uiPanel::changeImage(image * img)
{
}

void uiPanel::resize()
{
}

void uiPanel::setOnClickFunction(function<void(void)> func)
{
}

void uiPanel::setPressFunction(function<void(void)> func)
{
}

void uiPanel::setOnClickUPFunction(function<void(void)> func)
{
}


