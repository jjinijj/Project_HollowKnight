#include "stdafx.h"
#include "gameNode.h"


gameNode::gameNode()
{
}


gameNode::~gameNode()
{
}

HRESULT gameNode::init()
{
	_managerInit = false;

	return S_OK;
}

HRESULT gameNode::init(bool managerInit)
{
	_managerInit = managerInit;

	if (_managerInit)
	{
		SetTimer(_hWnd, 1, 10, NULL);
		D2DMANAGER->init();
		TIMEMANAGER->init();
		SCENEMANAGER->init();
		KEYMANAGER->init();
		IMAGEMANAGER->init();
		INIDATAMANAGER->init();
		SOUNDMANAGER->init();
		KEYANIMANAGER->init();
		CAMERA->init();
		TXTDATA->init();
		EFFECTMANAGER->init();
	}

	return S_OK;
}

void gameNode::release()
{
	if (_managerInit)
	{
		KillTimer(_hWnd, 1);

		KEYMANAGER->release();
		KEYMANAGER->releaseSingleton();
		IMAGEMANAGER->release();
		IMAGEMANAGER->releaseSingleton();
		CAMERA->release();
		CAMERA->releaseSingleton();
		D2DMANAGER->release();
		D2DMANAGER->releaseSingleton();
		TIMEMANAGER->release();
		TIMEMANAGER->releaseSingleton();
		SCENEMANAGER->release();
		SCENEMANAGER->releaseSingleton();
		TXTDATA->release();
		TXTDATA->releaseSingleton();
		SOUNDMANAGER->release();
		SOUNDMANAGER->releaseSingleton();
		INIDATAMANAGER->release();
		INIDATAMANAGER->releaseSingleton();
		KEYANIMANAGER->release();
		KEYANIMANAGER->releaseSingleton();
		EFFECTMANAGER->release();
		EFFECTMANAGER->releaseSingleton();
	}
}

void gameNode::update()
{
	
}

void gameNode::render()
{
}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;		//조사 한 번 해바밤바1
	HDC			hdc;	//조사 한 번 해바밤바2

	switch (iMessage)
	{
		case WM_CREATE:
		break;
		
		case WM_MOUSEMOVE:
		{
			_ptMouse.x = static_cast<float>(LOWORD(lParam));
			_ptMouse.y = static_cast<float>(HIWORD(lParam));
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
