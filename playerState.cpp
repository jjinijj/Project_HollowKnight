#include "stdafx.h"
#include "playerState.h"
#include "player.h"
#include "animation.h"


playerState::playerState()
: _player(nullptr)
, _img(nullptr)
, _ani(nullptr)
, _nextState(player::ePlayer_State_None)
, _state(player::ePlayer_State_None)
, _aniKey(player::ePlayer_Ani_None)
, _isEnd(false)
, _isDoEvent(false)
, _alpha(0.f)
{
}

playerState::~playerState()
{
}

HRESULT playerState::init(player* p)
{
	_player = p;
	assert(nullptr != _player);

	_alpha = 1.0f;
	return S_OK;
}

void playerState::release()
{
	_player = nullptr;
	_img = nullptr;
	_ani = nullptr;
}

void playerState::update()
{
	if (_ani->isPlay())
		_ani->frameUpdate(TIMEMANAGER->getElapsedTime());
	else
		_isEnd = true;
}

void playerState::render()
{
	if(_isRight)
		_img->aniRender( _player->getPosX() - player::PLAYER_SIZE_WIDE_HALF
						,_player->getPosY() - player::PLAYER_SIZE_HEIGHT
						, _ani, _alpha, false);
	else
		_img->aniRenderReverseX( _player->getPosX() - player::PLAYER_SIZE_WIDE_HALF
								,_player->getPosY() - player::PLAYER_SIZE_HEIGHT
								,_ani, _alpha, false);
}

void playerState::start()
{
	_nextState = player::ePlayer_State_None;
	_isRight = _player->checkDirection(player::eDirection_Right);
	_isEnd = false;
	_isDoEvent = false;
	if(_ani)
		_ani->stop();
}

void playerState::end()
{
}

UINT playerState::nextState()
{
	return _nextState;
}

void playerState::setAnimation(UINT aniKey)
{
	_aniKey = aniKey;
	_ani = ANIMANAGER->findAnimation(PLAYER_UID, _aniKey);
	assert(nullptr != _ani);

	_img = _ani->getImage();
}

void playerState::moveLeft()
{
	_player->setDirectionLeft();
	_isRight = false;
	_player->moveLeft();
}

void playerState::moveRight()
{
	_player->setDirectionRight();
	_isRight = true;
	_player->moveRight();
}

void playerState::setUpAndDownDirection()
{
	if (KEYMANAGER->isStayKeyDown(VK_UP))
		_player->setDirectionUp();
	else if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		_player->setDirectionDown();
	else
		_player->setDirectionIdle();
}



//=============================================
// idle
//=============================================

idleState::idleState()
{
}

idleState::~idleState()
{
}

HRESULT idleState::init(player* p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);
	
	_state = player::ePlayer_State_Idle;
	setAnimation(player::ePlayer_Ani_Idle);

	return S_OK;
}

void idleState::release()
{
	playerState::release();
}

void idleState::update()
{
	playerState::update();

	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_nextState = player::ePlayer_State_Walk;
		_player->setDirectionLeft();
	}
	
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_nextState = player::ePlayer_State_Walk;
		_player->setDirectionRight();
	}

	setUpAndDownDirection();

	if (KEYMANAGER->isOnceKeyDown('Z'))
		_nextState = player::ePlayer_State_Flying;
	else if (KEYMANAGER->isOnceKeyDown('X'))
		_player->attack();
	else if (KEYMANAGER->isOnceKeyDown('A'))
	{
		_player->standOff();
		_nextState = player::ePlayer_State_StandOff;
	}
	else if (_player->checkDirection(player::eDirection_Up))
	{
		if (_player->checkPossibleTalk())
			_nextState = player::ePlayer_State_Talk;
		else if (_player->trySit())
			_nextState = player::ePlayer_State_Sit;
		else if (_player->checkPortal())
			_player->enterPortal();
		else
			_nextState = player::ePlayer_State_Look_Up;
	}
	else if (_player->checkDirection(player::eDirection_Down))
		_nextState = player::ePlayer_State_Look_Down;

	if (_player->isStateFloating())
		_nextState = player::ePlayer_State_Falling;
}

void idleState::render()
{
	playerState::render();
}

void idleState::start()
{
	playerState::start();
	_ani->start();
}

void idleState::end()
{
}

UINT idleState::nextState()
{
	return _nextState;
}


//=============================================
// walk
//=============================================

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
	
	_state = player::ePlayer_State_Walk;
	setAnimation(player::ePlayer_Ani_Walk);

	return S_OK;
}

void walkState::release()
{
	playerState::release();
}

void walkState::update()
{
	playerState::update();

	if(_isRight)
		rightMove();
	else
		leftMove();

	setUpAndDownDirection();

	if (KEYMANAGER->isOnceKeyDown('Z'))
		_nextState = player::ePlayer_State_Flying;
	else if (KEYMANAGER->isOnceKeyDown('X'))
		_player->attack();
	else if (KEYMANAGER->isOnceKeyDown('A'))
	{
		_player->standOff();
		_nextState = player::ePlayer_State_StandOff;
	}

	if (_player->checkDirection(player::eDirection_Up))
	{
		if (_player->checkPossibleTalk())
			_nextState = player::ePlayer_State_Talk;
		else if (_player->trySit())
			_nextState = player::ePlayer_State_Sit;
		else if (_player->checkPortal())
			_player->enterPortal();
	}

	if (_player->isStateFloating() && player::ePlayer_State_None == _nextState)
		_nextState = player::ePlayer_State_Falling;
}

void walkState::render()
{
	playerState::render();
}

void walkState::start()
{
	playerState::start();
	_ani->start();
}

void walkState::end()
{
	_nextState = player::ePlayer_State_Idle;
}

UINT walkState::nextState()
{
	return _nextState;
}

void walkState::rightMove()
{
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		_player->moveRight();
	else
		end();
}

void walkState::leftMove()
{
	if(KEYMANAGER->isStayKeyDown(VK_LEFT))
		_player->moveLeft();
	else
		end();
}


//=============================================
// take damage
//=============================================
HRESULT takeDamage::init(player* p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = player::ePlayer_State_Dead;
	setAnimation(player::ePlayer_Ani_Dead);

	return S_OK;
}

void takeDamage::update()
{
	playerState::update();
	if(!_ani->isPlay())
		end();
}

void takeDamage::end()
{
	_player->regen();
}

//=============================================
// dead
//=============================================
HRESULT deadState::init(player * p)
{
	HRESULT hr = playerState::init(p);
	assert(S_OK == hr);

	_state = player::ePlayer_State_Dead;
	setAnimation(player::ePlayer_Ani_Dead);

	return S_OK;
}

void deadState::update()
{
	playerState::update();
	if(!_ani->isPlay())
		end();
}

void deadState::start()
{
	playerState::start();
	_ani->start();
}

void deadState::end()
{
	_player->regen();
}

