#include "stdafx.h"
#include "uiImage.h"

uiImage::uiImage()
 : _img(nullptr)
{
}


uiImage::~uiImage()
{
}


HRESULT uiImage::init(float x, float y, image* img)
{
	if ( img )
	{
		uiObject::init(x, y, img->GetWidth(), img->GetHeight());
		_img = img;
		_width = 0.f;
		_height = 0.f;
		_rc = {};
	}

	return S_OK;
}

void uiImage::release()
{
	_img = nullptr;
}

void uiImage::update()
{
	_worldPosition = getWorldPosition();
}

void uiImage::render()
{
	_img->render(_worldPosition.x, _worldPosition.y, 1.f);
	uiObject::render();
}
