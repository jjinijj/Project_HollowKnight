#include "stdafx.h"
#include "uiSprite.h"
#include "animation.h"

uiSprite::uiSprite()
{
}


uiSprite::~uiSprite()
{
}

HRESULT uiSprite::init(float x, float y, const char* imgName)
{
	uiObject::init();

	_x = x;
	_y = y;

	_img = IMAGEMANAGER->findImage(imgName);
	assert(nullptr != _img);

	return S_OK;
}

void uiSprite::release()
{
	uiObject::release();
	_img = nullptr;
	_ani = nullptr;
}

void uiSprite::update()
{
	uiObject::update();
	if(_ani)
		_ani->frameUpdate(TIMEMANAGER->getElapsedTime());
}

void uiSprite::render()
{
	uiObject::render();
	if(_img && _ani)
		_img->aniRender(_x, _y, _ani, 1.0f, true);
}

void uiSprite::setAnimation(WORD aniKey)
{
	_aniKey = aniKey;
	if(_ani)
		_ani->stop();
	_ani = ANIMANAGER->findAnimation(_aniUID, aniKey);
	if(_ani)
		_ani->start();
}
