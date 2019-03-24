#pragma once

#include "playerState.h"

class player;
// 위 보기
class lookUpState : public playerState
{
public:
	lookUpState() {}
	~lookUpState(){}

	HRESULT init(player* p);
	void update();

	void start();
	void end();
};

// 위 보기
class lookUpStayState : public playerState
{
public:
	lookUpStayState() {}
	~lookUpStayState(){}

	HRESULT init(player* p);
	void update();

	void start();
	void end();
};

// 아래 보기
class lookDownState : public playerState
{
public:
	lookDownState() {}
	~lookDownState(){}

	HRESULT init(player* p);
	void update();

	void start();
	void end();
};

// 아래 보기
class lookDownStayState : public playerState
{
public:
	lookDownStayState() {}
	~lookDownStayState(){}

	HRESULT init(player* p);
	void update();

	void start();
	void end();
};

// 대화중
class talkState : public playerState
{
public:
	talkState() {}
	~talkState(){}

	HRESULT init(player* p);
	void update();

	void start();
	void end();
};

// 앉기
class sitState : public playerState
{
private:
	float _sitTime;

public:
	sitState() {}
	~sitState(){}

	HRESULT init(player* p);
	void update();
	void render();

	void start();
	void end();
};

// 졸기
class drowseState : public playerState
{
public:
	drowseState() {}
	~drowseState(){}

	HRESULT init(player* p);
	void update();

	void start();
	void end();
};