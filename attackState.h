/*	
	플레이어 상태 : 공격 관련
	attack, standOff
 */

#pragma once
#include "playerState.h"

class player;
class attackState: public playerState
{
private:
	image*		_effectImg;
	animation*	_effectAni;
	UINT		_effectKey;

public:
	attackState();
	~attackState();

	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};

class standOffState: public playerState
{
private:
	bool _isRight;
	float _floatingTime;

public:
	standOffState();
	~standOffState();

	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};