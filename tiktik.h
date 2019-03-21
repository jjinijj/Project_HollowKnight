#pragma once
#include "gameNode.h"
#include "animation.h"
#include "enemy.h"

class tiktik : public enemy
{
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
	gameObject* _activeArea;

public:

	HRESULT init(POINTF position, unsigned int uid);
	void update();
	void move();
	void dead();

private:
	void setActiveArea();

};