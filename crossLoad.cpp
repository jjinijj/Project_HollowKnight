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

	SOUNDMANAGER->play("Crossroad");


	PLAYER->setPositionY(500.f);
	return S_OK;
}

void crossLoad::release()
{
	
}

void crossLoad::update()
{
	baseInGameScene::update();
}

void crossLoad::render()
{
	baseInGameScene::render();
}
