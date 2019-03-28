#include "stdafx.h"
#include "uiPanel.h"
#include "iTween.h"


uiPanel::uiPanel()
: _img(nullptr)
, _itween(nullptr)
{
	_onFunction = NULL;
	_pressFunction = NULL;
	_offFunction = NULL;
	_hoverFunction = NULL;
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
	if(!_isActive)
		return;
	
	uiObject::update();

	if(_itween )
		_itween->update();
	

	if ( PtInRectD2D(_rc, _ptMouse) )
	{
		if( _hoverFunction && !KEYMANAGER->isStayKeyDown(VK_LBUTTON))
			_hoverFunction();
	}

	//int width = GetSystemMetrics(SM_CXSCREEN);
	//int height = GetSystemMetrics(SM_CYSCREEN);
}

void uiPanel::render()
{
	if(!_isViewing)
		return;

	//D2DMANAGER->drawRectangle(_rc);
	if(_img)
		_img->render(_rc, 1.f, true);

	uiObject::render();
}

void uiPanel::changeImage(image* img)
{
	_img = img;
	if( _img )
	{
		_width =  static_cast<float>(_img->GetWidth());
		_height = static_cast<float>(_img->GetHeight());
	}
}

void uiPanel::resize()
{
	POINTF pf = getWorldPosition();
	_rc = {pf.x, pf.y, pf.x + _width, pf.y + _height};
}

void uiPanel::setOnClickFunction(function<void(void)> func)
{
	_onFunction = std::move(func);
}

void uiPanel::setPressFunction(function<void(void)> func)
{
	_pressFunction = std::move(func);
}

void uiPanel::setOnClickUPFunction(function<void(void)> func)
{
	_offFunction = std::move(func);
}

void uiPanel::setHoverFunction(function<void(void)> func)
{
	_hoverFunction = std::move(func);
}

void uiPanel::onceKeyDownMouseL()
{
	if ( _onFunction )
		_onFunction();   
}

void uiPanel::onceKeyUpMouseL()
{
	if (_offFunction )
		_offFunction();
}

void uiPanel::stayKeyMouseL()
{
	if( _pressFunction )
		_pressFunction();
}

void uiPanel::onceKeyDownMouseR()
{
}

void uiPanel::onceKeyUpMouseR()
{
}

void uiPanel::stayKeyMouseR()
{
}


