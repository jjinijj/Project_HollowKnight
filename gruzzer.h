#pragma once
#include "enemy.h"



enum eSTATE
{
	eMOVE,
	eDEAD,

	eSTATE_NONE,
	eSTATE_COUNT = eSTATE_NONE,
};


class gruzzer : public enemy
{
public:
	enum
	{
		GRUZZER_ANI_SPEED = 5,
		GRUZZER_MAX_HP = 1,
		GRUZZER_SPEED = 1,

		GRUZZER_WIDTH = 100,
		GRUZZER_HEIGHT = 109,
	};

private:

public:

	HRESULT init(UINT uid, float x, float y);
	void release();
	void update();
	void render();

	void move();
	void dead();
};

