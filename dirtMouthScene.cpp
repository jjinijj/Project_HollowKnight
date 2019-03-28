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
	_sceneName = eSceneName_DirtMouth;
	baseInGameScene::init();

	return S_OK;
}

void dirtMouthScene::release()
{
	baseInGameScene::release();
}

void dirtMouthScene::update()
{
	baseInGameScene::update();
}

void dirtMouthScene::render()
{
	baseInGameScene::render();
}

void dirtMouthScene::startScene()
{
	baseInGameScene::startScene();
	SOUNDMANAGER->play("Dirtmouth");
}

void dirtMouthScene::initUI()
{
	baseInGameScene::initUI();
}
