#pragma once
#include "actorState.h"

class tiktik;
class tiktikMove : public actorState
{
protected:
	tiktik* _tiktik;

public:
	tiktikMove();
	~tiktikMove();

	virtual HRESULT init(tiktik* actor);
	void release();
	void update();
	void render();

	void start();
	void end();

};

class tiktikMoveOn : public tiktikMove
{
private:

public:
	tiktikMoveOn() {}
	~tiktikMoveOn() {}
	HRESULT init(tiktik* actor);
	void update();
};

class tiktikMoveUnder : public tiktikMove
{
private:

public:
	tiktikMoveUnder() {}
	~tiktikMoveUnder() {}
	HRESULT init(tiktik* actor);
	void update();
};

class tiktikMoveSideUp : public tiktikMove
{
private:

public:
	tiktikMoveSideUp() {}
	~tiktikMoveSideUp() {}
	HRESULT init(tiktik* actor);
	void update();
};

class tiktikMoveSiedDown : public tiktikMove
{
private:

public:
	tiktikMoveSiedDown() {}
	~tiktikMoveSiedDown() {}
	HRESULT init(tiktik* actor);
	void update();
};

class tiktikClimbSideToOn : public tiktikMove
{
private:

public:
	tiktikClimbSideToOn() {}
	~tiktikClimbSideToOn() {}
	HRESULT init(tiktik* actor);
	void update();
	void end();
};

class tiktikClimbSideToDown : public tiktikMove
{
private:

public:
	tiktikClimbSideToDown() {}
	~tiktikClimbSideToDown() {}
	HRESULT init(tiktik* actor);
	void update();
	void end();
};

class tiktikClimbOnToSide : public tiktikMove
{
private:

public:
	tiktikClimbOnToSide() {}
	~tiktikClimbOnToSide() {}
	HRESULT init(tiktik* actor);
	void update();
	void end();
};

class tiktikClimbUnderToSide : public tiktikMove
{
private:

public:
	tiktikClimbUnderToSide() {}
	~tiktikClimbUnderToSide() {}
	HRESULT init(tiktik* actor);
	void update();
	void end();
};