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
	return S_OK;
}

void baseScene::release()
{
}

void baseScene::update()
{
}

void baseScene::render()
{
}

void baseScene::initUI()
{
}