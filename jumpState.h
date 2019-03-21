/*
	�÷��̾� ���� : ���� ����
	flying, falling, jumpFalling, land
 */

#pragma once
#include "playerState.h"

class player;

// ���
class flyingState: public playerState
{
private:
	float _jumpPower;		// ���� �Ŀ�
	float _gravity;			// �߷�

public:
	flyingState();
	~flyingState();

	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};

// �Ϲ����� �ϰ�
class fallingState: public playerState
{
private:
	float _fallingPower;	// �������� ��
	float _gravity;			// �߷�

public:
	fallingState();
	~fallingState();

	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};

// ���� �� �ϰ�
class jumpFallingState: public playerState
{
private:
	float _jumpPower;
	float _gravity;

public:
	jumpFallingState();
	~jumpFallingState();

	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};

// ����
class landState: public playerState
{
public:
	landState();
	~landState();

	HRESULT init(player* p);
	void release();
	void update();
	void render();

	void start();
	void end();
};