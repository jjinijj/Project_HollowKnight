#pragma once
#include "playerState.h"

class player;
class flyingState: public playerState
{
private:
	float _flyingTime;


public:
	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};

class fallingState: public playerState
{
private:

public:
	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};

class landState: public playerState
{
private:

public:
	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};