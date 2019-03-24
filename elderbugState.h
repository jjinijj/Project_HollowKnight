#pragma once
#include "actorState.h"

class elderbug;
class elderbugState : public actorState
{
protected:
	elderbug* _elderbug;

public:
	elderbugState();
	~elderbugState();

	virtual HRESULT init(elderbug* actor);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void start();
	virtual void end();
};


class elderbugIdle: public elderbugState
{
public:
	elderbugIdle() {}
	~elderbugIdle(){}

	HRESULT init(elderbug* actor);
};

class elderbugTalk : public elderbugState
{
public:
	elderbugTalk() {}
	~elderbugTalk(){}

	HRESULT init(elderbug* actor);
};
