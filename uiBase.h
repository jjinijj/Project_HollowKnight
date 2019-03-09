#pragma once
#include "uiObject.h"

class uiBase
{
protected:
	UINT _uid;
	
	bool _isOpen; 
	bool _isActive;

	list<uiObject*> _uiList;

public:
	uiBase();
	~uiBase();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void uiOpen();
	virtual void uiClose();

	void insertUIObject(uiObject* ui);

	void setActive(bool isActive) { _isActive = isActive; }

	bool isOpen()	{ return _isOpen;}
	bool isActive() { return _isActive;}
};

