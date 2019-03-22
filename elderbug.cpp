#include "stdafx.h"
#include "elderbug.h"
#include "elderbugState.h"


elderbug::elderbug()
{
}


elderbug::~elderbug()
{
}

HRESULT elderbug::init(UINT uid, float x, float y)
{
	npc::init(uid, x, y);
	_subType = eNpc_Elderbug;
	_name = L"Elderbug";

	{
		image* img = IMAGEMANAGER->findImage("elderbug");
		ANIMANAGER->addArrayFrameAnimation( uid, eIdle, "elderbug"
										   ,0, img->GetMaxFrameX(), ELDERBUG_ANI_SPEED, true);
	}

	elderbugIdle* idleState = new elderbugIdle;
	idleState->init(this);

	_dir = eRIGHT;
	
	_state = idleState;
	_state->start();

	_collision = {	 _x - ELDERBUG_WIDTH / 2.f, _y - ELDERBUG_HEIGHT
					,_x + ELDERBUG_WIDTH / 2.f, _y};

	_dialogIdx = NULL;

	return S_OK;
}

void elderbug::release()
{
	npc::release();
}

void elderbug::update()
{
	npc::update();
}

void elderbug::render()
{
	npc::render();
}
