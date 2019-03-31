#include "stdafx.h"
#include "baseScene.h"


baseScene::baseScene()
: _sceneName(eSceneName_None)
, _isInGameScene(false)
{
}


baseScene::~baseScene()
{
}

HRESULT baseScene::init()
{
	DEVTOOL->clearDebugMode();
	return S_OK;
}

void baseScene::release()
{
	SOUNDMANAGER->setAllSoundStop();
}

void baseScene::update()
{
}

void baseScene::render()
{
}

void baseScene::startScene()
{
	initUI();
}

void baseScene::initUI()
{
}

void baseScene::setSceneData(mapData * m, actorManager * am)
{
}
