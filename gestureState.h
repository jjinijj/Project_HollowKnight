#pragma once

#include "playerState.h"

class player;
// �� ����
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

// �� ����
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

// �Ʒ� ����
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

// �Ʒ� ����
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

// ��ȭ��
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