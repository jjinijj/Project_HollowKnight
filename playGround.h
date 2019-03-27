#pragma once
#include "gameNode.h"

#include "loadingScene.h"
#include "mapToolScene.h"
#include "dirtMouthScene.h"
#include "titleScene.h"
#include "crossLoad.h"
#include "loadScene.h"
#include"exitPopup.h"

class playGround : public gameNode
{
private:
	exitPopup* _exitPop;


public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

};