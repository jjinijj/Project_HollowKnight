#include "stdafx.h"
#include "mapToolScene.h"

#include "mapTool.h"
#include "uiButton.h"

mapToolScene::mapToolScene()
{
}


mapToolScene::~mapToolScene()
{
}

HRESULT mapToolScene::init()
{
	baseScene::init();
	_sceneName = eSceneName_MapTool;

	// ����ȭ������ ���������� ����
	_isInGameScene = true;
	
	mapTool* tool = new mapTool;
	tool->init();
	UIMANAGER->insertUI(eUI_MapTool, tool);
	tool->uiOpen();
	tool->setActive(true);

	DEVTOOL->clearDebugMode();
	DEVTOOL->setDebugMode(DEBUG_SHOW_RECT);
	DEVTOOL->setDebugMode(DEBUG_SHOW_TEXT);
	DEVTOOL->setDebugMode(DEBUG_SHOW_UID);
	DEVTOOL->setDebugMode(DEBUG_SHOW_POSITION);

	return S_OK;
}

void mapToolScene::release()
{
}

void mapToolScene::update()
{
}

void mapToolScene::render()
{
}

