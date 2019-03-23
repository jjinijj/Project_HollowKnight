#pragma once
#include "actorState.h"

class primalAspid;
class primalState : public actorState
{
protected:
	primalAspid* _primal;

	primalState();
public:
	~primalState();

	virtual HRESULT init(primalAspid* actor);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void start();
	virtual void end();
};

class primalIdle : public primalState
{
private:
	

public:
	primalIdle() {}
	~primalIdle() {}

	HRESULT init(primalAspid* actor);
	void update();
	void end();
};

class primalApproach : public primalState
{
private:

public:
	primalApproach() {}
	~primalApproach() {}

	HRESULT init(primalAspid* actor);
	void update();
	
	void end();
};

class primalAttack : public primalState
{
private:

public:
	primalAttack() {}
	~primalAttack() {}

	HRESULT init(primalAspid* actor);
	void update();

	void end();
};

class primalFarway : public primalState
{
private:

public:
	primalFarway() {}
	~primalFarway(){}

	HRESULT init(primalAspid* actor);
	void update();

	void end();
};

class primalDead : public primalState
{
private:

public:
	primalDead();
	~primalDead();

	HRESULT init(primalAspid* actor);
	void release();
	void update();

	void start();
	void end();
};