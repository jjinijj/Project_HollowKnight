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
	_aniUID = UI_UID;
	return S_OK;
}

void uiManager::release()
{
	iterUiMap iter = _uiMap.begin();

	for (iter; _uiMap.end() != iter;)
	{
		uiBase* ui = (*iter).second;
		iter = _uiMap.erase(iter);

		SAFE_RELEASE(ui);
		SAFE_DELETE(ui);
	}

	_uiMap.clear();
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
