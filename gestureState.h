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

// �ɱ�
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

// ����
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