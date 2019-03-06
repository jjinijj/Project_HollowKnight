#include "stdafx.h"
#include "timeManager.h"


timeManager::timeManager()
	: _timer(NULL)
{
}


timeManager::~timeManager()
{
}

HRESULT timeManager::init()
{
	_timer = new timer;
	_timer->init();

	return S_OK;
}

void timeManager::release()
{
	if (_timer != NULL) SAFE_DELETE(_timer);
}

void timeManager::update(float lockFPS)
{
	if (_timer != NULL)
	{
		_timer->tick(lockFPS);
	}
}

void timeManager::render()
{
	WCHAR str[128];

	//디버그 상태면
#ifdef _DEBUG
	{
		

		swprintf_s(str, L"framePerSec(FPS) : %d", _timer->getFrameRate());
		D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY());
		swprintf_s(str, L"worldTime : %f", _timer->getWorldTime());
		D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 20);
		swprintf_s(str, L"elapsedTime : %f", _timer->getElapsedTime());
		D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 40);
	}
#else
	//릴리즈 상태면
	{

		swprintf_s(str, L"framePerSec(FPS) : %d", _timer->getFrameRate(), (float)(MAPSIZEX - WINSIZEX));
		D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY());
	}
#endif
}
