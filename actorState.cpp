#include "stdafx.h"
#include "actorState.h"
#include "animation.h"
#include "actorBase.h"

actorState::actorState()
{
}


actorState::~actorState()
{
}

HRESULT actorState::init(actorBase* actor)
{
	_actor = actor;
	assert(nullptr != _actor);
	return S_OK;
}

void actorState::release()
{
	_actor = nullptr;
	_next = nullptr;
}

void actorState::update()
{
	_ani->frameUpdate(TIMEMANAGER->getElapsedTime());
}

void actorState::render()
{
	if(_isRight)
		_img->aniRender( static_cast<int>(_actor->getPosX() - _width / 2.f)
						,static_cast<int>(_actor->getPosY() - _height)
						,_ani, _actor->getRenderAlpha(), false);
	else
		_img->aniRenderReverseX( _actor->getPosX() - _width / 2.f
								,_actor->getPosY() - _height
								,_ani, _actor->getRenderAlpha(), false);
}

void actorState::start()
{
	_isRight = (_actor->getDirectionLR() == actorBase::eRIGHT);
}

void actorState::end()
{
}

UINT actorState::getState()
{
	return _state;
}

actorState* actorState::nextState()
{
	return _next;
}

void actorState::setAnimaion(UINT uid, UINT aniKey)
{
	_aniKey = aniKey;
	_ani = ANIMANAGER->findAnimation(uid, aniKey);
	assert(nullptr != _ani);

	_img = _ani->getImage();
}
