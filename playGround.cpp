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

	CAMERA->update();
	UIMANAGER->update();
	SCENEMANAGER->update();

#ifdef _DEBUG
	DEVTOOL->pushBackDebugText(format(L"cameraX : %.2f", CAMERA->getPosX()));
	DEVTOOL->pushBackDebugText(format(L"cameraY : %.2f", CAMERA->getPosY()));
	DEVTOOL->pushBackDebugText(format(L"mouseX : %.2f", _ptMouse.x));
	DEVTOOL->pushBackDebugText(format(L"mouseY : %.2f", _ptMouse.y));
	DEVTOOL->pushBackDebugText(format(L"sceneName %d", SCENEMANAGER->getCurrentScene()->getSceneName()));
#endif // DEBUG
}

void playGround::render()
{
	// Draw ���� - �� �ڵ尡 ������ D2D ��� X
	D2DMANAGER->beginDraw();
	//===========================================================================
	//				##		���⿡ �ڵ� �ۼ�(Start)			##

	SCENEMANAGER->render();
	UIMANAGER->render();


	//				##			���⿡ �ڵ� �ۼ�(End)			##
	//===========================================================================
	//				##ī�޶� ���� ���콺 ���� �ð����� ���	##===================

	DEVTOOL->render();
	TIMEMANAGER->render();

	// Draw �� - �� �ڵ尡 ������ D2D ��� X
	D2DMANAGER->endDraw();
}