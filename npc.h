#pragma once
#include "actorBase.h"
class npc :	public actorBase
{
protected:
	UINT _dialogIdx;

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

