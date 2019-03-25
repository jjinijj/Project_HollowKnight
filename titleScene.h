#pragma once
#include "baseScene.h"
class titleScene : 	public baseScene
{
private:
	float _alpha;
	bool _isInputEnterKey;

	image* _back;
	image* _bg;

public:
	titleScene();
	~titleScene();

	HRESULT init();
	void release();
	void update();
	void render();
};

