#pragma once
#include "singletonBase.h"
#include "uiBase.h"


enum eUIType
{
	eUI_MapTool,
	eUI_Store,
	eUI_Title,
	eUI_Dialog,
	eUI_Status,
	eUI_ExitPop,

	eUI_None,
	eUI_Count = eUI_None,
};

class windowDialog;
class playerStatusUI;
class uiManager: public singletonBase<uiManager>
{
private:
	typedef map<eUIType, uiBase*> uiMap;
	typedef map<eUIType, uiBase*>::iterator iterUiMap;

private:
	uiMap _uiMap;
	
	UINT _aniUID;

	windowDialog* _dialogUi;
	playerStatusUI* _statusUi;

public:
	uiManager();
	~uiManager();

	HRESULT init();
	void release();
	void update();
	void render();

	void insertUI(eUIType uid, uiBase* ui);
	uiBase* getUI(eUIType uid);

	void setDialogUI(windowDialog* ui) {_dialogUi = ui; }
	void setStatusUI(playerStatusUI* ui) {_statusUi = ui;}

	windowDialog* getDialogUI() { return _dialogUi; }
	playerStatusUI* getStatusUI() { return _statusUi; }

	UINT getUIAniUID()
	{
		return _aniUID++;
	}
};

