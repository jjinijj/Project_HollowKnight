#include "stdafx.h"
#include "player.h"
#include "jumpState.h"


//=============================================
// jump start
//=============================================
HRESULT flyingState::init(player* p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = ePlayer_State_Flying;

	return S_OK;
}

void flyingState::release()
{
	playerState::release();
}

void flyingState::update()
{
	if( 0.7f <= _flyingTime)
		end();
	else
	{
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
			_player->moveLeft();

		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			_player->moveRight();

		if (KEYMANAGER->isStayKeyDown('Z'))
		{
			_flyingTime += TIMEMANAGER->getElapsedTime();
			_player->moveUp();
		}
		else
			end();
	}
}

void flyingState::render()
{
}

void flyingState::start()
{
	playerState::start();
	_player->setJumping(true);
	_flyingTime = 0.f;
}

void flyingState::end()
{
	_nextState = ePlayer_State_Falling;
}


//=============================================
// falling
//=============================================
HRESULT fallingState::init(player * p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = ePlayer_State_Falling;

	return S_OK;
}

void fallingState::release()
{
	playerState::release();
}

void fallingState::update()
{
	if (_player->isStateFloating())
	{
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
			_player->moveLeft();

		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			_player->moveRight();
		
		_player->moveDown();
	}
	else
	{
		end();
	}
}

void fallingState::render()
{
}

void fallingState::start()
{
	playerState::start();
	_player->setJumping(false);
}

void fallingState::end()
{
	_nextState = ePlayer_State_Land;
}


//==========================================
// landing
//==========================================
HRESULT landState::init(player * p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = ePlayer_State_Land;

	return S_OK;
}

void landState::release()
{
	playerState::release();
}

void landState::update()
{
	end();
}

void landState::render()
{
}

void landState::start()
{
	playerState::start();
}

void landState::end()
{
	_nextState = ePlayer_State_Idle;
}
