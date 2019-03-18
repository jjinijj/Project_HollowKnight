#pragma once


enum
{
	// �ӵ�
	PLAYER_IDLE_SPEED = 10,
	PLAYER_ATTACK_SPEED = 3,
	PLAYER_MOVE_SPEED = 3,

	// �÷��̾� �� ������ �̹��� ũ��
	PLAYER_SIZE_WIDE = 142,
	PLAYER_SIZE_HEIGHT = 133,
	PLAYER_SIZE_WIDE_HALF = PLAYER_SIZE_WIDE / 2,

	// �÷��̾� collision ũ��
	PLAYER_COL_SIZE_WIDE = 50,
	PLAYER_COL_SIZE_HEIGHT = 125,
	PLAYER_COL_SIZE_WIDE_HALF = PLAYER_COL_SIZE_WIDE / 2,
	PLAYER_COL_SIZE_HEIGHT_HALF = PLAYER_COL_SIZE_HEIGHT / 2,

	// �÷��̾� ���� �ð�
	PLAYER_INVINCIBILITY_TIME = 50,
	PLAYER_PUSHED_TIME = 10,

	// �÷��̾� ���� �Ŀ�
	PLAYER_JUMP_POWER = 8,
	PLAYER_GRAVITY = 2,

	// �и��� ���� ũ��
	PLAYER_PUSHED_POW = 2,

};

enum ePlayer_State
{
	ePlayer_State_Idle,
	ePlayer_State_Walk,
	
	ePlayer_State_Look,
	//ePlayer_State_Look_Up,
	//ePlayer_State_Look_Down,
	
	//ePlayer_State_Jump,
	ePlayer_State_Attack,

	//ePlayer_State_Attack_1,		// 5
	//ePlayer_State_Attack_2,		// 6
	//ePlayer_State_Attack_3,		// 7
	ePlayer_State_Attack_Up,	// 8
	ePlayer_State_Attack_Down,	// 9
	
	ePlayer_State_Sit,
	//ePlayer_State_Drowse,
	//ePlayer_State_WakeUp,
	ePlayer_State_Dead,
	
	ePlayer_State_Flying,
	ePlayer_State_Falling,
	ePlayer_State_Land,
	
	//ePlayer_State_Hit,

	ePlayer_State_None,
	ePlayer_State_Count = ePlayer_State_None,
};

enum eDirection
{
	eDirection_None,
	
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

	float _x;
	float _y;

	bool _isFloating;
	bool _isJumping;

	UINT _coin;

	mapData* _mapData;
	image* _img;
	animation* _ani;

	playerState* _state;
	UINT _nextState;

	playerState* _actState;

	RECTD2D _collision;
	RECTD2D _attkCol;

	WORD _dir;

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
	void setJumping(bool flag)
	{
		_isJumping = flag;
		_isFloating = true;
	}
	bool isJumping() {return _isJumping;}
	
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
	void setDirectionLeft()	 { _dir ^= eDirection_Right; }
	void setDirectionUp() 
	{
		_dir |= eDirection_Up; 
		_dir ^= eDirection_Down; 
	}
	void setDirectionDown()
	{
		_dir ^= eDirection_Up; 
		_dir |= eDirection_Down;
	}

	void setDirectionIdle()
	{
		_dir ^= eDirection_Up;
		_dir ^= eDirection_Down;
	}
	WORD getDirection() {return _dir;}


	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();

private:
	void initState();
	void changeState(UINT state);
	void updateCollision();

	void fixPosition();
};

