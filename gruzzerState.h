#pragma once
#include "actorState.h"

class gruzzer;
class gruzzerMove: public actorState
{

private:
	gruzzer* _gruzzer;

public:
	gruzzerMove();
	~gruzzerMove();

	HRESULT init(gruzzer* actor);
	void release();
	void update();
	void render();

	void start();
	void end();
};

class gruzzerDead: public actorState
{
public:
	gruzzerDead();
	~gruzzerDead();
};

