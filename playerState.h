#pragma once

class player;
class playerState
{
protected:
	player* _player;
	animation* _ani;
	UINT _nextState;

	UINT _state;
	UINT _moveDir;
	
	playerState();
public:
	~playerState();

	virtual HRESULT init(player* p);
	virtual void release();
	virtual void update();
	virtual void render();
	
	virtual void start();
	virtual void end();

	virtual UINT nextState();

	UINT getState() {return _state;}
};

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

class walkState: public playerState
{
private:
	bool _isRight;

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