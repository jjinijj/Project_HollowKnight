#pragma once
#include "baseInGameScene.h"
class crossLoad: public baseInGameScene
{
public:
	crossLoad();
	~crossLoad();

	HRESULT init()		override;
	void release()		override;
	void update()		override;
	void render()		override;

	void startScene()	override;
	void initUI()		override;
};

