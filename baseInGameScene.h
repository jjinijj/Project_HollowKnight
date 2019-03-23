#pragma once
#include "baseScene.h"
#include "player.h"
#include "actorManager.h"
#include "mapData.h"

class baseInGameScene: public baseScene
{
protected:
	//player*			_player;
	mapData*		_mapData;
	actorManager*	_actorM;

public:
	baseInGameScene();
	~baseInGameScene();

	HRESULT init()  override;
	void release()  override;
	void update()	override;
	void render()	override;

	// actorµé ¹èÄ¡(npc, enemy)
	virtual void setActors();
};

