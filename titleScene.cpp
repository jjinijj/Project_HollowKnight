#include "stdafx.h"
#include "titleScene.h"


titleScene::titleScene()
{
}


titleScene::~titleScene()
{
}

HRESULT titleScene::init()
{
	return S_OK;
}

void titleScene::release()
{
}

void titleScene::update()
{
	if(KEYMANAGER->isOnceKeyDown(VK_RETURN))
		SCENEMANAGER->changeScene(eSceneName_DirtMouth);
		//SCENEMANAGER->changeScene(eSceneName_MapTool);
}

void titleScene::render()
{
}
