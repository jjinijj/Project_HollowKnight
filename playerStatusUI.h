#pragma once
#include "uiBase.h"

class uiSprite;
class playerStatusUI : public uiBase
{
public:
	enum eHpIconState
	{
		eHpIconState_Idle,
		eHpIconState_Crash,
		eHpIconState_Enpty,
		
		eHpIconState_None,
		eHpIconState_Count = eHpIconState_None,
	};


private:
	vector<uiSprite*> _hp;

public:
	playerStatusUI();
	~playerStatusUI();

	HRESULT init();
	void release();
	void update();
	void render();

	void setHpStatus(UINT count);
};

