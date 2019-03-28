#pragma once
#include "baseInGameScene.h"

class dirtMouthScene :	public baseInGameScene
{
private:

public:
	dirtMouthScene();
	~dirtMouthScene();

	HRESULT init()		override;
	void release()		override;
	void update()		override;
	void render()		override;

	void startScene()	override;
	void initUI()		override;
};


