/*
	�÷��̾� ����
	player state baseClass,
	�⺻(idle), �̵�(walk)
 */
#pragma once

class player;
class playerState
{
protected:
	player* _player;			
	image* _img;			// �ִϸ��̼ǿ� ����� �̹���
	animation* _ani;		// �ִϸ��̼�

	UINT _nextState;		// ���� ����

	UINT _state;			// ���� ����
	UINT _aniKey;			// �ִϸ��̼� Ű

	bool _isRight;			// �������� ���ϴ°�
	bool _isEnd;			// �ִϸ��̼��� �����°�
	bool _isDoEvent;		// �ִϸ��̼� �̺�Ʈ�� ���ߴ°�
	
	float _alpha;			// �̹��� ���� ������

	playerState();			// ����� ���� ���ٸ� ����
public:
	~playerState();			// ������ public

	virtual HRESULT init(player* p);
	virtual void release();
	virtual void update();
	virtual void render();
	
	virtual void start();
	virtual void end();

	virtual UINT nextState();

	UINT getState() { return _state; }
	bool isEnd()	{ return _isEnd; }

protected:
	void setAnimation(UINT aniKey);
	void moveLeft();
	void moveRight();
	void setUpAndDownDirection();
};

// �⺻
class idleState: public playerState
{
private:
	
public:
	idleState();
	~idleState();

	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();

	UINT nextState();
};

// �ȱ�
class walkState: public playerState
{
private:

public:
	walkState();
	~walkState();

	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();

	UINT nextState();

private:
	void rightMove();
	void leftMove();
};

// ������ ����
class takeDamage : public playerState
{
private:
	float _time;

public:
	HRESULT init(player* p);
	void update();
	void end();
};

class deadState : public playerState
{
public:
	HRESULT init(player* p);
	void update();
	void start();
	void end();
};