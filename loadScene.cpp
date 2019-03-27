#include "stdafx.h"
#include "loadScene.h"
#include "animation.h"

loadScene::loadScene()
{
}


loadScene::~loadScene()
{
}

HRESULT loadScene::init()
{
	_sceneName = eSceneName_Load;
	_ani = ANIMANAGER->findAnimation(LOADING_UID, 0);
	_ani->start();

	SOUNDMANAGER->setBgmPause();

	_time = 0.f;
	return S_OK;
}

void loadScene::release()
{
	_ani = nullptr;
}

void loadScene::render()
{
	IMAGEMANAGER->findImage("bg")->render(0.f, 0.f, WINSIZEX, WINSIZEY, 1.0f, true);
	IMAGEMANAGER->findImage("knight_loading")->aniRender(WINSIZEX - 100.f, WINSIZEY - 200.f, _ani, 1.0f, true);
}

void loadScene::update()
{
	if (3.f <= _time)
	{
		if(!SOUNDMANAGER->isPlaySound("ui_button_confirm"))
			SCENEMANAGER->changeNextScene();
	}
	else
	{
		_ani->frameUpdate(TIMEMANAGER->getElapsedTime());
		_time += TIMEMANAGER->getElapsedTime();

		if(3.f <= _time)
			SOUNDMANAGER->play("ui_button_confirm");
	}
}