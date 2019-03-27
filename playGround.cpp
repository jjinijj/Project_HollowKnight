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
	SCENEMANAGER->addScene(eSceneName_DirtMouth, new dirtMouthScene);
	SCENEMANAGER->addScene(eSceneName_Title, new titleScene);
	SCENEMANAGER->addScene(eSceneName_CrossLoad_01, new crossLoad);
	SCENEMANAGER->addScene(eSceneName_Load, new loadScene);


	SCENEMANAGER->changeScene(eSceneName_Loading);
	
	PLAYER->init(0, 0.f);

	_exitPop = new exitPopup;
	_exitPop->init();
	_exitPop->uiClose();
	
	return S_OK;
}

void playGround::release()
{
	gameNode::release();
	PLAYER->release();
	PLAYER->releaseSingleton();
	
}

void playGround::update()
{
	gameNode::update();

	SOUNDMANAGER->update();
	
	// 인게임에서 메인화면으로 빠져나오기 : 로딩중에는 불가능
	if(SCENEMANAGER->getCurrentScene()->isInGameScene() && KEYMANAGER->isOnceKeyDown(VK_ESCAPE))
	{
		if (_exitPop->isOpen())
			_exitPop->uiClose();
		else
			_exitPop->uiOpen();
	}

	if (_exitPop->isOpen())
	{
		_exitPop->update();
		return;
	}

	CAMERA->update();
	SCENEMANAGER->update();
	UIMANAGER->update();

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
	// Draw 시작 - 이 코드가 빠지면 D2D 출력 X
	D2DMANAGER->beginDraw();
	//===========================================================================
	//				##		여기에 코드 작성(Start)			##

	if (!_exitPop->isOpen())
	{
		SCENEMANAGER->render();
		UIMANAGER->render();
		DEVTOOL->render();
		TIMEMANAGER->render();
	}
	else
		_exitPop->render();

	//				##			여기에 코드 작성(End)			##
	//===========================================================================
	//				##카메라 정보 마우스 정보 시간정보 출력	##===================


	// Draw 끝 - 이 코드가 빠지면 D2D 출력 X
	D2DMANAGER->endDraw();
}