#include "stdafx.h"
#include "baseInGameScene.h"
#include "mapData.h"
#include "actorManager.h"
#include "windowDialog.h"
#include "playerStatusUI.h"


baseInGameScene::baseInGameScene()
: //_player(nullptr)
 _mapData(nullptr)
, _actorM(nullptr)
{
}


baseInGameScene::~baseInGameScene()
{
}

HRESULT baseInGameScene::init()
{
	baseScene::init();
	_isInGameScene = true;

	_mapData = new mapData;
	_mapData->init();
	_mapData->load(SCENEMANAGER->getSceneFileName(_sceneName));

	_actorM = new actorManager;
	_actorM->init();
	_actorM->mapDataLink(_mapData);

	setActors();

	PLAYER->mapDataLink(_mapData);
	PLAYER->actorManagerLink(_actorM);

	return S_OK;

}

void baseInGameScene::release()
{
	baseScene::release();

	SAFE_RELEASE(_mapData);
	SAFE_RELEASE(_actorM);

	SAFE_DELETE(_mapData);
	SAFE_DELETE(_actorM);
}

void baseInGameScene::update()
{
	baseScene::update();

	if(KEYMANAGER->isOnceKeyDown('1')) setDebugMode(DEBUG_SHOW_RECT);
	if(KEYMANAGER->isOnceKeyDown('2')) setDebugMode(DEBUG_SHOW_TEXT);
	if(KEYMANAGER->isOnceKeyDown('3')) setDebugMode(DEBUG_SHOW_UID);
	if(KEYMANAGER->isOnceKeyDown('4')) setDebugMode(DEBUG_SHOW_COLLISON);
	if(KEYMANAGER->isOnceKeyDown('5')) setDebugMode(DEBUG_SHOW_POSITION);
	if(KEYMANAGER->isOnceKeyDown('6')) setDebugMode(DEBUG_HIDE_IMAGE);

	PLAYER->update();
	_actorM->update();
}

void baseInGameScene::render()
{
	baseScene::render();

	IMAGEMANAGER->findImage("bg")->render(0.f, 0.f, WINSIZEX, WINSIZEY, 0.4f, true);

	_mapData->renderBack();
	IMAGEMANAGER->findImage("bg")->render(0.f, 0.f, WINSIZEX, WINSIZEY, 0.2f, true);

	_actorM->render();
	PLAYER->render();
	_mapData->renderFront();
}

void baseInGameScene::initUI()
{
	windowDialog* dialogUi = new windowDialog;
	dialogUi->init();
	dialogUi->setActive(true);
	dialogUi->uiClose();
	UIMANAGER->insertUI(eUI_Dialog, dialogUi);
	UIMANAGER->setDialogUI(dialogUi);

	playerStatusUI* statusUI = new playerStatusUI;
	statusUI->init();
	statusUI->setActive(true);
	statusUI->uiOpen();
	UIMANAGER->insertUI(eUI_Status, statusUI);
	UIMANAGER->setStatusUI(statusUI);


	POINTF pos = _mapData->getPlayerStartPosition();
	PLAYER->setStart(pos.x, pos.y);
}

void baseInGameScene::setActors()
{
	{
		map<UINT, npc*>* npcs = _mapData->getNpcs();
		map<UINT, npc*>::iterator iter = npcs->begin();
		map<UINT, npc*>::iterator end = npcs->end();

		for (iter; end != iter; ++iter)
			_actorM->addNpc(iter->second);
	}
	
	{
		vector<enemy*>* enemys = _mapData->getEnemys();
		vector<enemy*>::iterator iter = enemys->begin();
		vector<enemy*>::iterator end = enemys->end();

		for(iter; end != iter; ++iter)
			_actorM->addEnemy(*iter);
	}
}

void baseInGameScene::setDebugMode(UINT mode)
{
	if (DEVTOOL->checkDebugMode(mode))
		DEVTOOL->delDebugMode(mode);
	else
		DEVTOOL->setDebugMode(mode);
}
