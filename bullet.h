#pragma once
#include "gameNode.h"

enum eBULLET_TYPE
{
	eLINEARBULLET,
	eARCBULLET,

	eBULLET_NONE,
	eBULLET_TYPE_COUNT = eBULLET_NONE,
};

class bullet : public gameNode
{
protected:
	enum eSTATE
	{
		eMOVE,
		ePANG,

		eNONE,
		eCOUNT = eNONE,
	};

	animation* _anim_move;
	animation* _anim_pang;

	animation* _anim;

	POINTF _position;
	float _angle;
	float _speed;
	float _radius;

	bool _islongBullet;

	POINTF _colPos;

	eSTATE _state;
	eBULLET_TYPE _type;
	bool _isAppear;

public:

	HRESULT init();
	virtual HRESULT init(POINTF pos, float angle, float speed, float radius, const char* moveimgName, const char* pangimgName);
	void release();
	void update();
	void render();

	virtual void move();
	virtual void clear();
	void hitSomething();

	void setColPos(POINTF pos) {_colPos = pos;}
	POINTF getColPos() {return _colPos;}

	POINTF getPosition()		{ return _position;}
	float getRadius()			{ return _radius;}
	bool isAppear()				{ return _isAppear;}
	bool isPang()				{ return ePANG == _state;}
	eBULLET_TYPE getBulletType(){ return _type;}
};

class linearBullet : public bullet
{
public:
	HRESULT init(POINTF pos, float angle, float speed, float radius, const char* moveimgName, const char* pangimgName);
	void update();
	void move();
};

class arcBullet : public bullet
{
private:
	POINTF _startPos;
	float _time;
	float _gravity;

public:
	HRESULT init(POINTF pos, float angle, float speed, float radius, const char* moveimgName, const char* pangimgName);
	void setGravity(float gravity) {_gravity = gravity;}
	void update();
	void move();
	void clear();
};