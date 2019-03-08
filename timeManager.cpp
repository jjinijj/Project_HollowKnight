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

	//����� ���¸�
#ifdef _DEBUG
	{
		DEVTOOL->pushBackDebugText(format(L"framePerSec(FPS) : %d", _timer->getFrameRate()));
		DEVTOOL->pushBackDebugText(format(L"worldTime : %f", _timer->getWorldTime()));
		DEVTOOL->pushBackDebugText(format(L"elapsedTime : %f", _timer->getElapsedTime()));
	}
#else
	//������ ���¸�
	{
		DEVTOOL->pushBackDebugText(format(L"framePerSec(FPS) : %d", _timer->getFrameRate()));
	}
#endif
}

void timeManager::render()
{
}
