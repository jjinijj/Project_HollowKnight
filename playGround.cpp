#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}


playGround::~playGround()
{
}

HRESULT playGround::init()
{
	gameNode::init(true);

	SCENEMANAGER->addScene(eSceneName_Loading, new loadingScene);
	SCENEMANAGER->addScene(eSceneName_MapTool, new mapToolScene);



	SCENEMANAGER->changeScene(eSceneName_Loading);
	
	return S_OK;
}

void playGround::release()
{
	gameNode::release();
	
}

void playGround::update()
{
	gameNode::update();

	if (KEYMANAGER->isOnceKeyDown(VK_ESCAPE))
		PostQuitMessage(0);

	SCENEMANAGER->update();
	CAMERA->update();
}

void playGround::render()
{
	// Draw 시작 - 이 코드가 빠지면 D2D 출력 X
	D2DMANAGER->beginDraw();
	//===========================================================================
	//				##		여기에 코드 작성(Start)			##


	SCENEMANAGER->render();




	//				##			여기에 코드 작성(End)			##
	//===========================================================================
	//				##카메라 정보 마우스 정보 시간정보 출력	##===================
	WCHAR str[128];
	swprintf_s(str, L"cameraX : %.2f", CAMERA->getPosX());
	D2DMANAGER->drawText(str, 0, 80);
	swprintf_s(str, L"cameraY : %.2f", CAMERA->getPosY());
	D2DMANAGER->drawText(str, 0, 100);
	swprintf_s(str, L"mouseX : %.2f", _ptMouse.x);
	D2DMANAGER->drawText(str, 0, + 140);
	swprintf_s(str, L"mouseY : %.2f", _ptMouse.y);
	D2DMANAGER->drawText(str, 0, 160);
	swprintf_s(str, L"sceneName : %d", SCENEMANAGER->getCurrentScene()->getSceneName());
	D2DMANAGER->drawText(str, 0, 240);
	TIMEMANAGER->render();
	// Draw 끝 - 이 코드가 빠지면 D2D 출력 X
	D2DMANAGER->endDraw();
}