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
	return E_NOTIMPL;
}

void uiBase::release()
{
}

void uiBase::update()
{
	if(!_isOpen || !_isActive)
		return;
}

void uiBase::render()
{
	if(!_isOpen)
		return;
}

void uiBase::uiOpen()
{
}

void uiBase::uiClose()
{
}
