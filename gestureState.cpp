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
		_nextState = player::ePlayer_State_Look_Stay;
}

void lookUpState::start()
{
	playerState::start();
	_ani->start();
}

void lookUpState::end()
{
	
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

	if(KEYMANAGER->isOnceKeyDown('X'))
		_nextState = player::ePlayer_State_Attack;
	else if (KEYMANAGER->isOnceKeyDown('Z'))
		_nextState = player::ePlayer_State_Flying;
	else if(_player->checkDirection(player::eDirection_Up))
		_player->lookUp();
	else
		_nextState = player::ePlayer_State_Idle;

	if(player::ePlayer_State_None != _nextState)
		_player->sightResetDown();
}

void lookUpStayState::start()
{
	playerState::start();
	_ani->start();
}

void lookUpStayState::end()
{
	
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

	if(!_ani->isPlay())
		_nextState = player::ePlayer_State_Look_Down_Stay;
}

void lookDownState::start()
{
	playerState::start();
	_ani->start();
}

void lookDownState::end()
{
	
}


HRESULT lookDownStayState::init(player * p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = player::ePlayer_State_Look_Down_Stay;
	setAnimation(player::eplayer_Ani_Look_Down_Loop);

	return S_OK;
}

void lookDownStayState::update()
{
	playerState::update();

	// 위,아래
	setUpAndDownDirection();

	if(KEYMANAGER->isOnceKeyDown('X'))
		_nextState = player::ePlayer_State_Attack;
	else if (KEYMANAGER->isOnceKeyDown('Z'))
		_nextState = player::ePlayer_State_Flying;
	else if(_player->checkDirection(player::eDirection_Down))
		_player->lookDown();
	else
		_nextState = player::ePlayer_State_Idle;

	if(player::ePlayer_State_None != _nextState)
		_player->sightResetUp();
}

void lookDownStayState::start()
{
	playerState::start();
	_ani->start();
}

void lookDownStayState::end()
{
	
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
		if (_player->isTalkEnd())
		{
			_nextState = player::ePlayer_State_Idle;
			_player->endTalk();
		}
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
	
}

HRESULT sitState::init(player * p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = player::ePlayer_State_Sit;
	setAnimation(player::ePlayer_Ani_Sit);


	return S_OK;
}

void sitState::update()
{
	playerState::update();
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT) || KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		_nextState = player::ePlayer_State_Idle;
	}
	else
	{
		_sitTime += TIMEMANAGER->getElapsedTime();
		if(3.f <= _sitTime)
			_nextState = player::ePlayer_State_Drowse;
	}
}

void sitState::render()
{
	float x = _player->getPosX();
	float y = _player->getPosY();

	_img->aniRender( _player->getPosX() - player::PLAYER_SIZE_WIDE_HALF
					,_player->getPosY() - player::PLAYER_SIZE_HEIGHT
					, _ani, _alpha, false);
}

void sitState::start()
{
	playerState::start();
	_sitTime = 0.f;
	_ani->start();
}

void sitState::end()
{
	playerState::end();
	
}

HRESULT drowseState::init(player* p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = player::ePlayer_State_Drowse;
	setAnimation(player::ePlayer_Ani_Drowse);

	return S_OK;
}

void drowseState::update()
{
	playerState::update();
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		_player->setDirectionLeft();
		_nextState = player::ePlayer_State_Sit;
	}
	else if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		_player->setDirectionRight();
		_nextState = player::ePlayer_State_Sit;
	}
}

void drowseState::start()
{
	playerState::start();
	_ani->start();
}

void drowseState::end()
{
	
}
