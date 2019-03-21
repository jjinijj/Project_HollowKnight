#pragma once
#include "enemy.h"

class player;
class mawlek : public enemy
{
	enum 
	{
		ATTACK_RANGE = 300, // 근거리 공격 범위
		ATTACK_RANGE_FAR = 800,// 원거리 공격 범위

		NEXT_STATE_TERM = 50,
	};


	enum eSTATE
	{
		eIDLE,
		eMOVE,
		eATTACK1,		// 근접
		eATTACK2,		// 단발 발사
		eATTACK3,		// 무더기로 발사
		eDEAD,

		eNONE,
		eSTATE_COUNT = eNONE,
	};

	enum eBODYSTATE
	{
		eBODY_IDLE,
		eBODY_MOVE,
		eBODY_ATTACK,
	};

	enum eARMSTATE
	{
		eARM_IDLE,
		eARM_MOVE,
		eARM_ATTACK,
	};

	enum eHEADSTATE
	{
		eHEAD_IDLE,
		eHEAD_ATTAK,
	};

private:
	map<int, animation*>			_animMap_armL;
	map<int, animation*>			_animMap_armR;
	map<int, animation*>			_animMap_head;

	animation* _anim_armL;
	animation* _anim_armR;
	animation* _anim_head;

	eSTATE _mawlekState;

	eARMSTATE _armState;
	eHEADSTATE _headState;

	RECT _collisionAtk;

	player* _target;

	int _time;

public:
	HRESULT init(POINTF position, unsigned int uid);
	void update();
	void release();
	void render();
	
	void dead();
	void move();

	void setPlayerLink(player* target) {_target = target;}
	void bulletFire();
	bool isFire();
	bool isOnBulletFire() { return (_mawlekState == eATTACK2); }
	POINTF getBulletFirePoint() {return MakePointF(_position.x - 35, _collision.top + 40);}

private:
	void changeArmState(eARMSTATE state);
	void changeHeadState(eHEADSTATE state);
	void changeBodyState(eBODYSTATE state);
	void changeMawlekState(eSTATE state);

	void attackSide(eDIRECTION dir);
};