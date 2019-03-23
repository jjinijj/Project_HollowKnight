#include "stdafx.h"
#include "tiktikState.h"
#include "tiktik.h"

tiktikMove::tiktikMove()
{
}

tiktikMove::~tiktikMove()
{
}

HRESULT tiktikMove::init(tiktik* actor)
{
	actorState::init(actor);
	_tiktik = actor;

	_height = actor->getHeight();
	_width = actor->getWidth();

	return S_OK;
}

void tiktikMove::release()
{
	actorState::release();
	_tiktik = nullptr;
}

void tiktikMove::update()
{
	actorState::update();
}

void tiktikMove::render()
{
	actorState::render();
}

void tiktikMove::start()
{
	_ani->start();
}

void tiktikMove::end()
{
	actorState::end();
}


HRESULT tiktikMoveOn::init(tiktik* actor)
{
	tiktikMove::init(actor);
	setAnimaion(actor->getUID(), tiktik::eMOVE_ON);
	return S_OK;
}

void tiktikMoveOn::update()
{
	tiktikMove::update();
	_tiktik->moveOn();
}

HRESULT tiktikMoveUnder::init(tiktik* actor)
{
	tiktikMove::init(actor);
	setAnimaion(actor->getUID(), tiktik::eMOVE_UNDER);
	return S_OK;
}

void tiktikMoveUnder::update()
{
	tiktikMove::update();
	_tiktik->moveUnder();
}

HRESULT tiktikMoveSideUp::init(tiktik* actor)
{
	tiktikMove::init(actor);
	setAnimaion(actor->getUID(), tiktik::eMOVE_SIDE_UP);
	return S_OK;
}

void tiktikMoveSideUp::update()
{
	tiktikMove::update();
	_tiktik->moveSideUp();
}

HRESULT tiktikMoveSiedDown::init(tiktik* actor)
{
	tiktikMove::init(actor);
	setAnimaion(actor->getUID(), tiktik::eMOVE_SIDE_DOWN);
	return S_OK;
}

void tiktikMoveSiedDown::update()
{
	tiktikMove::update();
	_tiktik->moveSiedDown();
}

HRESULT tiktikClimbSideToOn::init(tiktik* actor)
{
	tiktikMove::init(actor);
	setAnimaion(actor->getUID(), tiktik::eCLIMB_SIDE_TO_ON);
	return S_OK;
}

void tiktikClimbSideToOn::update()
{
	tiktikMove::update();
	if(_ani->isPlay())
		end();
}

void tiktikClimbSideToOn::end()
{
	if (!_next)
	{
		_next = new tiktikMoveOn;
		_next->init(_actor);
	}
}

HRESULT tiktikClimbSideToDown::init(tiktik* actor)
{
	tiktikMove::init(actor);
	setAnimaion(actor->getUID(), tiktik::eCLIMB_SIDE_TO_UNDER);
	return S_OK;
}

void tiktikClimbSideToDown::update()
{
	if(_ani->isPlay())
		end();
}

void tiktikClimbSideToDown::end()
{
	if (!_next)
	{
		_next = new tiktikMoveUnder;
		_next->init(_actor);
	}
}

HRESULT tiktikClimbOnToSide::init(tiktik* actor)
{
	tiktikMove::init(actor);
	setAnimaion(actor->getUID(), tiktik::eCLIME_ON_TO_SIDE);
	return S_OK;
}

void tiktikClimbOnToSide::update()
{
	if(_ani->isPlay())
		end();
}

void tiktikClimbOnToSide::end()
{
	if (!_next)
	{
		_next = new tiktikMoveSiedDown;
		_next->init(_actor);
	}
}

HRESULT tiktikClimbUnderToSide::init(tiktik* actor)
{
	tiktikMove::init(actor);
	setAnimaion(actor->getUID(), tiktik::eCLIME_UNDER_TO_SIDE);
	return S_OK;
}

void tiktikClimbUnderToSide::update()
{
	if(_ani->isPlay())
		end();
}

void tiktikClimbUnderToSide::end()
{
	if (!_next)
	{
		_next = new tiktikMoveSideUp;
		_next->init(_actor);
	}
}
