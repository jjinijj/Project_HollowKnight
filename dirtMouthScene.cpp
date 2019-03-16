#include "stdafx.h"
#include "dirtMouthScene.h"


dirtMouthScene::dirtMouthScene()
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

	return S_OK;
}

void dirtMouthScene::release()
{
}

void dirtMouthScene::update()
{
}

void dirtMouthScene::render()
{
}
