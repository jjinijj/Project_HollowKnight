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

	_width = actor->getWidth();
	_height = actor->getHeight();
	
	_gruzzer = actor;

	_state = eMOVE;
	
	setAnimaion(_gruzzer->getUID(), eMOVE);
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

HRESULT gruzzerDead::init(gruzzer* actor)
{
	actorState::init(actor);

	_width = actor->getWidth();
	_height = actor->getHeight();
	
	_gruzzer = actor;

	_state = eDEAD;
	
	setAnimaion(_gruzzer->getUID(), eDEAD);
	return S_OK;
}

void gruzzerDead::release()
{
	actorState::release();
	_gruzzer = nullptr;
}

void gruzzerDead::update()
{
	actorState::update();
	if(!_ani->isPlay())
		end();
}

void gruzzerDead::render()
{
	actorState::render();
}

void gruzzerDead::start()
{
	actorState::start();
	_ani->start();
}

void gruzzerDead::end()
{
	_gruzzer->setDisappear();
}
