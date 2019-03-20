#pragma once


enum
{
	// 속도
	PLAYER_ANI_SPEED = 10,
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
	image* _img;
	animation* _ani;

	playerState* _state;
	playerState* _act;
	ePlayer_State _nextState;

	RECTD2D _collision;
	RECTD2D _attkCol;

	WORD _dir;
	WORD _dir_ud;

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

	bool isStateFloating() { return  _isFloating; }
	//
	void setPositionX(float x) { _x = x; }
	void setPositionY(float y) { _y = y; }
	//
	float getPositionX() { return _x; }
	float getPositionY() { return _y; }
	//RECTD2D getCollisionRECT() { return _collision; }
	//UINT getCoin() { return _coin; }
	void setDirectionRight() { _dir |= eDirection_Right; }
	void setDirectionLeft()	
	{
		if(checkDirection(eDirection_Right))
			_dir ^= eDirection_Right; 
	}
	void setDirectionUp() 
	{
		if (checkDirection(eDirection_Down))
			_dir ^= eDirection_Down; 
		
		_dir |= eDirection_Up; 
	}
	void setDirectionDown()
	{
		if (checkDirection(eDirection_Up))
			_dir ^= eDirection_Up;

		_dir |= eDirection_Down;
	}

	void setDirectionIdle()
	{
		if (checkDirection(eDirection_Right))
			_dir = eDirection_Right;
		else
			_dir = eDirection_Left;
	}
	WORD getDirection() {return _dir;}

	bool checkDirection(eDirection dir);

	void setAnimation(animation* ani) {_ani = ani;}
	void moveRight();
	void moveLeft();
	void moveJump(float jumpPower);
	void moveFall(float gravity);
	void moveUp();
	void moveDown();

	void attack();
	void attackDamage();
	void standOff();
	void standOffDamage();


private:
	void initState();
	void initAnimaion();
	void changeState(ePlayer_State state);
	void updateCollision();

	void fixPosition();

	playerState* findState(ePlayer_State state);
};

