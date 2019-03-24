#pragma once
#include "actorState.h"

class cornifer;
class corniferState : public actorState
{
protected:
	cornifer* _cornifer;

public:
	corniferState();
	~corniferState();

	virtual HRESULT init(cornifer* actor);
	virtual void release();
	virtual void update();
	virtual void render();
	
	virtual void start();
	virtual void end();
};

class corniferIdle : public corniferState
{
public:
	HRESULT init(cornifer* actor);
};

class corniferTalkLeft : public corniferState
{
public:
public:
	HRESULT init(cornifer* actor);
};

class corniferTalkRight : public corniferState
{
public:
public:
	HRESULT init(cornifer* actor);
};



