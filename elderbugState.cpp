#include "stdafx.h"
#include "elderbugState.h"
#include "elderbug.h"
#include "animation.h"

elderbugIdle::elderbugIdle()
: _elderbug(nullptr)
{
}

elderbugIdle::~elderbugIdle()
{
}

HRESULT elderbugIdle::init(elderbug* actor)
{
	actorState::init(actor);
	_aniKey = elderbug::eIdle;
	setAnimaion(actor->getUID(), _aniKey);

	_width = elderbug::ELDERBUG_WIDTH;
	_height = elderbug::ELDERBUG_HEIGHT;

	return S_OK;
}

void elderbugIdle::release()
{
	actorState::release();
}

void elderbugIdle::update()
{
	actorState::update();
}

void elderbugIdle::render()
{
	actorState::render();
}

void elderbugIdle::start()
{
	actorState::start();
	_ani->start();
}

void elderbugIdle::end()
{
	actorState::end();
}
