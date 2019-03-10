#pragma once
class iTween
{
protected:

	bool _isMoving;		// 움직이는 중인가
	bool _isLoop;		// 반복 할 것인가

	POINTF _start;		// 시작 위치
	POINTF _end;		// 종료 위치

	POINTF _direct;		// 이동 방향

	float* _x;			// x 좌표
	float* _y;			// y 좌표

	float _speed;		// 이동 속도

	iTween();
	~iTween();

public:

	virtual HRESULT init(POINTF start, POINTF end, float* x, float* y, float speed, bool isLoop = false);
	virtual void release();
	virtual void update();

	void start();
	void stop();
	void reverse();
	bool isMoving() { return _isMoving; }
	
};


//=======================================================
// 선형 이동
//=======================================================
class iTween_Linear : public iTween
{
private:

	
public:
	void update();
};