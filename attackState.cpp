#include "stdafx.h"
#include "attackState.h"
#include "player.h"
#include "animation.h"


//=============================================
// attack : 근거리
//=============================================
attackState::attackState()
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
	playerState::update();

	if (_isEnd)
	{
		if (_player->isStateFloating())
			_nextState = ePlayer_State_Falling;
		else
			end();
	}
	else if (_ani->isEventFrame())
	{
		if (!_isDoEvent)
		{
			_player->attackDamage();
			_isDoEvent = true;
		}
	}
	else if (_isDoEvent)
	{
		_isDoEvent = false;
	}
}

void attackState::render()
{
	playerState::render();
}

void attackState::start()
{
	playerState::start();

	// 방향키가 눌린 방향으로 공격
	if (_player->checkDirection(eDirection_Up))
	{
		_aniKey = ePlayer_Ani_Attack_Up;
		_player->setAttkDirection(eDirection_Up);
	}
	else if (_player->checkDirection(eDirection_Down))
	{
		_aniKey = ePlayer_Ani_Attack_Down;
		_player->setAttkDirection(eDirection_Down);
	}
	else
	{
		// 2개의 공격모션 중 랜덤 출력
		int value = RND->getInt(100);
		if(value < 50)
			_aniKey = ePlayer_Ani_Attack_1;
		else
			_aniKey = ePlayer_Ani_Attack_2;

		if(_isRight)
			_player->setAttkDirection(eDirection_Right);
		else
			_player->setAttkDirection(eDirection_Left);
	}
	
	setAnimation(_aniKey);
	_ani->start();
}

void attackState::end()
{
	_nextState = ePlayer_State_Idle;
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
	_state = ePlayer_State_StandOff;
	
	setAnimation(ePlayer_Ani_StandOff);

	return S_OK;
}

void standOffState::release()
{
	playerState::release();
}

void standOffState::update()
{
	playerState::update();

	if (_isEnd)
	{
		if (_player->isStateFloating())
			_nextState = ePlayer_State_Falling;
		else 
			end();
			
	}
	else if (_ani->isEventFrame())
	{
		if (!_isDoEvent)
		{
			_player->standOffDamage();
			_isDoEvent = true;
		}
	}
}

void standOffState::render()
{
	playerState::render();
}

void standOffState::start()
{
	playerState::start();
	_ani->start();
}

void standOffState::end()
{
	_nextState = ePlayer_State_Idle;
}
