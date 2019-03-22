#include "stdafx.h"
#include "actorBase.h"


actorBase::actorBase()
: _type(eActor_None)
, _subType(NULL)
, _state(nullptr)
, _nextState(nullptr)
, _dir(eDirection_None)
, _dirUD(eDirection_None)
{
	_collision = {};
	_name.clear();
}


actorBase::~actorBase()
{
}

HRESULT actorBase::init()
{
	gameObject::init();
	return S_OK;
}

HRESULT actorBase::init(UINT uid, float x, float y)
{
	gameObject::init(uid, x, y);

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

ACTORPACK* actorBase::makePack()
{
	ACTORPACK* pack = new ACTORPACK;
	pack->clear();
	pack->uid = _uid;
	pack->x = _x;
	pack->y = _y;
	pack->type = _type;
	pack->subType = _subType;

	return pack;
}

void actorBase::loadPack(ACTORPACK* pack)
{
	if (pack)
	{
		_uid = pack->uid;
		_x = pack->x;
		_y = pack->y;
		_type = (eActorType)pack->type;
		_subType = pack->subType;
	}
}


void actorBase::setPosition(float x, float y)
{
	float disX = x - _x;
	float disY = y - _y;

	_x = x;
	_y = y;

	updateRect();
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

void actorBase::updateRect()
{
	_collision = {_x - _colWidth / 2.f, _y - _colHeight
				 ,_x + _colWidth / 2.f, _y };

	_rc = {  _x - _width / 2.f, _y - _height
			,_x + _width / 2.f, _y };
}
