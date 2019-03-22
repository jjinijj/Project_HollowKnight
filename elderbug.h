#pragma once
#include "npc.h"
class elderbug: public npc
{
public:
	enum
	{
		ELDERBUG_ANI_SPEED = 5,

		ELDERBUG_WIDTH = 110,
		ELDERBUG_HEIGHT = 160,

	};

	enum eState
	{
		eIdle,
		eTalk,

		eNone,
		eCount = eNone,
	};

public:
	elderbug();
	~elderbug();

	HRESULT init();
	HRESULT init(UINT uid, float x, float y);
	void release();
	void update();
	void render();
};

