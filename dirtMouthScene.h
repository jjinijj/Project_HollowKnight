#pragma once
#include "baseInGameScene.h"
#include "player.h"
#include "gruzzer.h"
#include "actorManager.h"

class mapData;
class actorManager;
class dirtMouthScene :	public baseInGameScene
{
private:
	player* _player;

public:
	dirtMouthScene();
	~dirtMouthScene();

	HRESULT init()  override;
	void release()  override;
	void update()	override;
	void render()	override;
};

