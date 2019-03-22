#pragma once
#include "npc.h"
class elderbug: public npc
{
public:
	enum
	{
		ELDERBUG_ANI_SPEED = 5,

		ELDERBUG_WIDTH = 50,
		ELDERBUG_HEIGHT = 100,

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

	HRESULT init(UINT uid, float x, float y);
	void release();
	void update();
	void render();
};

