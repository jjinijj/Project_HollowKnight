#pragma once


enum
{
	// 속도
	PLAYER_ANI_SPEED = 10,
	PLAYER_ANI_SPEED_SLOW = 5,
	PLAYER_ATTACK_SPEED = 3,
	PLAYER_MOVE_SPEED = 250,

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

};

enum ePlayer_State
{
	ePlayer_State_Idle,
	ePlayer_State_Walk,
	
	ePlayer_State_Sit,
	ePlayer_State_Drowse,
	ePlayer_State_WakeUp,
	
	ePlayer_State_Dead,
	
	ePlayer_State_Flying,
	ePlayer_State_FlyingStay,
	ePlayer_State_Falling,
	ePlayer_State_JumpFalling,
	ePlayer_State_Land,
	
	ePlayer_State_Attack,
	ePlayer_State_StandOff,
	
	ePlayer_State_Look,

	ePlayer_State_None,
	ePlayer_State_Count = ePlayer_State_None,
};

enum ePlayer_Animation
{
	ePlayer_Ani_Idle,
	ePlayer_Ani_Walk,
	
	ePlayer_Ani_Sit,
	ePlayer_Ani_Drowse,

	ePlayer_Ani_Dead,

	ePlayer_Ani_Flying,
	ePlayer_Ani_FlyingStay,
	ePlayer_Ani_Falling,
	ePlayer_Ani_Land,

	ePlayer_Ani_Attack_1,
	ePlayer_Ani_Attack_2,
	ePlayer_Ani_Attack_Up,
	ePlayer_Ani_Attack_Down,
	ePlayer_Ani_StandOff,
	
	ePlayer_Ani_Look_Up,
	ePlayer_Ani_Look_Down,

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

class playerState;
class gameObject;
class mapData;
class player
{
private:

	UINT _coin;
	bool _isFloating;
	bool _isJumping;

	float _x;
	float _y;
	float _gravity;
	float _jumpPower;

	mapData* _mapData;

	playerState* _state;
	playerState* _act;
	ePlayer_State _nextState;

	RECTD2D _collision;
	RECTD2D _collisionAtk;

	POINTF _atkRange;

	WORD _dir;
	WORD _dir_ud;
	WORD _dir_atk;

	map<UINT, playerState*> _stateMap;

public:
	player();
	~player();

	HRESULT init(float x, float y);
	void release();
	void update();
	void render();

	void mapDataLink(mapData* data) { _mapData = data; }

	//void resetPlayer();
	//void move();
	
	//void changeState(ePlayer_State state);
	//
	//void evaluateEvent();
	//
	//void takeDamage();
	//void changeCoin(int value);
	//
	//void attackUseSword();
	//void attackUseBullet();
	//
	////void setManagerLink(objectManager* objM, bulletManager* bulletM, enemyManager* enemyM, uiManager* uiM) { _objM = objM; _bulletM = bulletM; _enemyM = enemyM; _uiM = uiM; }
	//
	//bool checkInteractionObject(int type);
	//gameObject* findInteractionObject(int type);
	//bool checkIntersectEnemy();
	//bool checkFloating();
	//
	//bool isStateCheck_Attack();	
	//bool isMoveable();
	//

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

	//=====================================================
	// check
	//=====================================================

	// 공중에 떠 있는가
	bool isStateFloating() { return  _isFloating; }
	// 방향 확인
	bool checkDirection(eDirection dir);

	//=====================================================
	// set
	//=====================================================
	
	// 위치 x : mid
	void setPositionX(float x) { _x = x; }
	void setPositionY(float y) { _y = y; }
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
	float getPositionX() { return _x; }
	// 위치 y : bottom   
	float getPositionY() { return _y; }
	// 방향			   
	WORD getDirection()	 { return _dir;}


private:
	// 상태 초기화
	void initState();
	// 애니메이션 초기화
	void initAnimaion();
	// 상태 전환
	void changeState(ePlayer_State state);
	// 충돌체 갱신
	void updateCollision();
	// 위치 보정 및 공중에 떠있는지 확인
	void fixPosition();
	// 상태 찾기
	playerState* findState(ePlayer_State state);
};

