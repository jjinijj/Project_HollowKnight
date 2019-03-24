#pragma once
#include "actorBase.h"
class npc :	public actorBase
{
protected:
	UINT _dialogIdx;
	int _idx;

	map<UINT, vector<wstring>> _dialogMap;
	vector<string> _vSound;

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

	

	virtual bool isTalkEnd();
	virtual void talkStart();
	virtual void talkEnd();
	virtual wstring getNextDialog();

protected:
	virtual void initSetDialog() = 0;
};

