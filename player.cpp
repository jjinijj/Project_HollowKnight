#include "stdafx.h"
#include "player.h"
#include "playerState.h"
#include "jumpState.h"
#include "mapData.h"
#include "terrain.h"

player::player()
{
}


player::~player()
{
}

HRESULT player::init(float x, float y)
{
	_x = x;
	_y = y;

	_collision = { x, y, x + 10.f, y + 10.f };

	RECTD2D rc = { 0.f, 0.f, WINSIZEX, WINSIZEY };
	POINTFLOAT pf = {WINSIZEX, WINSIZEY};
	//CAMERA->init(pf, rc, (float)PLAYER_MOVE_SPEED, &_x, &_y);

	CAMERA->init(pf, rc, 0.f);

	initState();

	return S_OK;
}

void player::release()
{
}

void player::update()
{
	_state->update();
	_nextState = _state->nextState();
	changeState(_nextState);

	fixPosition();
	updateCollision();

	DEVTOOL->pushBackDebugText(format(L"state : %d", _state->getState()));
}

void player::render()
{
	D2DMANAGER->drawRectangle(_collision, false);

	if(_actState)
		_actState->render();
	else
		_state->render();
}

void player::moveRight()
{
	_x += PLAYER_MOVE_SPEED;
}

void player::moveLeft()
{
	_x -= PLAYER_MOVE_SPEED;
}

void player::moveUp()
{
	_y -= PLAYER_MOVE_SPEED;
}

void player::moveDown()
{
	_y += PLAYER_MOVE_SPEED;
}

void player::initState()
{
	// idle
	{
		playerState* st = new idleState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Idle, st));
	}
	// walk
	{
		playerState* st = new walkState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Walk, st));
	}
	// flying
	{
		playerState* st = new flyingState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Flying, st));
	}
	// falling
	{
		playerState* st = new fallingState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Falling, st));
	}
	// landing
	{
		playerState* st = new landState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Land, st));
	}

	//
	_state = _stateMap[ePlayer_State_Idle];
}

void player::changeState(UINT state)
{
	if(_stateMap.find(state) == _stateMap.end())
		return;

	if(_state->getState() == state)
		return;

	_state = _stateMap[state];
	_state->start();
}

void player::updateCollision()
{
	// 플레이어
	_collision = {  _x - PLAYER_COL_SIZE_WIDE_HALF, _y - PLAYER_COL_SIZE_HEIGHT
				  , _x + PLAYER_COL_SIZE_WIDE_HALF, _y };

	// 공격범위
	//if (ePLAYER_STATE_ATTACK_1 == _state || ePLAYER_STATE_ATTACK_2 == _state)
	//{
	//	if (eDIRECTION_RIGHT == _dir_LR)
	//		_collisionAtk = { _collision.right, (int)_position.y - PLAYER_COL_SIZE_HEIGHT_HALF - _atkRange.y
	//						, _collision.right + _atkRange.x, (int)_position.y - PLAYER_COL_SIZE_HEIGHT_HALF + _atkRange.y };
	//	else if (eDIRECTION_LEFT == _dir_LR)
	//		_collisionAtk = { _collision.left - _atkRange.x, (int)_position.y - PLAYER_COL_SIZE_HEIGHT_HALF - _atkRange.y
	//						, _collision.left, (int)_position.y - PLAYER_COL_SIZE_HEIGHT_HALF + _atkRange.y };
	//}
	//else if (ePLAYER_STATE_ATTACK_UP == _state)
	//{
	//	_collisionAtk = { (int)_position.x - _atkRange.y, _collision.top - _atkRange.x
	//					, (int)_position.x + _atkRange.y, _collision.top };
	//}
	//else if (ePLAYER_STATE_ATTACK_DOWN == _state)
	//{
	//	_collisionAtk = { (int)_position.x - _atkRange.y, _collision.bottom
	//					, (int)_position.x + _atkRange.y, _collision.bottom + _atkRange.x };
	//}
	//else
	//	_collisionAtk = {};

}

void player::fixPosition()
{
	vector<terrain*>* vCol = _mapData->getColliderTerrains();
	vector<terrain*>::iterator iter = vCol->begin();
	vector<terrain*>::iterator end = vCol->end();

	float offsetX = 0.f;
	float offsetY = 0.f;
	terrain* ter = nullptr;
	RECTD2D rc = {};
	for (iter; end != iter; ++iter)
	{
		ter = (*iter);
		rc = ter->getRect();

		if (!CheckIntersectRect(rc, _collision))
			continue;

		offsetX = GetIntersectOffsetX(_collision, rc);
		offsetY = GetIntersectOffsetY(_collision, rc);

		// 상하
		if (rc.left <= _collision.left && _collision.right < rc.right)
		{
			_y += offsetY;
			if (offsetY <= 0)
				_isFloating = false;
		}
		// 좌우
		else if (rc.top <= _collision.top && _collision.bottom <= rc.bottom)
			_x += offsetX;
		// 모서리
		else if (abs(offsetX) < abs(offsetY))
			_x += offsetX;
		// 모서리
		else
		{
			// 정확히 모서리
			if (_collision.bottom == rc.top)
			{
				if (_collision.left == rc.right || _collision.right == rc.left)
				{
					_isFloating = true;
					continue;
				}
			}

			_y += offsetY;

			// 발에 닿음
			if (offsetY <= 0.f)
				_isFloating = false;
		}
	}
}
