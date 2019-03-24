#include "stdafx.h"
#include "npc.h"


npc::npc()
: _dialogIdx(NULL)
{
}


npc::~npc()
{
}

HRESULT npc::init()
{
	return S_OK;
}

HRESULT npc::init(UINT uid, float x, float y)
{
	actorBase::init(uid, x, y);
	_type = eActor_Npc;

	return S_OK;
}

void npc::release()
{
	actorBase::release();
}

void npc::update()
{
	actorBase::update();
}

void npc::render()
{
	actorBase::render();
}

ACTORPACK* npc::makePack()
{
	ACTORPACK* pack = actorBase::makePack();
	if(pack)
	{
		pack->value = _dialogIdx;
	}

	return pack;
}

void npc::loadPack(ACTORPACK* pack)
{
	if (pack)
	{
		actorBase::loadPack(pack);
		_dialogIdx = pack->value;
	}
}

void npc::changeState()
{
}

bool npc::isTalkEnd()
{
	return false;
}

void npc::talkStart()
{
}

wstring npc::getNextDialog()
{
	return wstring();
}
