#pragma once
#include "npc.h"
class cornifer : public npc
{
public:
	enum
	{
		CORNIFER_ANI_SPEED = 5,

		CORNIFER_WIDTH = 317,
		CORNIFER_HEIGHT = 188,
	};

	enum eState
	{
		eIdle,
		eTalk_Left,
		eTalk_Right,

		eNone,
		eCount = eNone,
	};

public:
	cornifer();
	~cornifer();

	HRESULT init();
	HRESULT init(UINT uid, float x, float y);
	void release();
	void update();
	void render();

	bool isTalkEnd();
	void talkStart() override;
	void talkEnd() override;
	wstring getNextDialog() override;

private:
	void initSetDialog();
	void humming();
};

