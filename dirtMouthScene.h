#pragma once
#include "baseScene.h"
class dirtMouthScene :	public baseScene
{
public:
	dirtMouthScene();
	~dirtMouthScene();

	HRESULT init()  override;
	void release()  override;
	void update()	override;
	void render()	override;
};

