#pragma once
#include "singletonBase.h"

class playerState;
class gameObject;
class mapData;
class player : public singletonBase<player>
{
public:
	// default value
	enum
	{
		// �ӵ�
		PLAYER_ANI_SPEED = 10,
		PLAYER_ANI_SPEED_SLOW = 5,
		PLAYER_ANI_SPEED_FAST = 13,
		PLAYER_ATTACK_SPEED = 3,
		PLAYER_MOVE_SPEED = 250,

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
		PLAYER_JUMP_POWER = 10,
		PLAYER_FALLING_POWER = 5,
		PLAYER_GRAVITY = 13,

		// �и��� ���� ũ��
		PLAYER_PUSHED_POW = 2,

	};

	// player state
	enum ePlayer_State
	{
		ePlayer_State_Idle,			// �⺻
		ePlayer_State_Walk,			// �̵�

		ePlayer_State_Sit,			// �ɱ�
		ePlayer_State_Drowse,		// ����
		ePlayer_State_WakeUp,		// �Ͼ��

		ePlayer_State_Dead,			// ����

		ePlayer_State_Flying,		// ���
		ePlayer_State_Falling,		// �ϰ�
		ePlayer_State_JumpFalling,	// ���� �� �ϰ�
		ePlayer_State_Land,			// ����

		ePlayer_State_Attack,		// �ٰŸ� ����
		ePlayer_State_StandOff,		// ���Ÿ� ����

		ePlayer_State_Look,			// (����or�Ʒ���)����

		ePlayer_State_None,			// ���¾���
		ePlayer_State_Count = ePlayer_State_None,
	};

	enum ePlayer_Animation
	{
		ePlayer_Ani_Idle,		// �⺻
		ePlayer_Ani_Walk,		// �̵�

		ePlayer_Ani_Sit,		// �ɱ�
		ePlayer_Ani_Drowse,		// ����

		ePlayer_Ani_Dead,		// ����

		ePlayer_Ani_Flying,		// ���
		ePlayer_Ani_Falling,	// �ϰ�
		ePlayer_Ani_Land,		// ����

		ePlayer_Ani_Attack_1,	// �¿� ���� 1
		ePlayer_Ani_Attack_2,	// �¿� ���� 2
		ePlayer_Ani_Attack_Up,	// ���� ����
		ePlayer_Ani_Attack_Down,// �Ʒ��� ����
		ePlayer_Ani_StandOff,	// ���Ÿ� ����

		ePlayer_Ani_Look_Up,	// ���� ����
		ePlayer_Ani_Look_Down,	// �Ʒ��� ����

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

	UINT _coin;					// ��
	
	bool _isFloating;			// ���߿� ���ִ°�

	UINT _power;
	int	 _hp;

	float _x;					// ��ġ : center
	float _y;					// ��ġ : bottom

	mapData* _mapData;			// �� ����

	playerState* _state;		// ���� ����
	playerState* _act;			// �߰� ���� : ���� ���¿� �Բ� ���Ǹ� �ִϸ��̼� �켱 ���
	ePlayer_State _nextState;	// ���� ����

	RECTD2D _collision;			// �浹ü
	RECTD2D _collisionAtk;		// ���� �浹ü

	POINTF _atkRange;			// ���� ����

	WORD _dir;					// Ű�� ���� ����
	WORD _dir_atk;				// ���� ����

	actorManager* _actorM;

	map<UINT, playerState*> _stateMap;	// ���� ��

public:
	player();
	~player();

	HRESULT init(float x, float y);
	void release();
	void update();
	void render();

	void mapDataLink(mapData* data) { _mapData = data; }
	void actorManagerLink(actorManager* actorM) {_actorM = actorM;}

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

	// �̵�
	void moveRight();
	void moveLeft();
	void moveJump(float jumpPower);
	void moveFall(float gravity);
	void moveUp();
	void moveDown();

	// ���� ����
	void attack();
	// ���� ���� ������ ����
	void attackDamage();
	// ���Ÿ� ����
	void standOff();
	// ���Ÿ� ���� ������ ����
	void standOffDamage();
	// ������ ����
	void takeDamage();
	// ����
	void dead();
	// ����
	void regen();

	//=====================================================
	// check
	//=====================================================

	// ���߿� �� �ִ°�
	bool isStateFloating() { return  _isFloating; }
	// ���� Ȯ��
	bool checkDirection(eDirection dir);

	//=====================================================
	// set
	//=====================================================
	
	// ��ġ x : mid
	void setPositionX(float x) { _x = x; }
	void setPositionY(float y) { _y = y; }
	// ��ġ y : bottom
	void setAttkDirection(eDirection dir) {_dir_atk = dir;}

	// ����
	void setDirectionRight();
	void setDirectionLeft();
	void setDirectionUp();
	void setDirectionDown();
	// ���� �ʱ�ȭ : R or L
	void setDirectionIdle();


	//=====================================================
	// get
	//=====================================================

	// ��ġ x : mid
	float getPosX() { return _x; }
	// ��ġ y : bottom   
	float getPosY() { return _y; }
	// ����			   
	WORD getDirection()	 { return _dir; }
	// ��
	UINT getPower()		{ return _power; }
	// �浹ü
	RECTD2D getCollision() { return _collision; }

private:
	// ���� �ʱ�ȭ
	void initState();
	// �ִϸ��̼� �ʱ�ȭ
	void initAnimaion();
	// ���� ��ȯ
	void changeState(ePlayer_State state);
	// �浹ü ����
	void updateCollision();
	// ��ġ ���� �� ���߿� ���ִ��� Ȯ��
	void fixPosition();
	// ���� ã��
	playerState* findState(ePlayer_State state);
};

