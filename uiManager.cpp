#include "stdafx.h"
#include "uiManager.h"


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

	while (iter != end)
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

	while (iter != end)
	{
		uiBase* ui = (*iter).second;
		if(ui->isOpen())
			(*iter).second->render();
	}
}

void uiManager::insetUI(UINT uid, uiBase * ui)
{
}

uiBase* uiManager::getUI(UINT uid)
{
	return nullptr;
}
