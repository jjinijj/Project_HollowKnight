#pragma once
#include "baseInGameScene.h"
#include "gruzzer.h"
#include "elderbug.h"


class mapData;
class actorManager;
class dirtMouthScene :	public baseInGameScene
{
private:

public:
	dirtMouthScene();
	~dirtMouthScene();

	HRESULT init()  override;
	void release()  override;
	void update()	override;
	void render()	override;
};

