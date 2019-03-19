#pragma once
#include "playerState.h"

class player;
class flyingState: public playerState
{
private:
	float _flyingTime;
	float _jumpPower;
	float _gravity;

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
	float _fallingPower;
	float _gravity;

public:
	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};

class jumpFallingState: public playerState
{
private:
	float _fallingTime;
	float _jumpPower;
	float _gravity;

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