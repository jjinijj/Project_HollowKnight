#include "stdafx.h"
#include "titleScene.h"
#include "uiButton.h"
#include "animation.h"


titleScene::titleScene()
: _isInputEnterKey(false)
, _selIdx(0)
, _nextSceneNameName(eSceneName_None)
, _alpha(0.f)
, _back(nullptr)
, _bg(nullptr)
{
	_menu[0] = nullptr;
	_menu[1] = nullptr;
	_menu[2] = nullptr;
}


titleScene::~titleScene()
{
}

HRESULT titleScene::init()
{
	SOUNDMANAGER->play("Title");
	_back = IMAGEMANAGER->findImage("bg");
	_bg = IMAGEMANAGER->findImage("title_");
	_start = IMAGEMANAGER->findImage("startText");
	_cursor = IMAGEMANAGER->findImage("menuCursor");

	ANIMANAGER->addArrayFrameAnimation(MENU_CURSOR_UID, 0, "menuCursor", 0, _cursor->GetMaxFrameX(), 60, false);
	_cursorAni = ANIMANAGER->findAnimation(MENU_CURSOR_UID, 0);

	_selIdx = 0;

	_menu[0] = IMAGEMANAGER->findImage("startText");
	_menu[1] = IMAGEMANAGER->findImage("mapToolText");
	_menu[2] = IMAGEMANAGER->findImage("exitText");
	
	_cursorAni->start();

	_isInputEnterKey = false;
	_nextSceneNameName = eSceneName_None;
	_alpha = 0.f;

	return S_OK;
}

void titleScene::release()
{
	_back = nullptr;
	_bg = nullptr;
	_menu[0] = nullptr;
	_menu[1] = nullptr;
	_menu[2] = nullptr;
}

void titleScene::update()
{
	if (!_isInputEnterKey)
	{
		if (1.f <= _alpha)
		{
			_alpha = 1.f;
			
			if (!_isInputEnterKey)
			{
				_cursorAni->frameUpdate(TIMEMANAGER->getElapsedTime());
				if(KEYMANAGER->isOnceKeyDown(VK_RETURN))
				{
					_isInputEnterKey = true;
					SOUNDMANAGER->play("ui_button_confirm");

					if (0 == _selIdx)
						//_nextSceneNameName = eSceneName_DirtMouth;
						_nextSceneNameName = eSceneName_CrossLoad_01;
					else if(1 == _selIdx)
						_nextSceneNameName = eSceneName_MapTool;
					else
						PostQuitMessage(0);
				}
				else if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
				{
					++_selIdx;
					if (2 < _selIdx)
						_selIdx = 0;
					_cursorAni->start();
					SOUNDMANAGER->play("ui_change_selection");
				}
				else if(KEYMANAGER->isOnceKeyDown(VK_UP))
				{
					--_selIdx;
					if(_selIdx < 0)
						_selIdx = 2;
					_cursorAni->start();
					SOUNDMANAGER->play("ui_change_selection");
				}
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
			SCENEMANAGER->setNextScene(_nextSceneNameName);
		}
	}
}

void titleScene::render()
{
	IMAGEMANAGER->findImage("titleBg")->render(0.f, 0.f, WINSIZEX, WINSIZEY, 1.0f, true);
	_bg->render(WINSIZEX / 2.f - _bg->GetWidth() / 2.f, WINSIZEY / 2.f - _bg->GetHeight() + 50.f, _alpha, true);

	if(1.0f <= _alpha) 
	{
		_menu[0]->render(WINSIZEX / 2.f - _menu[0]->GetWidth() / 2.f, WINSIZEY / 2.f + 100.f, _menu[0]->GetWidth(), _menu[0]->GetHeight());
		_menu[1]->render(WINSIZEX / 2.f - _menu[1]->GetWidth() / 2.f, WINSIZEY / 2.f + 210.f, _menu[1]->GetWidth(), _menu[1]->GetHeight());
		_menu[2]->render(WINSIZEX / 2.f - _menu[2]->GetWidth() / 2.f, WINSIZEY / 2.f + 320.f, _menu[2]->GetWidth(), _menu[2]->GetHeight());

		_cursor->aniRender(	WINSIZEX / 2.f + _menu[_selIdx]->GetWidth() / 2.f + 10.f
						   ,WINSIZEY / 2.f + 100.f + (_selIdx * 110.f) - 10.f
						   ,_cursorAni
						   ,1.0f, true);
		_cursor->aniRenderReverseX(  WINSIZEX / 2.f - _menu[_selIdx]->GetWidth() / 2.f - _cursor->GetFrameWidth() - 10.f
									,WINSIZEY / 2.f + 100.f + (_selIdx * 110.f) - 10.f
									,_cursorAni
									,1.0f, true);
	}
}

void titleScene::initUI()
{
	baseScene::initUI();
}
