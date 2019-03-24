#pragma once
#include "actorState.h"

class bullet;
class bulletState : public actorState
{
protected:
	bullet* _bullet;

public:
	bulletState();
	~bulletState();

	virtual HRESULT init(bullet* actor);
	virtual void release();
	virtual void update();
	virtual void render();
	
	virtual void start();
	virtual void end();
};

class bulletMove : public bulletState
{
public:
	bulletMove() {}
	~bulletMove(){}

	HRESULT init(bullet* actor);
	void update();
};


class bulletPang : public bulletState
{
public:
	bulletPang() {}
	~bulletPang(){}

	HRESULT init(bullet* actor);
	void update();
	void end();
};