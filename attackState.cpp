#include "stdafx.h"
#include "attackState.h"
#include "player.h"


//=============================================
// attack : 근거리
//=============================================
attackState::attackState()
: _isRight(false)
{
}

attackState::~attackState()
{
}

HRESULT attackState::init(player * p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);
	_state = ePlayer_State_Attack;

	return S_OK;
}

void attackState::release()
{
	playerState::release();
}

void attackState::update()
{
	_ani->frameUpdate(TIMEMANAGER->getElapsedTime());
	_isEnd = !(_ani->isPlay());
	if (_isEnd)
	{
		end();
		if (_player->isStateFloating())
			_state = ePlayer_State_Falling;
	}
	
}

void attackState::render()
{
}

void attackState::start()
{
	playerState::start();
	_isRight = (_player->getDirection() & eDirection_Right) == eDirection_Right;
}

void attackState::end()
{
	_state = ePlayer_State_Idle;
}


//=============================================
// standOff : 원거리
//=============================================
standOffState::standOffState()
: _isRight(false)
, _floatingTime(0.f)
{
}

standOffState::~standOffState()
{
}

HRESULT standOffState::init(player * p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);
	_state = ePlayer_State_Attack;

	return S_OK;
}

void standOffState::release()
{
	playerState::release();
}

void standOffState::update()
{
}

void standOffState::render()
{
}

void standOffState::start()
{
	playerState::start();
	_isRight = (_player->getDirection() & eDirection_Right) == eDirection_Right;
}

void standOffState::end()
{
	_state = ePlayer_State_Idle;
}
