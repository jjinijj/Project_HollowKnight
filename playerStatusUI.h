#pragma once
#include "uiBase.h"

class uiSprite;
class uiProgress;
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
	vector<uiSprite*>	_hp;
	uiProgress*			_uiGauge;

public:
	playerStatusUI();
	~playerStatusUI();

	HRESULT init();
	void release();
	void update();
	void render();

	void setHpStatus(UINT count);

	// 0.0 ~ 1.0f
	void setGaugeValue(float value);
	void setGaugeChangeValue(float value);
};

