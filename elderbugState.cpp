#include "stdafx.h"
#include "elderbugState.h"
#include "elderbug.h"
#include "animation.h"


elderbugState::elderbugState()
: _elderbug(nullptr)
{
}

elderbugState::~elderbugState()
{
}

HRESULT elderbugState::init(elderbug* actor)
{
	actorState::init(actor);
	_elderbug = actor;

	_width = elderbug::ELDERBUG_WIDTH;
	_height = elderbug::ELDERBUG_HEIGHT;

	return S_OK;
}

void elderbugState::release()
{
	actorState::release();
	_elderbug = nullptr;
}

void elderbugState::update()
{
	actorState::update();
}

void elderbugState::render()
{
	actorState::render();
}

void elderbugState::start()
{
	actorState::start();
	_ani->start();
}

void elderbugState::end()
{
	actorState::end();
}


//=============================================
// idle
//=============================================
HRESULT elderbugIdle::init(elderbug* actor)
{
	elderbugState::init(actor);

	_state = elderbug::eIdle;
	_aniKey = elderbug::eIdle;
	setAnimaion(actor->getUID(), _aniKey);

	return S_OK;
}

//=============================================
// idle
//=============================================
HRESULT elderbugTalk::init(elderbug* actor)
{
	elderbugState::init(actor);

	_state = elderbug::eTalk;
	_aniKey = elderbug::eTalk;
	setAnimaion(actor->getUID(), _aniKey);

	return S_OK;
}
