#include "stdafx.h"
#include "bulletState.h"
#include "bullet.h"
#include "animation.h"

bulletState::bulletState()
: _bullet(nullptr)
{
}

bulletState::~bulletState()
{
}

HRESULT bulletState::init(bullet* actor)
{
	actorState::init(actor);
	_bullet = actor;

	_height = actor->getHeight();
	_width = actor->getWidth();

	setAnimaion(actor->getUID(), _aniKey);

	return S_OK;
}

void bulletState::release()
{
	actorState::release();
	_bullet = nullptr;
}

void bulletState::update()
{
	actorState::update();
}

void bulletState::render()
{
	actorState::render();
}

void bulletState::start()
{
	actorState::start();
	_ani->start();
}

void bulletState::end()
{
	actorState::end();
}



HRESULT bulletMove::init(bullet* actor)
{
	_state = bullet::eMOVE;
	_aniKey = bullet::eMOVE;
	bulletState::init(actor);

	return S_OK;
}

void bulletMove::update()
{
	bulletState::update();
	_bullet->move();
}



HRESULT bulletPang::init(bullet* actor)
{
	_state = bullet::ePANG;
	_aniKey = bullet::ePANG;
	bulletState::init(actor);

	return S_OK;
}

void bulletPang::update()
{
	bulletState::update();
	if(!_ani->isPlay())
		end();
}

void bulletPang::end()
{
	_bullet->setDisappear();
}
