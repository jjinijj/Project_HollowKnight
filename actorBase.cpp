#include "stdafx.h"
#include "actorBase.h"


actorBase::actorBase()
: _uid(NULL)
, _type(eActor_None)
, _subType(NULL)
, _state(nullptr)
, _nextState(nullptr)
, _x(0.f)
, _y(0.f)
{
	_colSize = {};
	_collision = {};
}


actorBase::~actorBase()
{
}

HRESULT actorBase::init(UINT uid, float x, float y)
{
	_uid = uid;
	_x = x;
	_y = y;

	return S_OK;
}

void actorBase::release()
{
	SAFE_RELEASE(_state);
	SAFE_RELEASE(_nextState);

	SAFE_DELETE(_state);
	SAFE_RELEASE(_nextState);
}

void actorBase::update()
{
	_state->update();
	_nextState = _state->nextState();
	if (_nextState)
		chansgeState(_nextState);
}

void actorBase::render()
{
	_state->render();
}

void actorBase::chansgeState(actorState* state)
{
	assert(nullptr != state);

	if(_state->getState() == state->getState())
		return;

	SAFE_RELEASE(_state);
	SAFE_DELETE(_state);

	_state = state;
	state = nullptr;
	
	_state->start();
}
