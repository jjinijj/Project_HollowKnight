#include "stdafx.h"
#include "dirtMouthScene.h"
#include "player.h"
#include "mapData.h"

dirtMouthScene::dirtMouthScene()
: _player(nullptr)
{
}


dirtMouthScene::~dirtMouthScene()
{
}

HRESULT dirtMouthScene::init()
{
	baseScene::init();
	_sceneName = eSceneName_DirtMouth;
	_isInGameScene = true;

	_player = new player;
	_player->init( WINSIZEX / 2.f, WINSIZEY / 2.f);

	_mapData = new mapData;
	_mapData->init();
	_mapData->load(SCENEMANAGER->getSceneFileName(_sceneName));

	_player->mapDataLink(_mapData);

	DEVTOOL->setDebugMode(DEBUG_SHOW_TEXT);
	DEVTOOL->setDebugMode(DEBUG_SHOW_RECT);

	return S_OK;
}

void dirtMouthScene::release()
{
	SAFE_RELEASE(_player);
	SAFE_RELEASE(_mapData);

	SAFE_DELETE(_mapData);
	SAFE_DELETE(_player);
}

void dirtMouthScene::update()
{
	_player->update();
}

void dirtMouthScene::render()
{
	_mapData->render();
	_player->render();
}
