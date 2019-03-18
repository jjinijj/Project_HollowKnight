#pragma once
#include "baseScene.h"

class player;
class mapData;
class dirtMouthScene :	public baseScene
{
private:
	player* _player;
	mapData* _mapData;

public:
	dirtMouthScene();
	~dirtMouthScene();

	HRESULT init()  override;
	void release()  override;
	void update()	override;
	void render()	override;
};

