#pragma once
#include "baseScene.h"

class uiButton;
class titleScene : 	public baseScene
{
private:
	bool _isInputEnterKey;

	int _selIdx;

	eSceneName _nextSceneNameName;

	float _alpha;

	image* _back;
	image* _bg;

	image* _start;
	image* _mapTool;

	image*		_cursor;
	animation*	_cursorAni;
	
	image* _menu[3];


public:
	titleScene();
	~titleScene();

	HRESULT init();
	void release();
	void update();
	void render();

	void initUI();
};

