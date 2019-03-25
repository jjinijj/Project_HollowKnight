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
	SOUNDMANAGER->play("Title");
	_back = IMAGEMANAGER->findImage("bg");
	_bg = IMAGEMANAGER->findImage("title_");
	return S_OK;
}

void titleScene::release()
{
}

void titleScene::update()
{
	if (!_isInputEnterKey)
	{
		if (1.f <= _alpha)
		{
			_alpha = 1.f;
			if (KEYMANAGER->isOnceKeyDown(VK_RETURN) )
			{
				_isInputEnterKey = true;
				//SCENEMANAGER->changeScene(eSceneName_DirtMouth);
				//SCENEMANAGER->changeScene(eSceneName_MapTool);
				//SCENEMANAGER->changeScene(eSceneName_Load);

				//SCENEMANAGER->setNextScene(eSceneName_DirtMouth);
			}
		}
		else
			_alpha += 0.01f;
	}
	else
	{
		_alpha -= 0.01f;
		if (_alpha < 0.f)
		{
			SCENEMANAGER->setNextScene(eSceneName_MapTool);
		}
	}
}

void titleScene::render()
{
	//_back->render(0.f, 0.f, WINSIZEX, WINSIZEY, 1.0f, true);
	IMAGEMANAGER->findImage("titleBg")->render(0.f, 0.f, WINSIZEX, WINSIZEY, 1.0f, true);
	_bg->render(WINSIZEX / 2.f - _bg->GetWidth() / 2.f, WINSIZEY / 2.f - _bg->GetHeight() / 2.f, _alpha, true);

	//D2DMANAGER->drawText(L"PRESS ENTER", WINSIZEX / 2.f - 100.f, WINSIZEY / 2.f, 100, RGB(255, 255, 255), true);
}

