#include "stdafx.h"
#include "loadScene.h"
#include "animation.h"
#include "mapData.h"
#include "actorManager.h"

loadScene::loadScene()
{
}


loadScene::~loadScene()
{
}

HRESULT loadScene::init()
{
	_sceneName = eSceneName_Load;
	_ani = ANIMANAGER->findAnimation(LOADING_UID, 0);
	_ani->start();

	SOUNDMANAGER->setBgmPause();

	_time = 0.f;


	CreateThread
	(
		NULL,						//스레드의 보안속성(자식윈도우가 있을때) 핸들로 자식인지 부모인지 여부파악
		NULL,						//스레드의 스택크기(NULL로 두면 메인쓰레드와 스택크기는 동일)
		ThreadFunction_LoadScene,	//스레드 함수 명
		this,						//스레드의 매개변수 (this로 하면 본 클래스)
		NULL,						//스레드의 특성(NULL로 두면 바로 실행함)
		0							//스레드의 생성 후 스레드의 ID를 넘겨준다 보통은 NULL로 둠.
	);

	_isFinSceneData = false;

	return S_OK;
}

void loadScene::release()
{
	_ani = nullptr;
}

void loadScene::render()
{
	IMAGEMANAGER->findImage("bg")->render(0.f, 0.f, WINSIZEX, WINSIZEY, 1.0f, true);
	IMAGEMANAGER->findImage("knight_loading")->aniRender(WINSIZEX - 100.f, WINSIZEY - 200.f, _ani, 1.0f, true);
}

void loadScene::update()
{
	if (_isFinSceneData)
			SCENEMANAGER->changeNextScene();
	else
		_ani->frameUpdate(TIMEMANAGER->getElapsedTime());
}

void loadScene::startScene()
{
}

void loadScene::initUI()
{
	baseScene::initUI();
}


static DWORD CALLBACK ThreadFunction_LoadScene(LPVOID lpParameter)
{
	loadScene* loadHelper = (loadScene*)lpParameter;

	eSceneName name = SCENEMANAGER->getNextSceneName();
	
	mapData* data			= nullptr;
	actorManager* actorM	= nullptr;
	string sceneStr = SCENEMANAGER->getSceneFileName(name);
	if (!sceneStr.empty())
	{
		data = new mapData;
		data->init();
		data->load(sceneStr);
		Sleep(1000);

		actorM = new actorManager;
		actorM->init();
		actorM->mapDataLink(data);
		Sleep(1000);
		
		PLAYER->mapDataLink(data);
		PLAYER->actorManagerLink(actorM);
		Sleep(1000);
	}
	
	SCENEMANAGER->createNextScene(data, actorM);
	Sleep(1000);

	loadHelper->_isFinSceneData = true;
	

	return 0;
}