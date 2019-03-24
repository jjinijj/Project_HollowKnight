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
{
}

playerState::~playerState()
{
}

HRESULT playerState::init(player* p)
{
	_player = p;
	assert(nullptr != _player);

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
						, _ani, 1.0f, false);
	else
		_img->aniRenderReverseX( _player->getPosX() - player::PLAYER_SIZE_WIDE_HALF
								,_player->getPosY() - player::PLAYER_SIZE_HEIGHT
								,_ani, 1.0f, false);
}

void playerState::start()
{
	_nextState = player::ePlayer_State_None;
	_isRight = _player->checkDirection(player::eDirection_Right);
	_isEnd = false;
	_isDoEvent = false;
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

	if (KEYMANAGER->isStayKeyDown(VK_UP))
		_player->setDirectionUp();
	else if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		_player->setDirectionDown();
	else
		_player->setDirectionIdle();

	if (KEYMANAGER->isOnceKeyDown('Z'))
		_nextState = player::ePlayer_State_Flying;
	else if (KEYMANAGER->isOnceKeyDown('X'))
		_player->attack();
	else if (KEYMANAGER->isOnceKeyDown('A'))
	{
		_player->standOff();
		_nextState = player::ePlayer_State_StandOff;
	}
	else if (_player->checkDirection(player::eDirection_Up) || _player->checkDirection(player::eDirection_Down))
		_nextState = player::ePlayer_State_Look;

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

	if (KEYMANAGER->isStayKeyDown(VK_UP))
		_player->setDirectionUp();
	else if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		_player->setDirectionDown();
	else
		_player->setDirectionIdle();


	if (KEYMANAGER->isOnceKeyDown('Z'))
		_nextState = player::ePlayer_State_Flying;
	else if (KEYMANAGER->isOnceKeyDown('X'))
		_player->attack();
	else if (KEYMANAGER->isOnceKeyDown('A'))
	{
		_player->standOff();
		_nextState = player::ePlayer_State_StandOff;
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