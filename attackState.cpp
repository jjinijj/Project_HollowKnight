#include "stdafx.h"
#include "attackState.h"
#include "player.h"
#include "animation.h"


//=============================================
// attack : �ٰŸ�
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

	_state = player::ePlayer_State_Attack;

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
			_nextState = player::ePlayer_State_Falling;
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

	// ����Ű�� ���� �������� ����
	if (_player->checkDirection(player::eDirection_Up))
	{
		_aniKey = player::ePlayer_Ani_Attack_Up;
		_player->setAttkDirection(player::eDirection_Up);
	}
	else if (_player->checkDirection(player::eDirection_Down))
	{
		_aniKey = player::ePlayer_Ani_Attack_Down;
		_player->setAttkDirection(player::eDirection_Down);
	}
	else
	{
		// 2���� ���ݸ�� �� ���� ���
		int value = RND->getInt(100);
		if(value < 50)
			_aniKey = player::ePlayer_Ani_Attack_1;
		else
			_aniKey = player::ePlayer_Ani_Attack_2;

		if(_isRight)
			_player->setAttkDirection(player::eDirection_Right);
		else
			_player->setAttkDirection(player::eDirection_Left);
	}
	
	setAnimation(_aniKey);
	_ani->start();
}

void attackState::end()
{
	_nextState = player::ePlayer_State_Idle;
}


//=============================================
// standOff : ���Ÿ�
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
	_state = player::ePlayer_State_StandOff;
	
	setAnimation(player::ePlayer_Ani_StandOff);

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
			_nextState = player::ePlayer_State_Falling;
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
	_nextState = player::ePlayer_State_Idle;
}
