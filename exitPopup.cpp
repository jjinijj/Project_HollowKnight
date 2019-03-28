#include "stdafx.h"
#include "exitPopup.h"
#include "animation.h"


exitPopup::exitPopup()
{
}


exitPopup::~exitPopup()
{
}

HRESULT exitPopup::init()
{
	_background = IMAGEMANAGER->findImage("bg");
	_cursor = IMAGEMANAGER->findImage("menuCursor");

	ANIMANAGER->addArrayFrameAnimation(MENU_CURSOR_UID, 0, "menuCursor", 0, _cursor->GetMaxFrameX(), 60, false);
	_cursorAni = ANIMANAGER->findAnimation(MENU_CURSOR_UID, 0);

	_question = IMAGEMANAGER->findImage("qText");
	_menu[0] = IMAGEMANAGER->findImage("yesText");
	_menu[1] = IMAGEMANAGER->findImage("noText");

	
	_isExit = false;
	_cursorAni->start();

	return S_OK;
}

void exitPopup::release()
{
	_background = nullptr;
	_menu[0] = nullptr;
	_menu[1] = nullptr;
	_cursor = nullptr;
	_cursorAni = nullptr;
}

void exitPopup::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_UP) || KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		_isExit = !_isExit;
		_cursorAni->start();
		SOUNDMANAGER->play("ui_change_selection");
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		if (_isExit)
			SCENEMANAGER->setNextScene(eSceneName_Title);
		uiClose();
	}

	_cursorAni->frameUpdate(TIMEMANAGER->getElapsedTime());
}

void exitPopup::render()
{
	_background->render(0.f, 0.f, WINSIZEX, WINSIZEY, 1.f, true);

	_question->render(WINSIZEX / 2.f - _question->GetWidth() / 2.f, WINSIZEY / 2.f - _question->GetHeight(), 1.0f, true);

	_menu[0]->render(WINSIZEX / 2.f - _menu[0]->GetWidth() / 2.f, WINSIZEY / 2.f + 100.f, _menu[0]->GetWidth(), _menu[0]->GetHeight());
	_menu[1]->render(WINSIZEX / 2.f - _menu[1]->GetWidth() / 2.f, WINSIZEY / 2.f + 210.f, _menu[1]->GetWidth(), _menu[1]->GetHeight());

	_cursor->aniRender(WINSIZEX / 2.f + _menu[!_isExit]->GetWidth() / 2.f + 10.f
					   , WINSIZEY / 2.f + 100.f + (!_isExit * 110.f) - 10.f
					   , _cursorAni
					   , 1.0f, true);
	_cursor->aniRenderReverseX(WINSIZEX / 2.f - _menu[!_isExit]->GetWidth() / 2.f - _cursor->GetFrameWidth() - 10.f
							   , WINSIZEY / 2.f + 100.f + (!_isExit * 110.f) - 10.f
							   , _cursorAni
							   , 1.0f, true);
}

void exitPopup::uiOpen()
{
	_isOpen = true;
	_isExit = false;
	_cursorAni->start();
}

void exitPopup::uiClose()
{
	_isOpen = false;
	_isExit = false;
	_cursorAni->stop();
}
