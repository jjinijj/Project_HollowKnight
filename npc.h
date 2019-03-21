#pragma once
#include "actorBase.h"
class npc :	public actorBase
{
public:
	npc();
	~npc();

	HRESULT init(UINT uid, float x, float y);
	void release();
	void update();
	void render();

	void changeState();

	void talkStart();
};

