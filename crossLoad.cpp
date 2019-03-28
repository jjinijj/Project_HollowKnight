#include "stdafx.h"
#include "crossLoad.h"


crossLoad::crossLoad()
{
}


crossLoad::~crossLoad()
{
}

HRESULT crossLoad::init()
{
	_sceneName = eSceneName_CrossLoad_01;
	baseInGameScene::init();

	


	//PLAYER->setPositionY(300.f);
	return S_OK;
}

void crossLoad::release()
{
	baseInGameScene::release();
}

void crossLoad::update()
{
	baseInGameScene::update();
}

void crossLoad::render()
{
	baseInGameScene::render();
}

void crossLoad::startScene()
{
	baseInGameScene::startScene();
	SOUNDMANAGER->play("Crossroad");
}

void crossLoad::initUI()
{
	baseInGameScene::initUI();
}
