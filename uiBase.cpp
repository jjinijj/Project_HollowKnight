#include "stdafx.h"
#include "uiBase.h"


uiBase::uiBase()
{
}


uiBase::~uiBase()
{
}

HRESULT uiBase::init()
{
	return S_OK;
}

void uiBase::release()
{
}

void uiBase::update()
{
	if(!_isOpen || !_isActive)
		return;

	list<uiObject*>::iterator iter = _uiList.begin();
	list<uiObject*>::iterator end = _uiList.end();
	for ( ; iter != end; ++iter)
		(*iter)->update();
}

void uiBase::render()
{
	if(!_isOpen)
		return;

	list<uiObject*>::reverse_iterator iter = _uiList.rbegin();
	list<uiObject*>::reverse_iterator end = _uiList.rend();
	for ( ; iter != end; ++iter)
		(*iter)->render();
}

void uiBase::uiOpen()
{
	_isOpen = true;
}

void uiBase::uiClose()
{
	_isOpen = false;
}

void uiBase::insertUIObject(uiObject* ui)
{
	list<uiObject*>::iterator iter = _uiList.begin();
	list<uiObject*>::iterator end = _uiList.end();
	for ( ; end != iter; ++iter )
	{
		if(*iter == ui)
			return;
	}

	_uiList.push_back(ui);
}
