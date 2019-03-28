#include "stdafx.h"
#include "player.h"
#include "playerState.h"
#include "jumpState.h"
#include "attackState.h"
#include "gestureState.h"
#include "mapData.h"
#include "terrain.h"
#include "animation.h"
#include "enemy.h"
#include "npc.h"
#include "actorManager.h"
#include "windowDialog.h"
#include "playerStatusUI.h";
#include "objectManager.h"
#include "dropObject.h"

player::player()
{
}


player::~player()
{
}

HRESULT player::init()
{
	_x = 0.f;
	_y = 0.f;
	_renderAlpha = 1.0f;
	initAnimaion();
	initState();


	return S_OK;
}

HRESULT player::init(float x, float y)
{
	_x = x;
	_y = y;

	_startX = x;
	_startY = y;

	_renderAlpha = 1.0f;

	_collision = { x, y, x + 10.f, y + 10.f };
	_atkRange = {100.f, 25.f};

	// 카메라
	RECTD2D rc = { 0.f, 0.f, WINSIZEX, WINSIZEY };
	POINTFLOAT pf = {MAPSIZEX, MAPSIZEY};
	CAMERA->init(pf, rc, PLAYER_MOVE_SPEED, &_x, &_y);

	initAnimaion();
	initState();

	_power = 1;
	_hp = PLAYER_INIT_HP;
	_skillValue = 0;
	_skillMax = PLAYER_SKILL_VALUE;

	_sight = 0.f;
	_function = NULL;

	_vSwordSound.push_back("sword_1");
	_vSwordSound.push_back("sword_2");
	_vSwordSound.push_back("sword_3");
	_vSwordSound.push_back("sword_4");

	return S_OK;
}

void player::release()
{
	_mapData = nullptr;
	_state = nullptr;
	_act = nullptr;

	map<UINT, playerState*>::iterator iter = _stateMap.begin();
	for (iter; _stateMap.end() != iter;)
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
	if (0.f < _invincibleTime)
	{
		_invincibleTime -= TIMEMANAGER->getElapsedTime();
		_renderAlpha = _tempValue % 10 * 0.1f;
		++_tempValue;
	}
	else
		_renderAlpha = 1.0f;
	

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
	checkCollisionEnemy();
	checkCollisionCoin();
	if(checkPortal())
		enterPortalTrigger();

	// 
	if(_function)
		_function();

	// 이펙트
	if (_effectAni)
	{
		if (!_effectAni->isPlay())
		{
			_effectAni = nullptr;
			_effectImg = nullptr;
		}
		else
		{
			switch (_dir_atk)
			{
				case eDirection_Right:
				{
					_effectPos.x = _collisionAtk.right - 100;
					_effectPos.y = _collisionAtk.top;
					break;
				}

				case eDirection_Left:
				{
					_effectPos.x = _collisionAtk.left;
					_effectPos.y = _collisionAtk.top;
					break;
				}

				case eDirection_Up:
				{
					_effectPos.x = _collision.left - 50;
					_effectPos.y = _collisionAtk.top;
					break;
				}

				case eDirection_Down:
				{
					_effectPos.x = _collision.left - 50;
					_effectPos.y = _collisionAtk.bottom - 120;
					break;
				}
			}

			_effectAni->frameUpdate(TIMEMANAGER->getElapsedTime());
		}
	}

	DEVTOOL->pushBackDebugText(format(L"state : %d", _state->getState()));

}

void player::render()
{
	if (DEVTOOL->checkDebugMode(DEBUG_SHOW_RECT))
	{
		D2DMANAGER->drawRectangle(_collision, false);
		D2DMANAGER->drawRectangle(_collisionAtk, false);
	}
	if (DEVTOOL->checkDebugMode(DEBUG_SHOW_TEXT))
	{
		D2DMANAGER->drawText(format(L"%d", _dir_pushed).c_str(), _collision.left, _collision.top - 10, false);
	}
	
	if(_act)
		_act->render();
	else
		_state->render();

	if (_effectImg)
	{
		_effectImg->aniRender(_effectPos.x, _effectPos.y, _effectAni, 1.0f, false);
		D2DMANAGER->drawText(format(L"%d, %.2f, %.2f", _effectAni->getFrameIdx().x, _effectPos.x, _effectPos.y).c_str()
							 , _collision.left, _collision.top - 20, false);
	}
}

void player::setStart(float x, float y)
{
	_x = x;
	_y = y;

	_startX = x;
	_startY = y;

	// 카메라
	RECTD2D rc = { 0.f, 0.f, WINSIZEX, WINSIZEY };
	POINTFLOAT pf = {MAPSIZEX, MAPSIZEY};
	CAMERA->init(pf, rc, PLAYER_MOVE_SPEED, &_x, &_y);

	UIMANAGER->getStatusUI()->setHpStatus(_hp);
	UIMANAGER->getStatusUI()->setCoin(_coin);

	updateCollision();
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
	// 밀리는 도중에는 추락안함
	if(!_isPushed || (eDirection_Up & _dir_pushed) != eDirection_Up)
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

	// 사운드 랜덤 출력
	int idx = RND->getInt(_vSwordSound.size());
	SOUNDMANAGER->play(_vSwordSound[idx]);
}

void player::attackDamage()
{
	if (!_actorM)
		return;

	// 적
	map<UINT, enemy*> ems = _actorM->getEnemys();

	map<UINT, enemy*>::iterator iter  = ems.begin();
	map<UINT, enemy*>::iterator end = ems.end();
	for (iter; end != iter; ++iter)
	{
		enemy* em = iter->second;

		if (em->isAlive() && CheckIntersectRect(_collisionAtk, em->getCollision()))
		{
			_actorM->hitEnemy(em->getUID(), _power);
			_isPushed = true;
			_pushedPower = static_cast<float>(PLAYER_PUSHED_POW);

			increaseSkillGauge();
		}
	}

	if (!_isPushed)
	{
		vector<terrain*> ters = *_mapData->getColliderTerrains();
		vector<terrain*>::iterator iterT = ters.begin();
		vector<terrain*>::iterator endT = ters.end();
		for (iterT; endT != iterT; ++iterT)
		{
			terrain* ter = *iterT;
			if (CheckIntersectRect(_collisionAtk, ter->getCollision()))
			{
				SOUNDMANAGER->play("sword_hit_reject");
				_isPushed = true;
				_pushedPower = static_cast<float>(PLAYER_PUSHED_POW);
			}
		}
	}


	if (_effectAni)
	{
		_effectAni->stop();
		_effectAni = nullptr;
		_effectImg = nullptr;
	}
	
	switch (_dir_atk)
	{
		case eDirection_Right:
		{
			_effectAni = ANIMANAGER->findAnimation(PLAYER_UID, ePlayer_Ani_Effect_Swing_Right);
			_dir_pushed = eDirection_Left;
			break;
		}

		case eDirection_Left:
		{
			_effectAni = ANIMANAGER->findAnimation(PLAYER_UID, ePlayer_Ani_Effect_Swing_Left);
			_dir_pushed = eDirection_Right;
			break;
		}

		case eDirection_Up:
		{
			_effectAni = ANIMANAGER->findAnimation(PLAYER_UID, ePlayer_Ani_Effect_Swing_Up);
			_dir_pushed = eDirection_Down;
			break;
		}

		case eDirection_Down:
		{
			_effectAni = ANIMANAGER->findAnimation(PLAYER_UID, ePlayer_Ani_Effect_Swing_Down);
			_dir_pushed = eDirection_Up;
			break;
		}
	}
	

	if (_effectAni)
	{
		_effectImg = _effectAni->getImage();
		_effectAni->start();
	}
}

void player::standOff()
{
	//_act = _stateMap[ePlayer_State_StandOff];
	//_act->start();

	_skillValue -= PLAYER_USE_SKILL_VALUE;
	if(_skillValue < 0)
		_skillValue = 0;
	
	if(checkDirection(eDirection_Right))
		_dir_atk = eDirection_Right;
	else
		_dir_atk = eDirection_Left;

	float value = static_cast<float>(_skillValue) / static_cast<float>(_skillMax);
	UIMANAGER->getStatusUI()->setGaugeChangeValue(value);
}

void player::standOffDamage()
{
	// fire bullet
	POINTF pos = {};
	float angle = 0.f;

	pos.y = _y - 150;
	if ((_dir_atk & eDirection_Right) == eDirection_Right)
	{
		pos.x = _x;
		angle = 0.f;
	}
	else
	{
		pos.x = _x - 270.f;
		angle = PI;
	}

	SOUNDMANAGER->play("hero_fireball");
	_actorM->firePlayerBullet(pos, angle);
}

void player::takeDamage()
{
	if(0.f < _invincibleTime)
		return;

	SOUNDMANAGER->play("enemy_damage");
	_hp -= 1;
	if (_hp <= 0)
		dead();
	
	UIMANAGER->getStatusUI()->setHpStatus(_hp);
	_invincibleTime = 1.5f;
	_tempValue = 9;
}

void player::takeDamageFromEnemy(enemy* em)
{
	if (0.f < _invincibleTime)
		return;

	SOUNDMANAGER->play("enemy_damage");
	_hp -= 1;
	if (_hp <= 0)
		dead();
	else
	{
		_isPushed = true;
		_pushedPower = static_cast<float>(PLAYER_PUSHED_POW);

		if(em->getPosX() < _x)
			_dir_pushed = eDirection_Right;
		else
			_dir_pushed = eDirection_Left;
	}

	UIMANAGER->getStatusUI()->setHpStatus(_hp);
	_invincibleTime = 1.5f;
	_tempValue = 9;
}

void player::dead()
{
	_hp = 0;
	changeState(ePlayer_State_Dead);
}

void player::regen()
{
	if (_chair)
	{
		RECTD2D col = _chair->getCollision();
		_x = col.left + (col.right - col.left) / 2.f;
		_y = col.bottom - 20.f;

		_renderAlpha = 1.0f;

		changeState(ePlayer_State_Sit);
	}
	else
	{
		_x = _startX;
		_y = _startY;

		_renderAlpha = 1.0f;

		changeState(ePlayer_State_Idle);
	}

	_hp = 5;
	UIMANAGER->getStatusUI()->setHpStatus(_hp);
}

void player::lookUp()
{
	CAMERA->setPosY(CAMERA->getPosY() - _sight);
	_sight += 10.f;

	DEVTOOL->pushBackDebugText(format(L"%f", CAMERA->getPosY()));

	if(PLAYER_LOOK_SIGHT < _sight)
		_sight = static_cast<float>(PLAYER_LOOK_SIGHT);
}

void player::lookDown()
{
	CAMERA->setPosY(CAMERA->getPosY() + _sight);
	_sight += 10.f;
	if(PLAYER_LOOK_SIGHT < _sight)
		_sight = static_cast<float>(PLAYER_LOOK_SIGHT);
}

void player::talkStart()
{
	if(_talkTarget)
	{
		_talkTarget->talkStart();
		UIMANAGER->getUI(eUI_Dialog)->uiOpen();
		UIMANAGER->getDialogUI()->setText(_talkTarget->getNextDialog());
	}
}

void player::nextTalk()
{
	if(!_talkTarget->isTalkEnd())
		UIMANAGER->getDialogUI()->setText(_talkTarget->getNextDialog());
}

void player::endTalk()
{
	_talkTarget->talkEnd();
	_talkTarget = nullptr;
	UIMANAGER->getUI(eUI_Dialog)->uiClose();
}

bool player::isTalkEnd()
{
	if (_talkTarget)
		return _talkTarget->isTalkEnd();
	else
		return true;
}

void player::enterPortal()
{
	if (_portal)
	{
		eSceneName next = _mapData->getLinkedSceneName(_portal->getUID());
		if(eSceneName_None != next)
		{
			SCENEMANAGER->setNextScene(next);
			_portal = nullptr;
		}
	}
}

void player::enterPortalTrigger()
{
	if(_portal)
	{
		if(_portal->checkAttribute(eAttr_Trigger))
			enterPortal();
	}
}

void player::sightResetUp()
{
	_function = std::move(std::bind(&player::sightUp, this));
}

void player::sightResetDown()
{
	_function = std::move(std::bind(&player::sightDown, this));
}

//===================================================================================================

bool player::checkDirection(eDirection dir)
{
	bool result = (_dir & dir) == dir;

	return result;
}

bool player::checkPossibleTalk()
{
	if (!_actorM)
		return false;

	bool check = false;

	map<UINT, npc*> npcs = _actorM->getNPCs();

	map<UINT, npc*>::iterator iter = npcs.begin();
	map<UINT, npc*>::iterator end = npcs.end();
	for(iter; end != iter; ++iter)
	{
		npc* n = iter->second;
		if (CheckIntersectRect(_collision, n->getCollision()))
		{
			_talkTarget = n;
			check = true;
			break;
		}
	}

	return check;
}

bool player::checkPortal()
{
	if (!_mapData)
		return false;

	vector<terrain*>* vCol = _mapData->getCollisionTerains();
	vector<terrain*>::iterator iter = vCol->begin();
	vector<terrain*>::iterator end = vCol->end();
	for (iter; end != iter; ++iter)
	{
		terrain* ter = (*iter);
		if (ter->checkAttribute(eAttr_Portal))
		{
			RECTD2D col = ter->getCollision();
			if (CheckIntersectRect(_collision, col))
			{
				_portal = ter;
				return true;
			}
		}
	}

	_portal = nullptr;

	return false;
}

bool player::trySit()
{
	if (!_mapData)
		return false;

	vector<terrain*>* vCol = _mapData->getCollisionTerains();
	vector<terrain*>::iterator iter = vCol->begin();
	vector<terrain*>::iterator end = vCol->end();
	for (iter; end != iter; ++iter)
	{
		terrain* ter = (*iter);
		if (ter->checkAttribute(eAttr_Chair))
		{
			RECTD2D col = ter->getCollision();
			if (CheckIntersectRect(_collision, col))
			{
				_x = col.left + (col.right - col.left) / 2.f;
				_y = col.bottom - 20.f;

				_chair = ter;

				_hp = 5;
				UIMANAGER->getStatusUI()->setHpStatus(_hp);

				return true;
			}
		}
	}

	_chair = nullptr;

	return false;
}

bool player::checkPossibleStandOff()
{
	if(!_actorM)
		return false;

	if(PLAYER_USE_SKILL_VALUE <= _skillValue && _actorM->checkPlayerBullet())
		return true;
	else
		return false;
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
	// dead
	{
		playerState* st = new deadState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Dead, st));
	}
	// look up
	{
		playerState* st = new lookUpState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Look_Up, st));
	}
	// look up stay
	{
		playerState* st = new lookUpStayState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Look_Stay, st));
	}
	// look down
	{
		playerState* st = new lookDownState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Look_Down, st));
	}
	// look down stay
	{
		playerState* st = new lookDownStayState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Look_Down_Stay, st));
	}
	// talk
	{
		playerState* st = new talkState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Talk, st));
	}
	// sit
	{
		playerState* st = new sitState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Sit, st));
	}
	// drowse
	{
		playerState* st = new drowseState;
		st->init(this);
		_stateMap.insert(make_pair(ePlayer_State_Drowse, st));
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

	// animation dead
	{
		image* img = IMAGEMANAGER->findImage("knight_dead");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Dead, "knight_dead"
										   , 0, img->GetMaxFrameX(), 5, false);
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
										   , 0, img->GetMaxFrameX(), 4, PLAYER_ANI_SPEED, false);
	}

	// animation look up
	{
		image* img = IMAGEMANAGER->findImage("knight_lookup");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Look_Up, "knight_lookup"
										   , 0, img->GetMaxFrameX() - 1, PLAYER_ANI_SPEED, false);
	}

	// animation look up stay
	{
		image* img = IMAGEMANAGER->findImage("knight_lookup");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, eplayer_Ani_Look_Up_Loop, "knight_lookup"
										   , 1, img->GetMaxFrameX() - 1, PLAYER_ANI_SPEED, true);
	}

	// animation look down
	{
		image* img = IMAGEMANAGER->findImage("knight_lookdown");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Look_Down, "knight_lookdown"
										   , 0, img->GetMaxFrameX(), 7, false);
	}

	// animation look down stay
	{
		image* img = IMAGEMANAGER->findImage("knight_lookdown");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, eplayer_Ani_Look_Down_Loop, "knight_lookdown"
										   , 0, img->GetMaxFrameX(), 7, false);
	}

	// animation dead
	{
		image* img = IMAGEMANAGER->findImage("knight_dead");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Dead, "knight_dead"
										   , 0, img->GetMaxFrameX(), PLAYER_ANI_SPEED, true);
	}

	// animation sit
	{
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Sit, "knight_sitAnddrowse"
										   , 0, 2, PLAYER_ANI_SPEED, false);
	}

	// animation drowse
	{
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Drowse, "knight_sitAnddrowse"
										   , 2, 3, 5, false);
	}

	// effect swing
	{
		image* img = IMAGEMANAGER->findImage("swing");
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Effect_Swing_Up, "swing_updown", 0, 0, PLAYER_ANI_SPEED_SLOW, false);
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Effect_Swing_Down, "swing_updown", 2, 2, PLAYER_ANI_SPEED_SLOW, false);
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Effect_Swing_Left, "swing", 4, 6, PLAYER_ANI_SPEED_FAST, false);
		ANIMANAGER->addArrayFrameAnimation(PLAYER_UID, ePlayer_Ani_Effect_Swing_Right, "swing", 0, 3, PLAYER_ANI_SPEED_FAST, false);
	}
}

void player::changeState(ePlayer_State state)
{
	if(_state->getState() == state)
		return;

	playerState* pState = findState(state);
	if (pState)
	{
		_state->end();
 		_state = _stateMap[state];
		_state->start();
		//_sight = 0.f;
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
	if(!_mapData)
		return;

	if (_isPushed)
	{
		if (eDirection_Left == _dir_pushed)			_x -= _pushedPower;
		else if(eDirection_Right == _dir_pushed)	_x += _pushedPower;
		else if(eDirection_Up == _dir_pushed)		_y -= _pushedPower;
		else if(eDirection_Down == _dir_pushed)		_y += _pushedPower;

		_pushedPower -= (TIMEMANAGER->getElapsedTime() * 5);
		updateCollision();

		if (_pushedPower <= 0.f)
			_isPushed = false;
	}


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

void player::checkCollisionEnemy()
{
	if(!_actorM)
		return;

	enemy* attaker = nullptr;

	map<UINT, enemy*> ems = _actorM->getEnemys();

	map<UINT, enemy*>::iterator iter  = ems.begin();
	map<UINT, enemy*>::iterator end = ems.end();
	for (iter; end != iter; ++iter)
	{
		enemy* em = iter->second;

		if (!CheckIntersectRect(_collision, em->getCollision()))
			continue;

		if (em->isAlive())
		{
			attaker = em;
			break;
		}
	}

	// 에너미랑 충돌 -> 피 까임
	if (attaker)
	{
		takeDamageFromEnemy(attaker);
	}
}

void player::checkCollisionCoin()
{
	if(!_objM)
		return;

	list<dropCoin*> coins = _objM->getCoinList();

	list<dropCoin*>::iterator iter = coins.begin();
	list<dropCoin*>::iterator end = coins.end();
	
	for(iter; end != iter; ++iter)
	{
		dropCoin* coin = (*iter);
		if(CheckIntersectRect(_collision, coin->getCollision()))
		{
			// todo

			SOUNDMANAGER->play("geo_small_collect_1");
			_coin += coin->getPrice();
			UIMANAGER->getStatusUI()->setCoin(_coin);
			_objM->removeGameObject(coin->getUID());
		}
	}
}

void player::sightDown()
{
	CAMERA->setPosY(CAMERA->getPosY() - _sight);
	_sight -= 10.f;
	if (_sight < 0.f)
	{
		_sight = 0.f;
		_function = NULL;
	}
}

void player::sightUp()
{
	CAMERA->setPosY(CAMERA->getPosY() + _sight);
	_sight -= 10.f;
	if (_sight < 0.f)
	{
		_sight = 0.f;
		_function = NULL;
	}
}

void player::increaseSkillGauge()
{
	++_skillValue;
	if(_skillMax < _skillValue)
		_skillValue = _skillMax;


	float value = static_cast<float>(_skillValue) / static_cast<float>(_skillMax);

	UIMANAGER->getStatusUI()->setGaugeChangeValue(value);
}
