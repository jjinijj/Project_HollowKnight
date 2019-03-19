#include "stdafx.h"
#include "player.h"
#include "jumpState.h"

//=============================================
// jump start
//=============================================
flyingState::flyingState()
: _flyingTime(0.f)
, _jumpPower(0.f)
, _gravity(0.f)
{
}

flyingState::~flyingState()
{
}

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
	//if( 1.0f <= _flyingTime)
	//	end();
	//else
	{
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
			_player->moveLeft();

		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			_player->moveRight();

		if (KEYMANAGER->isStayKeyDown('Z'))
		{
			_jumpPower -= (_gravity * TIMEMANAGER->getElapsedTime());
			if(_jumpPower < 0.f)
				end();

			_flyingTime += TIMEMANAGER->getElapsedTime();
			_player->moveJump(_jumpPower);
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
	_flyingTime = 0.f;
	_jumpPower	= static_cast<float>(PLAYER_JUMP_POWER);
	_gravity	= static_cast<float>(PLAYER_GRAVITY);
}

void flyingState::end()
{
	_nextState = ePlayer_State_JumpFalling;
}


//=============================================
// falling
//=============================================
fallingState::fallingState()
: _fallingPower(0.f)
, _gravity(0.f)
{
}

fallingState::~fallingState()
{
}


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

		_fallingPower += (_gravity * TIMEMANAGER->getElapsedTime());
		if(PLAYER_JUMP_POWER < _fallingPower)
			_fallingPower = static_cast<float>(PLAYER_JUMP_POWER);
		_player->moveFall(_fallingPower);
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
	_fallingPower	= static_cast<float>(PLAYER_FALLING_POWER);
	_gravity		= static_cast<float>(PLAYER_GRAVITY);
}

void fallingState::end()
{
	_nextState = ePlayer_State_Land;
}



//=============================================
// falling after jumping
//=============================================
jumpFallingState::jumpFallingState()
: _jumpPower(0.f)
, _gravity(0.f)
{
}

jumpFallingState::~jumpFallingState()
{
}

HRESULT jumpFallingState::init(player * p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = ePlayer_State_JumpFalling;
	return S_OK;
}

void jumpFallingState::release()
{
	playerState::release();
}

void jumpFallingState::update()
{
	if (_player->isStateFloating())
	{
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
			_player->moveLeft();

		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			_player->moveRight();

		_jumpPower += (_gravity * TIMEMANAGER->getElapsedTime());
		if (PLAYER_JUMP_POWER <= _jumpPower)
			_jumpPower = static_cast<float>(PLAYER_JUMP_POWER);

		_player->moveFall(_jumpPower);
	}
	else
	{
		end();
	}
}

void jumpFallingState::render()
{
}

void jumpFallingState::start()
{
	playerState::start();
	_jumpPower	= 0.f;
	_gravity	= static_cast<float>(PLAYER_GRAVITY);
}

void jumpFallingState::end()
{
	_nextState = ePlayer_State_Land;
}




//==========================================
// landing
//==========================================
landState::landState()
{
}

landState::~landState()
{
}

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
