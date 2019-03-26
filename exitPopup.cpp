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

	_menu[0] = IMAGEMANAGER->findImage("startText");
	_menu[1] = IMAGEMANAGER->findImage("mapToolText");

	
	_isExit = false;
	_cursorAni->start();

	return S_OK;
}

void exitPopup::release()
{
	_background = nullptr;
	_menu[0] = nullptr;
	_menu[0] = nullptr;
	_cursor = nullptr;
	_cursorAni = nullptr;
}

void exitPopup::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) || KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		_isExit = !_isExit;
	}
	else if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{

	}
}

void exitPopup::render()
{
	_background->render(0.f, 0.f, WINSIZEX, WINSIZEY, 1.f, true);
}

void exitPopup::uiOpen()
{
	_isExit = false;
	_cursorAni->start();
}

void exitPopup::uiClose()
{
	_isExit = false;
	_cursorAni->stop();
}
