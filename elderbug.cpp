#include "stdafx.h"
#include "elderbug.h"
#include "elderbugState.h"


elderbug::elderbug()
{
}


elderbug::~elderbug()
{
}

HRESULT elderbug::init()
{
	_subType = eNpc_Elderbug;

	_width = ELDERBUG_WIDTH;
	_colWidth = ELDERBUG_WIDTH;
	_height = ELDERBUG_HEIGHT;
	_colHeight = ELDERBUG_HEIGHT;

	return S_OK;
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
	_width = ELDERBUG_WIDTH;
	_colWidth = ELDERBUG_WIDTH;
	_height = ELDERBUG_HEIGHT;
	_colHeight = ELDERBUG_HEIGHT;

	elderbugIdle* idleState = new elderbugIdle;
	idleState->init(this);

	_dir = eRIGHT;
	
	_state = idleState;
	_state->start();

	_dialogIdx = NULL;

	updateRect();

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
