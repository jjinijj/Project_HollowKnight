#pragma once
#include "gameNode.h"
#include "animation.h"
#include "enemy.h"

class terrain;
class tiktik : public enemy
{
public:
	enum
	{
		TIKTIK_ANI_SPEED = 5,
		TIKTIK_ANI__CORNER_SPEED = 1,
		
		TIKTIK_MOVE_SPEED = 1,
		TIKTIK_MAX_HP = 1,

		TIKTIK_WIDTH = 90,
		TIKTIK_HIEGHT = 90,
	};

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
	UINT _areaUid;

	terrain* _area;
	RECTD2D	 _areaCol;

public:

	HRESULT init(UINT uid, float x, float y);
	void update();
	void release();
	void render();
	void dead();

	ACTORPACK* makePack();
	void loadPack(ACTORPACK* pack);

	void mapDataLink(mapData* data);

	void moveOn();
	void moveUnder();
	void moveSideUp();
	void moveSiedDown();
	void climbSideToOn();
	void climbSideToDown();
	void climbOnToSide();
	void climbUnderToSide();

	// 활동구역
	void terrainLink(terrain* ter)	{ _area = ter; }
	void terrainLink(UINT terUid)	{ _areaUid = terUid;}

	UINT getActiveAreaUid()	{ return _areaUid; }

private:
	void changeState(eSTATE state);
	void fixPosition();

};