#pragma once
#include "gameObject.h"
class dropObject : public gameObject
{
protected:
	eDropObject_Type _type;

	image*		_img;
	animation*	_ani;

	bool  _isMoveable;

	int	_value;

	float _angle;
	float _gravity;
	float _time;
	float _speed;

	POINTF _startPos;

	float _vecX;
	float _vecY;

public:
	dropObject();
	~dropObject();

	HRESULT init();
	HRESULT init(const UID uid, const float destX, const  float destY);
	void release();
	void update();
	void render();

	void updateCollision();

	virtual void move();

	void pushObject(float offsetX, float offsetY);
};

class dropCoin : public dropObject
{
private:

public:
	dropCoin();
	~dropCoin();

	HRESULT init();
	HRESULT init(const UID uid);
	void release();
	void update();
	void render();

	void start(float x, float y);
	void move();
	void clear();

	int getPrice() {return _value;}
	void intersectWithObject(gameObject* obj);
};

class dropItem : public dropObject
{
private:

public:
	dropItem();
	~dropItem();

	HRESULT init();
	HRESULT init(const UID uid, const float destX, const  float destY);
	void release();
	void update();
	void render();
};

