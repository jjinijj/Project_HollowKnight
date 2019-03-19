#include "stdafx.h"
#include "playerAction.h"
#include "player.h"

playerAction::playerAction()
: _isPlay(false)
, _isRight(false)
, _isDoEvent(false)
, _player(nullptr)
, _nextState(ePlayer_State_None)
{
}

playerAction::~playerAction()
{
}

HRESULT playerAction::init(player* p)
{
	_player = p;
	assert(nullptr != _player);

	return S_OK;
}

void playerAction::release()
{
	_player = nullptr;
}

void playerAction::update()
{
}

void playerAction::render()
{
}

void playerAction::start()
{
	_isPlay		= true;
	_isRight	= (_player->getDirection() & eDirection_Right) == eDirection_Right;
	_isDoEvent	= false;
	_nextState	= ePlayer_State_None;
}

void playerAction::end()
{
	_nextState = ePlayer_State_Idle;
}
