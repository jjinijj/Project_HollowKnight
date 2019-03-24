#include "stdafx.h"
#include "uiManager.h"
#include "windowDialog.h"


uiManager::uiManager()
{
}


uiManager::~uiManager()
{
}

HRESULT uiManager::init()
{
	return S_OK;
}

void uiManager::release()
{
}

void uiManager::update()
{
	iterUiMap iter = _uiMap.begin();
	iterUiMap end = _uiMap.end();

	for ( ; iter != end; ++iter)
	{
		uiBase* ui = (*iter).second;
		if (ui->isOpen() && ui->isActive())
			(*iter).second->update();
	}
}

void uiManager::render()
{
	iterUiMap iter = _uiMap.begin();
	iterUiMap end = _uiMap.end();

	for ( ; iter != end; ++iter)
	{
		uiBase* ui = (*iter).second;
		if(ui->isOpen())
			(*iter).second->render();
	}
}

void uiManager::insertUI(eUIType uid, uiBase* ui)
{
	if( _uiMap.find(uid) != _uiMap.end() )
		return;

	_uiMap.insert(make_pair(uid, ui));
}

uiBase* uiManager::getUI(eUIType uid)
{
	if( _uiMap.find(uid) == _uiMap.end() )
		return nullptr;

	return _uiMap[uid];
}
