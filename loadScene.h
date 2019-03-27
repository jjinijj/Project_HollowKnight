#pragma once
#include "baseScene.h"

class uiBase;
class loadScene : public baseScene
{
private:
	animation* _ani;
	float _time;

public:
	loadScene();
	~loadScene();

	HRESULT init();
	void release();
	void render();
	void update();

};

