#include "stdafx.h"
#include "baseInGameScene.h"
#include "mapData.h"
#include "actorManager.h"


baseInGameScene::baseInGameScene()
{
}


baseInGameScene::~baseInGameScene()
{
}

HRESULT baseInGameScene::init()
{
	baseScene::init();

	_mapData = new mapData;
	_mapData->init();
	_mapData->load(SCENEMANAGER->getSceneFileName(_sceneName));

	_actorM = new actorManager;
	_actorM->init();
	_actorM->mapDataLink(_mapData);

	return S_OK;

}

void baseInGameScene::release()
{
	baseScene::release();

	SAFE_RELEASE(_mapData);
	SAFE_RELEASE(_actorM);

	SAFE_DELETE(_mapData);
	SAFE_DELETE(_actorM);
}

void baseInGameScene::update()
{
	baseScene::update();
	_actorM->update();
}

void baseInGameScene::render()
{
	baseScene::render();
	_mapData->render();
	_actorM->render();
}
