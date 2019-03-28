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
		uiObject::init(x, y
					   , static_cast<float>(img->GetWidth())
					   , static_cast<float>(img->GetHeight()));
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
	if(!_isActive)
		return;

	_worldPosition = getWorldPosition();
}

void uiImage::render()
{
	if(!_isViewing)
		return;

	_img->render(_worldPosition.x, _worldPosition.y, 1.f, true);
	uiObject::render();
}
