#pragma once
#include "gameNode.h"

#include "loadingScene.h"
#include "mapToolScene.h"
#include "dirtMouthScene.h"
#include "titleScene.h"

class playGround : public gameNode
{
private:

public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

};