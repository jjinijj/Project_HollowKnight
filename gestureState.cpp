#include "stdafx.h"
#include "gestureState.h"
#include "player.h"
#include "animation.h"

//=============================================
// look up
//=============================================
HRESULT lookUpState::init(player* p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = player::ePlayer_State_Look_Up;
	setAnimation(player::ePlayer_Ani_Look_Up);

	return S_OK;
}

void lookUpState::update()
{
	playerState::update();

	if(!_ani->isPlay())
		end();
}

void lookUpState::start()
{
	playerState::start();
	_ani->start();
}

void lookUpState::end()
{
	_nextState = player::ePlayer_State_Look_Stay;
}

HRESULT lookUpStayState::init(player * p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = player::ePlayer_State_Look_Stay;
	setAnimation(player::eplayer_Ani_Look_Up_Loop);

	return S_OK;
}

void lookUpStayState::update()
{
	playerState::update();

	// 위,아래
	setUpAndDownDirection();

	if(_player->checkDirection(player::eDirection_Up))
		_player->lookUp();
	else
		end();
}

void lookUpStayState::start()
{
	playerState::start();
	_ani->start();
}

void lookUpStayState::end()
{
	_nextState = player::ePlayer_State_Idle;
}


//=============================================
// look down
//=============================================
HRESULT lookDownState::init(player* p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = player::ePlayer_State_Look_Down;
	setAnimation(player::ePlayer_Ani_Look_Down);

	return S_OK;
}

void lookDownState::update()
{
	playerState::update();
	
	// 위,아래
	setUpAndDownDirection();

	if(_player->checkDirection(player::eDirection_Down))
		_player->lookDown();
	else
		end();
}

void lookDownState::start()
{
	playerState::start();
	_ani->start();
}

void lookDownState::end()
{
	_nextState = player::ePlayer_State_Idle;
}


//=============================================
// talk
//=============================================
HRESULT talkState::init(player* p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = player::ePlayer_State_Talk;
	setAnimation(player::ePlayer_Ani_Look_Up);

	return S_OK;
}

void talkState::update()
{
	playerState::update();
	if (!_ani->isPlay())
	{
		setAnimation(player::eplayer_Ani_Look_Up_Loop);
	}

	if (KEYMANAGER->isOnceKeyDown('X'))
	{
		if(_player->isTalkEnd())
			end();
		else
			_player->nextTalk();
	}
}

void talkState::start()
{
	playerState::start();
	_ani->start();
	_player->talkStart();
}

void talkState::end()
{
	_nextState = player::ePlayer_State_Idle;
	_player->endTalk();
}

