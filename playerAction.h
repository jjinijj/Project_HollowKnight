#pragma once

class player;
class playerAction
{
protected:
	bool _isPlay;
	bool _isRight;
	bool _isDoEvent;
	player* _player;
	UINT _nextState;

public:
	playerAction();
	~playerAction();

	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};

class attackAction: public playerAction
{
private:

public:
	attackAction();
	~attackAction();

	HRESULT init(player* p);
	void release();
	void update();
	void render()
		;
	void start();
	void end();
};

class standOffAction: public playerAction
{
private:

public:
	standOffAction();
	~standOffAction();

	HRESULT init(player* p);
	void release();
	void update();
	void render()
		;
	void start();
	void end();
};