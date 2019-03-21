#pragma once
#include "gameNode.h"
#include "animation.h"
#include "enemy.h"

class terrain;
class tiktik : public enemy
{
	enum
	{
		TIKTIK_ANI_SPEED = 10,
		
		TIKTIK_MOVE_SPEED = 1,
		TIKTIK_MAX_HP = 2,

		TIKTIK_WIDTH = 50,
		TIKTIK_HIEGHT = 50,
	};

private:
	enum eSTATE
	{
		eMOVE_ON,
		eMOVE_UNDER,
		eMOVE_SIDE_UP,
		eMOVE_SIDE_DOWN,
		eCLIMB_SIDE_TO_ON,
		eCLIMB_SIDE_TO_UNDER,
		eCLIME_ON_TO_SIDE,
		eCLIME_UNDER_TO_SIDE,
		eDEAD,

		eSTATE_NONE,
		eSTATE_COUNT = eSTATE_NONE,
	};


private:
	terrain* _area;

public:

	HRESULT init(UINT uid, float x, float y);
	void update();
	void release();
	void render();

	void move();

	// 활동구역
	void terrainLink(terrain* ter) { _area = ter; }

private:
	void setActiveArea();

};