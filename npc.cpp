#include "stdafx.h"
#include "npc.h"


npc::npc()
{
}


npc::~npc()
{
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

void npc::changeState()
{
}

void npc::talkStart()
{
}
