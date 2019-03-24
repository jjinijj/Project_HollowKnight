#include "stdafx.h"
#include "player.h"
#include "jumpState.h"
#include "animation.h"

//=============================================
// jump start
//=============================================
flyingState::flyingState()
: _jumpPower(0.f)
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

	_state = player::ePlayer_State_Flying;
	setAnimation(player::ePlayer_Ani_Flying);

	return S_OK;
}

void flyingState::release()
{
	playerState::release();
}

void flyingState::update()
{
	playerState::update();

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
			_nextState = player::ePlayer_State_JumpFalling;

		_player->moveJump(_jumpPower);
	}
	else
		_nextState = player::ePlayer_State_JumpFalling;
}

void flyingState::render()
{
	playerState::render();
}

void flyingState::start()
{
	playerState::start();
	_jumpPower	= static_cast<float>(player::PLAYER_JUMP_POWER);
	_gravity	= static_cast<float>(player::PLAYER_GRAVITY);

	_ani->start();
	//SOUNDMANAGER->addSound("hero_jump","sound/hero_jump.wav", false, false);
	SOUNDMANAGER->play("hero_jump");
}

void flyingState::end()
{
	
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

	_state = player::ePlayer_State_Falling;
	setAnimation(player::ePlayer_Ani_Falling);

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
			_nextState = player::ePlayer_State_StandOff;
		}

		_fallingPower += (_gravity * TIMEMANAGER->getElapsedTime());
		if(player::PLAYER_JUMP_POWER < _fallingPower)
			_fallingPower = static_cast<float>(player::PLAYER_JUMP_POWER);
		_player->moveFall(_fallingPower);
	}
	else
	{
		_nextState = player::ePlayer_State_Land;
	}
}

void fallingState::render()
{
	playerState::render();
}

void fallingState::start()
{
	playerState::start();
	_fallingPower	= static_cast<float>(player::PLAYER_FALLING_POWER);
	_gravity		= static_cast<float>(player::PLAYER_GRAVITY);

	_ani->start();
	
}

void fallingState::end()
{
	
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

	_state = player::ePlayer_State_JumpFalling;
	setAnimation(player::ePlayer_Ani_Falling);

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
			_nextState = player::ePlayer_State_StandOff;
		}

		// falling
		_jumpPower += (_gravity * TIMEMANAGER->getElapsedTime());
		if (player::PLAYER_JUMP_POWER <= _jumpPower)
			_jumpPower = static_cast<float>(player::PLAYER_JUMP_POWER);

		_player->moveFall(_jumpPower);
	}
	else
	{
		_nextState = player::ePlayer_State_Land;
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
	_gravity	= static_cast<float>(player::PLAYER_GRAVITY);

	_ani->start();
}

void jumpFallingState::end()
{
	
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

	_state = player::ePlayer_State_Land;
	setAnimation(player::ePlayer_Ani_Land);

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
		_nextState = player::ePlayer_State_Idle;
}

void landState::render()
{
	playerState::render();
}

void landState::start()
{
	playerState::start();
	_ani->start();
	//SOUNDMANAGER->addSound("hero_land_soft","sound/hero_land_soft.wav", false, false);
	SOUNDMANAGER->play("hero_land_soft");
}

void landState::end()
{
	
}
