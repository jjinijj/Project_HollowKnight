#pragma once

class player;
class playerState
{
protected:
	player* _player;
	image* _img;
	animation* _ani;

	UINT _nextState;

	UINT _state;
	UINT _aniKey;

	bool _isRight;
	bool _isEnd;
	bool _isDoEvent;
	
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

	UINT getState() { return _state; }
	bool isEnd()	{ return _isEnd; }

protected:
	void setAnimation(UINT aniKey);
	void moveLeft();
	void moveRight();
	void setUpAndDownDirection();
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