#pragma once
#include "baseScene.h"

class mapData;
class actorManager;
class baseInGameScene: public baseScene
{
protected:
	mapData*		_mapData;
	actorManager*	_actorM;

public:
	baseInGameScene();
	~baseInGameScene();

	HRESULT init()  override;
	void release()  override;
	void update()	override;
	void render()	override;
};

