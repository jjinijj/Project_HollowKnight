#include "stdafx.h"
#include "playerState.h"
#include "player.h"


playerState::playerState()
: _player(nullptr)
, _nextState(ePlayer_State_None)
, _state(ePlayer_State_None)
{
}


playerState::~playerState()
{
}

HRESULT playerState::init(player* p)
{
	if (!p)
		return E_FAIL;

	_player = p;
	return S_OK;
}

void playerState::release()
{
	_player = nullptr;
}

void playerState::update()
{
}

void playerState::render()
{
}

void playerState::start()
{
	_nextState = ePlayer_State_None;
}

void playerState::end()
{
}

UINT playerState::nextState()
{
	return _nextState;
}

idleState::idleState()
{
}

idleState::~idleState()
{
}

HRESULT idleState::init(player* p)
{
	_state = ePlayer_State_Idle;
	return playerState::init(p);
}

void idleState::release()
{
}

void idleState::update()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_nextState = ePlayer_State_Walk;
		_player->setDirectionLeft();
	}
	
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_nextState = ePlayer_State_Walk;
		_player->setDirectionRight();
	}

	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		_nextState = ePlayer_State_Flying;
	} 
	else if (KEYMANAGER->isOnceKeyDown('X'))
	{
		_nextState = ePlayer_State_Attack;
	}

	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		_player->setDirectionUp();
		if (ePlayer_State_None == _nextState)
			_nextState = ePlayer_State_Look;
	}

	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		_player->setDirectionDown();

		if (ePlayer_State_None == _nextState)
			_nextState = ePlayer_State_Look;
	}

	if (_player->isStateFloating())
		_nextState = ePlayer_State_Falling;
}

void idleState::render()
{
}

void idleState::start()
{
	playerState::start();
}

void idleState::end()
{
}

UINT idleState::nextState()
{
	return _nextState;
}

walkState::walkState()
{
}

walkState::~walkState()
{
}

HRESULT walkState::init(player* p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = ePlayer_State_Walk;

	return S_OK;
}

void walkState::release()
{
	playerState::release();
}

void walkState::update()
{
	if(_isRight)
		rightMove();
	else
		leftMove();

	if (_player->isStateFloating())
		_nextState = ePlayer_State_Falling;
}

void walkState::render()
{
}

void walkState::start()
{
	playerState::start();
	
	_isRight = ((_player->getDirection() & eDirection_Right) == eDirection_Right);
}

void walkState::end()
{
	_nextState = ePlayer_State_Idle;
}

UINT walkState::nextState()
{
	return _nextState;
}

void walkState::rightMove()
{
	if(KEYMANAGER->isOnceKeyDown('Z'))
		_nextState = ePlayer_State_Flying;
	else if(KEYMANAGER->isOnceKeyDown('X'))
		_nextState = ePlayer_State_Attack;
	
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		_player->moveRight();
	else
		end();

}

void walkState::leftMove()
{
	if (KEYMANAGER->isOnceKeyDown('Z'))
		_nextState = ePlayer_State_Flying;
	else if (KEYMANAGER->isOnceKeyDown('X'))
		_nextState = ePlayer_State_Attack;

	if(KEYMANAGER->isStayKeyDown(VK_LEFT))
		_player->moveLeft();
	else
		end();
}