#include "stdafx.h"
#include "player.h"
#include "jumpState.h"
#include "animation.h"

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
	setAnimation(ePlayer_Ani_Flying);

	return S_OK;
}

void flyingState::release()
{
	playerState::release();
}

void flyingState::update()
{
	playerState::update();

	//if( 1.0f <= _flyingTime)
	//	end();
	//else
	{
		// 이동
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
			moveLeft();

		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			moveRight();

		// 위,아래
		setUpAndDownDirection();

		// 누르고 있는 일정기간동안 상승
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
	playerState::render();
}

void flyingState::start()
{
	playerState::start();
	_flyingTime = 0.f;
	_jumpPower	= static_cast<float>(PLAYER_JUMP_POWER);
	_gravity	= static_cast<float>(PLAYER_GRAVITY);

	_ani->start();
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
	setAnimation(ePlayer_Ani_Falling);

	return S_OK;
}

void fallingState::release()
{
	playerState::release();
}

void fallingState::update()
{
	playerState::update();

	if (_player->isStateFloating())
	{
		// 이동
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
			moveLeft();
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			moveRight();

		// 위,아래
		setUpAndDownDirection();

		// 공격
		if (KEYMANAGER->isOnceKeyDown('X'))			// 근거리 공격
			_player->attack();
		else if (KEYMANAGER->isOnceKeyDown('A'))	// 원거리 공격
		{
			_player->standOff();
			_nextState = ePlayer_State_StandOff;
		}

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
	playerState::render();
}

void fallingState::start()
{
	playerState::start();
	_fallingPower	= static_cast<float>(PLAYER_FALLING_POWER);
	_gravity		= static_cast<float>(PLAYER_GRAVITY);

	_ani->start();
}

void fallingState::end()
{
	_nextState = ePlayer_State_Land;
}



//=============================================
// falling after jump
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
	setAnimation(ePlayer_Ani_Falling);

	return S_OK;
}

void jumpFallingState::release()
{
	playerState::release();
}

void jumpFallingState::update()
{
	playerState::update();

	// 공중에 떠 있다면
	if (_player->isStateFloating())
	{
		// 이동
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
			moveLeft();
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			moveRight();

		// 위,아래
		setUpAndDownDirection();

		// 공격
		if (KEYMANAGER->isOnceKeyDown('X'))			// 근거리 공격
			_player->attack();
		else if (KEYMANAGER->isOnceKeyDown('A'))	// 원거리 공격
		{
			_player->standOff();
			_nextState = ePlayer_State_StandOff;
		}

		// falling
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
	playerState::render();
}

void jumpFallingState::start()
{
	playerState::start();
	_jumpPower	= 0.f;
	_gravity	= static_cast<float>(PLAYER_GRAVITY);

	_ani->start();
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
	setAnimation(ePlayer_Ani_Land);

	return S_OK;
}

void landState::release()
{
	playerState::release();
}

void landState::update()
{
	playerState::update();

	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		moveLeft();

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		moveRight();
	
	if(_isEnd)
		end();
}

void landState::render()
{
	playerState::render();
}

void landState::start()
{
	playerState::start();
	_ani->start();
}

void landState::end()
{
	_nextState = ePlayer_State_Idle;
}
