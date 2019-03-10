#pragma once
class iTween
{
protected:

	bool _isMoving;		// �����̴� ���ΰ�
	bool _isLoop;		// �ݺ� �� ���ΰ�

	POINTF _start;		// ���� ��ġ
	POINTF _end;		// ���� ��ġ

	POINTF _direct;		// �̵� ����

	float* _x;			// x ��ǥ
	float* _y;			// y ��ǥ

	float _speed;		// �̵� �ӵ�

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
// ���� �̵�
//=======================================================
class iTween_Linear : public iTween
{
private:

	
public:
	void update();
};