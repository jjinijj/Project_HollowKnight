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
		sprite->init(ii * 80 + 160, 10, "hp");
		
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

	return S_OK;
}

void playerStatusUI::release()
{
	uiBase::release();
	_hp.clear();
}

void playerStatusUI::update()
{
	uiBase::update();
}

void playerStatusUI::render()
{
	uiBase::render();
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
