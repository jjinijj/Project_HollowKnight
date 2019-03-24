#pragma once
#include "actorBase.h"
class npc :	public actorBase
{
protected:
	UINT _dialogIdx;
	map<UINT, vector<wstring>> _dialogMap;

public:
	npc();
	~npc();

	virtual HRESULT init();
	virtual HRESULT init(UINT uid, float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual ACTORPACK* makePack();
	virtual void loadPack(ACTORPACK* pack);

	virtual void changeState();

	virtual bool isTalkEnd();
	virtual void talkStart();
	virtual wstring getNextDialog();
};

