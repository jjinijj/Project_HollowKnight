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

	if(KEYMANAGER->isOnceKeyDown('1'))
	{
		if(DEVTOOL->checkDebugMode(DEBUG_SHOW_RECT))
			DEVTOOL->delDebugMode(DEBUG_SHOW_RECT);
		else
			DEVTOOL->setDebugMode(DEBUG_SHOW_RECT);
	}
	else if (KEYMANAGER->isOnceKeyDown('2'))
	{
		if (DEVTOOL->checkDebugMode(DEBUG_SHOW_TEXT))
			DEVTOOL->delDebugMode(DEBUG_SHOW_TEXT);
		else
			DEVTOOL->setDebugMode(DEBUG_SHOW_TEXT);
	}
	else if (KEYMANAGER->isOnceKeyDown('3'))
	{
		if (DEVTOOL->checkDebugMode(DEBUG_SHOW_UID))
			DEVTOOL->delDebugMode(DEBUG_SHOW_UID);
		else
			DEVTOOL->setDebugMode(DEBUG_SHOW_UID);
	}
	else if (KEYMANAGER->isOnceKeyDown('4'))
	{
		if (DEVTOOL->checkDebugMode(DEBUG_SHOW_COLLISON))
			DEVTOOL->delDebugMode(DEBUG_SHOW_COLLISON);
		else
			DEVTOOL->setDebugMode(DEBUG_SHOW_COLLISON);
	}
	else if (KEYMANAGER->isOnceKeyDown('5'))
	{
		if (DEVTOOL->checkDebugMode(DEBUG_SHOW_POSITION))
			DEVTOOL->delDebugMode(DEBUG_SHOW_POSITION);
		else
			DEVTOOL->setDebugMode(DEBUG_SHOW_POSITION);
	}
	else if (KEYMANAGER->isOnceKeyDown('6'))
	{
		if (DEVTOOL->checkDebugMode(DEBUG_HIDE_IMAGE))
			DEVTOOL->delDebugMode(DEBUG_HIDE_IMAGE);
		else
			DEVTOOL->setDebugMode(DEBUG_HIDE_IMAGE);
	}

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
