#include "stdafx.h"
#include "primalAspidState.h"
#include "primalAspid.h"
#include "animation.h"


primalState::primalState()
: _primal(nullptr)
{
}

primalState::~primalState()
{
}

HRESULT primalState::init(primalAspid* actor)
{
	actorState::init(actor);

	_primal = actor;
	assert(nullptr != _primal);

	_height = actor->getHeight();
	_width = actor->getWidth();

	setAnimaion(actor->getUID(), _aniKey);

	return S_OK;
}

void primalState::release()
{
	actorState::release();
	_primal = nullptr;
}

void primalState::update()
{
	actorState::update();
}

void primalState::render()
{
	actorState::render();
}

void primalState::start()
{
	actorState::start();
	_ani->start();
}

void primalState::end()
{
	actorState::end();
}


//=============================================
//
//=============================================
HRESULT primalIdle::init(primalAspid* actor)
{
	_state = primalAspid::eIDLE;
	_aniKey = primalAspid::eIDLE;
	primalState::init(actor);


	return S_OK;
}

void primalIdle::update()
{
	primalState::update();
	if(_primal->checkTargetInViewRange())
		end();
}

void primalIdle::end()
{
	primalState* state = new primalApproach;
	state->init(_primal);
	_next = state;
}


//=============================================
// 
//=============================================
HRESULT primalApproach::init(primalAspid* actor)
{
	_state = primalAspid::eMOVE_APPROACH;
	_aniKey = primalAspid::eMOVE_APPROACH;
	primalState::init(actor);

	return S_OK;
}

void primalApproach::update()
{
	primalState::update();
	if(_primal->checkTargetInAttackRange())
		end();
	else
		_primal->moveToTarget();
}

void primalApproach::end()
{
	primalState* state = new primalAttack;
	state->init(_primal);
	_next = state;
}


//=============================================
// 
//=============================================
HRESULT primalAttack::init(primalAspid* actor)
{
	_state = primalAspid::eATTACK;
	_aniKey = primalAspid::eATTACK;
	primalState::init(actor);

	return S_OK;
}

void primalAttack::update()
{
	primalState::update();
	if (_ani->isEventFrame())
	{
		_primal->attack();
		end();
	}
}

void primalAttack::end()
{
	primalState* state = new primalFarway;
	state->init(_primal);
	_next = state;
}


//=============================================
// 
//=============================================
HRESULT primalFarway::init(primalAspid* actor)
{
	_state = primalAspid::eMOVE_FARWAY;
	_aniKey = primalAspid::eMOVE_FARWAY;
	primalState::init(actor);

	return S_OK;
}

void primalFarway::update()
{
	primalState::update();
	if (!_primal->checkTargetInViewRange())
		end();
	else
		_primal->moveFromTarget();
}

void primalFarway::end()
{
	primalState* state = new primalIdle;
	state->init(_primal);
	_next = state;
}


//=============================================
// 
//=============================================
primalDead::primalDead()
{
}

primalDead::~primalDead()
{
}

HRESULT primalDead::init(primalAspid * actor)
{
	_state = primalAspid::eDEAD;
	_aniKey = primalAspid::eDEAD;
	primalState::init(actor);

	return S_OK;
}

void primalDead::release()
{
}

void primalDead::update()
{
}

void primalDead::start()
{
}

void primalDead::end()
{
}
