#pragma once
#include "singletonBase.h"
#include "uiBase.h"

enum eUIType
{
	eUI_MapTool,

	eUI_None,
	eUI_Count = eUI_None,
};

class uiManager: public singletonBase<uiManager>
{
private:
	typedef map<eUIType, uiBase*> uiMap;
	typedef map<eUIType, uiBase*>::iterator iterUiMap;

private:
	uiMap _uiMap;

public:
	uiManager();
	~uiManager();

	HRESULT init();
	void release();
	void update();
	void render();

	void insertUI(eUIType uid, uiBase* ui);
	uiBase* getUI(eUIType uid);
};

