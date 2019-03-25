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


	//DEVTOOL->setDebugMode(DEBUG_SHOW_TEXT);
	//DEVTOOL->setDebugMode(DEBUG_SHOW_RECT);

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

	PLAYER->init(100.f, 100.f);
	PLAYER->mapDataLink(_mapData);
	PLAYER->actorManagerLink(_actorM);

	//_actorM->createEnemy(WINSIZEX / 2.f, WINSIZEY / 2.f + 100.f, eEnemy_Gruzzer);
	//_actorM->createNPC(WINSIZEX / 2.f, WINSIZEY / 2.f, eNpc_Elderbug);

	return S_OK;

}

void baseInGameScene::release()
{
	baseScene::release();

	SAFE_RELEASE(_mapData);
	SAFE_RELEASE(_actorM);
	//SAFE_RELEASE(_player);

	SAFE_DELETE(_mapData);
	SAFE_DELETE(_actorM);
	//SAFE_DELETE(_player);
}

void baseInGameScene::update()
{
	baseScene::update();
	PLAYER->update();
	_actorM->update();
}

void baseInGameScene::render()
{
	baseScene::render();

	_mapData->renderBack();
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
