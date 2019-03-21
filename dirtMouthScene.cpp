#include "stdafx.h"
#include "dirtMouthScene.h"
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
	_sceneName = eSceneName_DirtMouth;
	baseInGameScene::init();
	_isInGameScene = true;

	_player = new player;
	_player->init( WINSIZEX / 2.f, WINSIZEY / 2.f);

	_player->mapDataLink(_mapData);

	DEVTOOL->setDebugMode(DEBUG_SHOW_TEXT);
	DEVTOOL->setDebugMode(DEBUG_SHOW_RECT);

	return S_OK;
}

void dirtMouthScene::release()
{
	SAFE_RELEASE(_player);
	
	SAFE_DELETE(_player);
}

void dirtMouthScene::update()
{
	baseInGameScene::update();
	_player->update();
}

void dirtMouthScene::render()
{
	baseInGameScene::render();
	_player->render();
}
