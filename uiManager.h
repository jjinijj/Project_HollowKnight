#pragma once
#include "singletonBase.h"
#include "uiBase.h"


class uiManager: public singletonBase<uiManager>
{
private:
	typedef map<UINT, uiBase*> uiMap;
	typedef map<UINT, uiBase*>::iterator iterUiMap;

private:
	uiMap _uiMap;

public:
	uiManager();
	~uiManager();

	HRESULT init();
	void release();
	void update();
	void render();

	void insetUI(UINT uid, uiBase* ui);
	uiBase* getUI(UINT uid);
};

