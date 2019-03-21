#include "stdafx.h"
#include "gruzzerState.h"
#include "gruzzer.h"
#include "animation.h"

gruzzerMove::gruzzerMove()
{
}


gruzzerMove::~gruzzerMove()
{
}

HRESULT gruzzerMove::init(gruzzer* actor)
{
	actorState::init(actor);

	_width = GRUZZER_WIDTH;
	_height = GRUZZER_HEIGHT;
	
	_gruzzer = actor;

	_state = eMOVE;
	_state = eMOVE;
	
	setAnimaion(_gruzzer->getUid(), eMOVE);
	return S_OK;
}

void gruzzerMove::release()
{
	actorState::release();
}

void gruzzerMove::update()
{
	actorState::update();
	
	_isRight = (_gruzzer->getDirectionLR() == actorBase::eRIGHT);
	if(_gruzzer->isAlive())
		_gruzzer->move();
	else
		end();
}

void gruzzerMove::render()
{
	actorState::render();
}

void gruzzerMove::start()
{
	actorState::start();
	_ani->start();
}

void gruzzerMove::end()
{
	if (!_next)
	{
		_next = new gruzzerDead;
		_next->init(_gruzzer);
	}
}


gruzzerDead::gruzzerDead()
{
}

gruzzerDead::~gruzzerDead()
{
}
