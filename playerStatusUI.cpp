#include "stdafx.h"
#include "playerStatusUI.h"
#include "uiHeaders.h"


playerStatusUI::playerStatusUI()
{
}


playerStatusUI::~playerStatusUI()
{
}

HRESULT playerStatusUI::init()
{
	for (int ii = 0; ii < 5; ++ii)
	{
		uiSprite* sprite = new uiSprite;
		sprite->init(ii * 80 + 260, 50, "hp");
		
		UINT uid = UIMANAGER->getUIAniUID();
		sprite->setAnimationUid(uid);

		ANIMANAGER->addArrayFrameAnimation(uid, eHpIconState_Idle, "hp"
										   ,0, 4, 5, true);
		ANIMANAGER->addArrayFrameAnimation(uid, eHpIconState_Crash, "hp"
										   ,7, 13, 5, false);

		sprite->setAnimation(eHpIconState_Idle);
		_hp.push_back(sprite);

		insertUIObject(sprite);
	}


	image* front = IMAGEMANAGER->findImage("skillGauge_in");
	image* back	= IMAGEMANAGER->findImage("skillGauge_out");
	_uiGauge = new uiProgress;
	_uiGauge->init(100, 50, front->GetWidth(), front->GetHeight(), front, back);
	_uiGauge->setScrollDirect(false);
	_uiGauge->setValue(0.0);
	insertUIObject(_uiGauge);

	_coinUi = IMAGEMANAGER->findImage("coin_ui");
	_textImg = IMAGEMANAGER->findImage("number");
	

	return S_OK;
}

void playerStatusUI::release()
{
	uiBase::release();
	_hp.clear();
	_uiGauge = nullptr;
}

void playerStatusUI::update()
{
	uiBase::update();
}

void playerStatusUI::render()
{
	uiBase::render();
	_coinUi->render(270, 180, 1.f, true);
	
	list<int>::iterator iter = _idxs.begin();
	list<int>::iterator end = _idxs.end();
	int ii = 0;
	for(iter; end != iter; ++iter)
	{
		_textImg->frameRender(ii * 50 + 350.f
							  , 180.f
							  , *iter
							  , 0
							  , 1.0f
							  , true);
		++ii;
	}
}

void playerStatusUI::setHpStatus(UINT count)
{
	if(count <= 0)
		count = 0;
	else if(5 < count)
		count = 5;

	int cnt = count;

	for (int ii = 0; ii < 5; ++ii)
	{
		if (ii <= (cnt - 1))
		{
			if(_hp[ii]->getAniKey() != eHpIconState_Idle)
				_hp[ii]->setAnimation(eHpIconState_Idle);
		}
		else
		{
			if(_hp[ii]->getAniKey() != eHpIconState_Crash)
				_hp[ii]->setAnimation(eHpIconState_Crash);
		}
	}
}

void playerStatusUI::setGaugeValue(float value)
{
	if(value < 0)
		value = 0.f;
	else if(1.f < value)
		value = 1.f;

	_uiGauge->setValue(value);
}

void playerStatusUI::setGaugeChangeValue(float value)
{
	_uiGauge->changeValue(value);
}

void playerStatusUI::setCoin(UINT price)
{
	_idxs.clear();

	if(0 == price)
	{
		_idxs.push_back(0);
		return;
	}

	int value = price;
	while (0 != value)
	{
		int temp = value /10;

		if (0 == temp)
		{
			_idxs.push_back(value);
			break;
		}
		else
		{
			_idxs.push_back(temp);
			value %= 10;
		}
	}
}
