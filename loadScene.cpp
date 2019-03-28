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
		NULL,						//�������� ���ȼӼ�(�ڽ������찡 ������) �ڵ�� �ڽ����� �θ����� �����ľ�
		NULL,						//�������� ����ũ��(NULL�� �θ� ���ξ������ ����ũ��� ����)
		ThreadFunction_LoadScene,	//������ �Լ� ��
		this,						//�������� �Ű����� (this�� �ϸ� �� Ŭ����)
		NULL,						//�������� Ư��(NULL�� �θ� �ٷ� ������)
		0							//�������� ���� �� �������� ID�� �Ѱ��ش� ������ NULL�� ��.
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