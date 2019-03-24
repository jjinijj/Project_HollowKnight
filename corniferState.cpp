#include "stdafx.h"
#include "corniferState.h"
#include "cornifer.h"
#include "animation.h"

corniferState::corniferState()
: _cornifer(nullptr)
{
}

corniferState::~corniferState()
{
}

HRESULT corniferState::init(cornifer* actor)
{
	actorState::init(actor);
	_cornifer = actor;

	_width = cornifer::CORNIFER_WIDTH;
	_height = cornifer::CORNIFER_HEIGHT;

	return S_OK;
}

void corniferState::release()
{
	actorState::release();
	_cornifer = nullptr;
}

void corniferState::update()
{
	actorState::update();
}

void corniferState::render()
{
	actorState::render();
}

void corniferState::start()
{
	actorState::start();
	_ani->start();
}

void corniferState::end()
{
	actorState::end();
}

HRESULT corniferIdle::init(cornifer* actor)
{
	corniferState::init(actor);
	_state = cornifer::eIdle;
	_aniKey = cornifer::eIdle;
	setAnimaion(actor->getUID(), _aniKey);

	return S_OK;
}

HRESULT corniferTalkLeft::init(cornifer * actor)
{
	corniferState::init(actor);
	_state = cornifer::eTalk_Left;
	_aniKey = cornifer::eTalk_Left;
	setAnimaion(actor->getUID(), _aniKey);

	return S_OK;
}

HRESULT corniferTalkRight::init(cornifer * actor)
{
	corniferState::init(actor);
	_state = cornifer::eTalk_Right;
	_aniKey = cornifer::eTalk_Right;
	setAnimaion(actor->getUID(), _aniKey);

	return S_OK;
}
