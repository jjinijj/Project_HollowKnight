#pragma once
#include "singletonBase.h"
#include <functional>

class playerState;
class gameObject;
class mapData;
class npc;
class terrain;
class enemy;
class objectManager;
class player : public singletonBase<player>
{
public:
	// default value
	enum
	{
		// 속도
		PLAYER_ANI_SPEED = 10,
		PLAYER_ANI_SPEED_SLOW = 5,
		PLAYER_ANI_SPEED_FAST = 13,
		PLAYER_ATTACK_SPEED = 3,
		PLAYER_MOVE_SPEED = 250,

		// 시선
		PLAYER_LOOK_SIGHT = 300,

		// 플레이어 한 프레임 이미지 크기
		PLAYER_SIZE_WIDE = 142,
		PLAYER_SIZE_HEIGHT = 133,
		PLAYER_SIZE_WIDE_HALF = PLAYER_SIZE_WIDE / 2,

		// 플레이어 collision 크기
		PLAYER_COL_SIZE_WIDE = 50,
		PLAYER_COL_SIZE_HEIGHT = 125,
		PLAYER_COL_SIZE_WIDE_HALF = PLAYER_COL_SIZE_WIDE / 2,
		PLAYER_COL_SIZE_HEIGHT_HALF = PLAYER_COL_SIZE_HEIGHT / 2,

		// 플레이어 무적 시간
		PLAYER_INVINCIBILITY_TIME = 50,
		PLAYER_PUSHED_TIME = 10,

		// 플레이어 점프 파워
		PLAYER_JUMP_POWER = 10,
		PLAYER_FALLING_POWER = 5,
		PLAYER_GRAVITY = 13,

		// 밀리는 힘의 크기
		PLAYER_PUSHED_POW = 2,
		PLAYER_PUSHED_FRAME_COUNT = 10,

		PLAYER_INIT_HP = 5,
		PLAYER_SKILL_VALUE = 10,

		PLAYER_USE_SKILL_VALUE = 3,
		PLAYER_FILL_HP_TIME = 3,

	};

	// player state
	enum ePlayer_State
	{
		ePlayer_State_Idle,			// 기본
		ePlayer_State_Walk,			// 이동

		ePlayer_State_Sit,			// 앉기
		ePlayer_State_Drowse,		// 졸기
		ePlayer_State_WakeUp,		// 일어나기

		ePlayer_State_Dead,			// 죽음

		ePlayer_State_Flying,		// 상승
		ePlayer_State_Falling,		// 하강
		ePlayer_State_JumpFalling,	// 점프 후 하강
		ePlayer_State_Land,			// 착륙

		ePlayer_State_Attack,		// 근거리 공격
		ePlayer_State_StandOff,		// 원거리 공격

		ePlayer_State_Look_Up,		// 위로 보기
		ePlayer_State_Look_Down,	// 아래로 보기
		ePlayer_State_Look_Stay,	// 계속 보기
		ePlayer_State_Look_Down_Stay,// 계속 보기
		
		ePlayer_State_Talk,			// 대화

		ePlayer_State_None,			// 상태없음
		ePlayer_State_Count = ePlayer_State_None,
	};

	enum ePlayer_Animation
	{
		ePlayer_Ani_Idle,		// 기본
		ePlayer_Ani_Walk,		// 이동

		ePlayer_Ani_Sit,		// 앉기
		ePlayer_Ani_Drowse,		// 졸기

		ePlayer_Ani_Dead,		// 죽음

		ePlayer_Ani_Flying,		// 상승
		ePlayer_Ani_Falling,	// 하강
		ePlayer_Ani_Land,		// 착륙

		ePlayer_Ani_Attack_1,	// 좌우 공격 1
		ePlayer_Ani_Attack_2,	// 좌우 공격 2
		ePlayer_Ani_Attack_Up,	// 위로 공격
		ePlayer_Ani_Attack_Down,// 아래로 공격
		ePlayer_Ani_StandOff,	// 원거리 공격

		ePlayer_Ani_Look_Up,	// 위로 보기
		ePlayer_Ani_Look_Down,	// 아래로 보기
		eplayer_Ani_Look_Up_Loop,// 위로 보기 반복
		eplayer_Ani_Look_Down_Loop,// 아래로 보기 반복

		ePlayer_Ani_Effect_Swing_Right,
		ePlayer_Ani_Effect_Swing_Left,
		ePlayer_Ani_Effect_Swing_Up,
		ePlayer_Ani_Effect_Swing_Down,

		ePlayer_Ani_None,
		ePlayer_Ani_Count = ePlayer_Ani_None,
	};

	enum eDirection
	{
		eDirection_Left		= 0,
		eDirection_Right	= 1,
		eDirection_Up		= 2,
		eDirection_Down		= 4,
	};



private:

	UINT _coin;					// 돈
	
	bool _isFloating;			// 공중에 떠있는가
	bool _isPushed;
	bool _isHit;

	UINT _power;
	int	 _hp;

	UINT _skillMax;
	int _skillValue;

	float _x;					// 위치 : center
	float _y;					// 위치 : bottom

	float _sight;				// 위/아래 카매라 이동
	float _invincibleTime;		// 무적
	float _renderAlpha;			// 이미지 알파값
	float _pushedPower;			// 밀림
	int _tempValue;


	playerState* _state;		// 현재 상태
	playerState* _act;			// 추가 상태 : 현재 상태와 함께 사용되며 애니메이션 우선 출력
	ePlayer_State _nextState;	// 다음 상태
	
	image*		_effectImg;
	animation*	_effectAni;
	POINTF		_effectPos;


	RECTD2D _collision;			// 충돌체
	RECTD2D _collisionAtk;		// 공격 충돌체

	POINTF _atkRange;			// 공격 범위

	WORD _dir;					// 키가 눌린 방향
	WORD _dir_atk;				// 공격 방향
	WORD _dir_pushed;			// 밀리는 방향

	mapData*		_mapData;	// 맵 정보
	actorManager*	_actorM;
	objectManager*	_objM;
	
	npc*	 _talkTarget;
	terrain* _chair;
	terrain* _portal;
	
	// 리젠포인트
	float _startX;
	float _startY;

	function<void(void)> _function;
	map<UINT, playerState*> _stateMap;	// 상태 맵

	vector<string> _vSwordSound;

public:
	player();
	~player();

	HRESULT init();
	HRESULT init(float x, float y);
	void release();
	void update();
	void render();

	void setStart(float x, float y);
	void mapDataLink(mapData* data)				{ _mapData = data;  }
	void actorManagerLink(actorManager* actorM) { _actorM = actorM; }
	void objectManagerLink(objectManager* objM) { _objM = objM;		}

	// 이동
	void moveRight();
	void moveLeft();
	void moveJump(float jumpPower);
	void moveFall(float gravity);
	void moveUp();
	void moveDown();

	// 근접 공격
	void attack();
	// 근접 공격 데미지 적용
	void attackDamage();
	// 원거리 공격
	void standOff();
	// 원거리 공격 데미지 적용
	void standOffDamage();
	// 데미지 입음
	void takeDamage();
	// 데미지 입음
	void takeDamageFromEnemy(enemy* em);
	// 죽음
	void dead();
	// 리젠
	void regen();
	
	// 보기
	void lookUp();
	void lookDown();

	// 대화
	void talkStart();
	void nextTalk();
	void endTalk();
	bool isTalkEnd();

	// 씬 이동
	void enterPortal();
	void enterPortalTrigger();

	void sightResetUp();
	void sightResetDown();

	// 상태 전환
	void changeState(ePlayer_State state);

	//=====================================================
	// check
	//=====================================================

	// 공중에 떠 있는가
	bool isStateFloating() { return  _isFloating; }
	// 방향 확인
	bool checkDirection(eDirection dir);
	// 대화 가능한지 확인
	bool checkPossibleTalk();
	// 씬 이동 구역인지 확인
	bool checkPortal();
	// 앉을 수 있는지 확인
	bool trySit();
	// 원거리 공격 가능한지 확인
	bool checkPossibleStandOff();
	// 무적상태인가
	bool isInvicible() { return (0.f <_invincibleTime); }

	//=====================================================
	// set
	//=====================================================
	
	// 위치 x : mid
	void setPositionX(float x) { _x = x; updateCollision();}
	void setPositionY(float y) { _y = y; updateCollision();}
	// 위치 y : bottom
	void setAttkDirection(eDirection dir) {_dir_atk = dir;}

	// 방향
	void setDirectionRight();
	void setDirectionLeft();
	void setDirectionUp();
	void setDirectionDown();
	// 방향 초기화 : R or L
	void setDirectionIdle();


	//=====================================================
	// get
	//=====================================================

	// 위치 x : mid
	float getPosX()			{ return _x; }
	// 위치 y : bottom   
	float getPosY()			{ return _y; }
	// 방향			   
	WORD getDirection()		{ return _dir; }
	// 힘
	UINT getPower()			{ return _power; }
	// 충돌체
	RECTD2D getCollision()	{ return _collision; }
	
	float getAlpha()		{ return _renderAlpha; }

private:
	// 상태 초기화
	void initState();
	// 애니메이션 초기화
	void initAnimaion();

	// 충돌체 갱신
	void updateCollision();
	// 위치 보정 및 공중에 떠있는지 확인
	void fixPosition();
	// 상태 찾기
	playerState* findState(ePlayer_State state);
	// 충돌 체크 enemy
	void checkCollisionEnemy();
	// 충돌체크 coin
	void checkCollisionCoin();

	// 시야 리셋
	void sightDown();
	void sightUp();

	void increaseSkillGauge();
};

