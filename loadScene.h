#pragma once
#include "baseScene.h"

class uiBase;
class loadScene : public baseScene
{
public:
	bool _isFinSceneData;

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

	void startScene();
	void initUI();

};

static DWORD CALLBACK ThreadFunction_LoadScene(LPVOID lpParameter);