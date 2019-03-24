/*
	플레이어 상태
	player state baseClass,
	기본(idle), 이동(walk)
 */
#pragma once

class player;
class playerState
{
protected:
	player* _player;			
	image* _img;			// 애니메이션에 사용할 이미지
	animation* _ani;		// 애니메이션

	UINT _nextState;		// 다음 상태

	UINT _state;			// 현재 상태
	UINT _aniKey;			// 애니메이션 키

	bool _isRight;			// 오른쪽을 향하는가
	bool _isEnd;			// 애니메이션이 끝났는가
	bool _isDoEvent;		// 애니메이션 이벤트를 행했는가
	
	float _alpha;			// 이미지 알파 렌더값

	playerState();			// 상속을 통한 접근만 가능
public:
	~playerState();			// 해제는 public

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

// 기본
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

// 걷기
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

// 데미지 입음
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