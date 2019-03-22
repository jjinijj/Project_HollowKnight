#pragma once
#include "actorState.h"

class elderbug;
class elderbugIdle: public actorState
{
private:
	elderbug* _elderbug;

public:
	elderbugIdle();
	~elderbugIdle();

	HRESULT init(elderbug* actor);
	void release();
	void update();
	void render();

	void start();
	void end();
};

