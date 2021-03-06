#pragma once
#include "baseScene.h"
#include "player.h"
#include "actorManager.h"
#include "mapData.h"
#include "objectManager.h"

class baseInGameScene: public baseScene
{
protected:
	//player*			_player;
	mapData*		_mapData;
	actorManager*	_actorM;
	objectManager*	_objM;

public:
	baseInGameScene();
	~baseInGameScene();

	HRESULT init()		override;
	void release()		override;
	void update()		override;
	void render()		override;
	
	void startScene()	override;
	void initUI()		override;


	void setSceneData(mapData* m, actorManager* am);

	// actor�� ��ġ(npc, enemy)
	virtual void setActors();

private:
	void setDebugMode(UINT mode);
};

