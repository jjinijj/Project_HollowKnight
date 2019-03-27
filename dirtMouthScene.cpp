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

	//SOUNDMANAGER->addSound("Dirtmouth","sound/Dirtmouth.wav", true, true); 
	SOUNDMANAGER->play("Dirtmouth");

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

void dirtMouthScene::initUI()
{
	baseInGameScene::initUI();
}
