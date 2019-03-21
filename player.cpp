#include "stdafx.h"
#include "player.h"
#include "playerState.h"
#include "jumpState.h"
#include "attackState.h"
#include "mapData.h"
#include "terrain.h"
#include "animation.h"

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
	_atkRange = {100.f, 25.f};

	// 카메라
	RECTD2D rc = { 0.f, 0.f, WINSIZEX, WINSIZEY };
	POINTFLOAT pf = {MAPSIZEX, MAPSIZEY};
	CAMERA->init(pf, rc, PLAYER_MOVE_SPEED, &_x, &_y);

	initAnimaion();
	initState();

	return S_OK;
}

void player::release()
{
	_mapData = nullptr;
	_state = nullptr;
	_act = nullptr;

	map<UINT, playerState*>::iterator iter = _stateMap.begin();
	for (iter; _stateMap.end() != iter; ++iter)
	{
		playerState* st = iter->second;
		iter = _stateMap.erase(iter);

		SAFE_RELEASE(st);
		SAFE_DELETE(st);
	}
	_stateMap.clear();
}

void player::update()
{
	// 상태 갱신
	_state->update();
	if (_act)
	{
		_act->update();
		if (_act->isEnd())
		{
			_nextState = (ePlayer_State)_act->nextState();
			_act = nullptr;
		}
	}
	else
		_nextState = (ePlayer_State)_state->nextState();

	changeState(_nextState);

	fixPosition();
	updateCollision();

	DEVTOOL->pushBackDebugText(format(L"state : %d", _state->getState()));
}

void player::render()
{
	D2DMANAGER->drawRectangle(_collision, false);
	D2DMANAGER->drawRectangle(_collisionAtk, false);
	//
	D2DMANAGER->drawText(format(L"%d", _state->getState()).c_str(), _collision.left, _collision.top, false);
	if(_act)
		_act->render();
	else
		_state->render();
}

//===================================================================================================

void player::moveRight()
{
	_x += (PLAYER_MOVE_SPEED * TIMEMANAGER->getElapsedTime());
}

void player::moveLeft()
{
	_x -= (PLAYER_MOVE_SPEED * TIMEMANAGER->getElapsedTime());
}

void player::moveJump(float jumpPower)
{
	_y -= (jumpPower);
}

void player::moveFall(float gravity)
{
	_y += (gravity);
}

void player::moveUp()
{
	_y -= (PLAYER_MOVE_SPEED * TIMEMANAGER->getElapsedTime());
}

void player::moveDown()
{
	_y += (PLAYER_MOVE_SPEED * TIMEMANAGER->getElapsedTime());
}

void player::attack()
{
	if (_act)
		return;

	_act = findState(ePlayer_State_Attack);
	if (_act)
		_act->start();
}

void player::attackDamage()
{
	// enemy check
}

void player::standOff()
{
	_act = _stateMap[ePlayer_State_StandOff];
	_act->start();
}

void player::standOffDamage()
{
	// fire bullet
}

//===================================================================================================

bool player::checkDirection(eDirection dir)
{
	bool result = (_dir & dir) == dir;

	return result;
}

//===================================================================================================

void player::setDirectionRight()
{
	_dir |= eDirection_Right;
}

void player::setDirectionLeft()
{
	if (checkDirection(eDirection_Right))
		_dir ^= eDirection_Right;
}

void player::setDirectionUp()
{
	if (checkDirection(eDirection_Down))
		_dir ^= eDirection_Down;

	_dir |= eDirection_Up;
}

void player::setDirectionDown()
{
	if (checkDirection(eDirection_Up))
		_dir ^= eDirection_Up;

	_dir |= eDirection_Down;
}

void player::setDirectionIdle()
{
	if (checkDirection(eDirection_Right))
		_dir = eDirection_Right;
	else
		_dir = eDirection_Left;
}

//===================================================================================================

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
	{
		playerState* st = new jumpFallingState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_JumpFalling, st));
	}
	// landing
	{
		playerState* st = new landState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Land, st));
	}
	// attack : 근거리 공격
	{
		playerState* st = new attackState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Attack, st));
	}
	// attack : standOff : 원거리 공격
	{
		playerState* st = new standOffState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_StandOff, st));
	}


	// set idle
	_state = _stateMap[ePlayer_State_Idle];
}

void player::initAnimaion()
{
	// animation idle
	{
		image* img = IMAGEMANAGER->findImage("knight_idle");
		ANIMANAGER->addArrayFrameAnimation(	PLAYER_UID, ePlayer_State_Idle, "knight_idle"
										   ,0, img->GetMaxFrameX(), PLAYER_ANI_SPEED, true);
	}

	// animation walking
	{
		animation* anim = new animation;
		image* img = IMAGEMANAGER->findImage("knight_walk");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Walk, "knight_walk"
										   , 0, img->GetMaxFrameX(), PLAYER_ANI_SPEED, true);
	}

	// animation sit, drowse
	{
		image* img = IMAGEMANAGER->findImage("knight_sitAnddrowse");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Sit, "knight_sitAnddrowse"
										   , 0, 2, 60, true);
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Drowse, "knight_sitAnddrowse"
										   , 2, img->GetMaxFrameX(), PLAYER_ANI_SPEED, false);
	}

	// animation dead
	{
		image* img = IMAGEMANAGER->findImage("knight_dead");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Dead, "knight_dead"
										   , 0, img->GetMaxFrameX(), PLAYER_ANI_SPEED, false);
	}

	// animation jump, flying, falling, land
	{
		image* img = IMAGEMANAGER->findImage("knight_jump");

		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Flying, "knight_jump"
										   , 0, 4, PLAYER_ANI_SPEED, false);
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Falling, "knight_jump"
										   , 5, 7, PLAYER_ANI_SPEED, true);
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Land, "knight_jump"
										   , 8, img->GetMaxFrameX(), PLAYER_ANI_SPEED, false);
	}

	// animation attack 1 : 근접
	{
		image* img = IMAGEMANAGER->findImage("knight_attack1");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Attack_1, "knight_attack1"
										   , 0, img->GetMaxFrameX(), 2, PLAYER_ANI_SPEED_FAST, false);
	}

	// animation attack 2 : 근접
	{
		image* img = IMAGEMANAGER->findImage("knight_attack2");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Attack_2, "knight_attack2"
										   , 0, img->GetMaxFrameX(), 1, PLAYER_ANI_SPEED_FAST, false);
	}

	// animation attack up
	{
		image* img = IMAGEMANAGER->findImage("knight_attack_up");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Attack_Up, "knight_attack_up"
										   , 0, img->GetMaxFrameX(), 1, PLAYER_ANI_SPEED_FAST, false);
	}

	// animation attack down
	{
		image* img = IMAGEMANAGER->findImage("knight_attack_down");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Attack_Down, "knight_attack_down"
										   , 0, img->GetMaxFrameX(), 1, PLAYER_ANI_SPEED_FAST, false);
	}

	// animation attack 3 : 원거리
	{
		image* img = IMAGEMANAGER->findImage("knight_attack3");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_StandOff, "knight_attack3"
										   , 0, img->GetMaxFrameX(), 4, PLAYER_ANI_SPEED_SLOW, false);
	}

	// animation look up
	{
		image* img = IMAGEMANAGER->findImage("knight_lookup");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Look_Up, "knight_lookup"
										   , 0, img->GetMaxFrameX(), PLAYER_ANI_SPEED, true);
	}

	// animation look down
	{
		image* img = IMAGEMANAGER->findImage("knight_lookdown");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Look_Down, "knight_lookdown"
										   , 0, img->GetMaxFrameX(), PLAYER_ANI_SPEED, true);
	}
}

void player::changeState(ePlayer_State state)
{
	if(_state->getState() == state)
		return;

	playerState* pState = findState(state);
	if (pState)
	{
		_state = _stateMap[state];
		_state->start();
	}
}

void player::updateCollision()
{
	// 플레이어
	_collision = {  _x - PLAYER_COL_SIZE_WIDE_HALF, _y - PLAYER_COL_SIZE_HEIGHT
				  , _x + PLAYER_COL_SIZE_WIDE_HALF, _y };

	// 공격범위
	if (_act && _act->getState() == ePlayer_State_Attack)
	{
		switch (_dir_atk)
		{
			case eDirection_Up : 
			{
				_collisionAtk = { _x - _atkRange.y, _collision.top - _atkRange.x
								, _x + _atkRange.y, _collision.top };
				break; 
			}
			case eDirection_Down: 
			{
				_collisionAtk = { _x - _atkRange.y, _collision.bottom
								, _x + _atkRange.y, _collision.bottom + _atkRange.x };
				break; 
			}
			case eDirection_Right: 
			{
				_collisionAtk = { _collision.right, _y - PLAYER_COL_SIZE_HEIGHT_HALF - _atkRange.y
								, _collision.right + _atkRange.x, _y - PLAYER_COL_SIZE_HEIGHT_HALF + _atkRange.y };
				break; 
			}
			case eDirection_Left: 
			{
				_collisionAtk = { _collision.left - _atkRange.x, _y - PLAYER_COL_SIZE_HEIGHT_HALF - _atkRange.y
								, _collision.left, _y - PLAYER_COL_SIZE_HEIGHT_HALF + _atkRange.y };
				break; 
			}
		}
	}
	else
		_collisionAtk = {};

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
	_isFloating = true;
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

		updateCollision();
	}
}

playerState* player::findState(ePlayer_State state)
{
	playerState* pState = nullptr;

	if (_stateMap.find(state) != _stateMap.end())
		pState = _stateMap[state];

	return pState;
}
