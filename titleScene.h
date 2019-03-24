#pragma once
#include "baseScene.h"
class titleScene : 	public baseScene
{
public:
	titleScene();
	~titleScene();

	HRESULT init();
	void release();
	void update();
	void render();
};

