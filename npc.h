#pragma once
#include "actorBase.h"
class npc :	public actorBase
{
public:
	npc();
	~npc();

	HRESULT init();
	void release();
	void update();
	void render();

	void changeState();

	void talkStart();
};

