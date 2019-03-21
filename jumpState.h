/*
	플레이어 상태 : 점프 관련
	flying, falling, jumpFalling, land
 */

#pragma once
#include "playerState.h"

class player;

// 상승
class flyingState: public playerState
{
private:
	float _jumpPower;		// 점프 파워
	float _gravity;			// 중력

public:
	flyingState();
	~flyingState();

	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};

// 일반적인 하강
class fallingState: public playerState
{
private:
	float _fallingPower;	// 떨어지는 힘
	float _gravity;			// 중력

public:
	fallingState();
	~fallingState();

	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};

// 점프 후 하강
class jumpFallingState: public playerState
{
private:
	float _jumpPower;
	float _gravity;

public:
	jumpFallingState();
	~jumpFallingState();

	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};

// 착륙
class landState: public playerState
{
public:
	landState();
	~landState();

	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};