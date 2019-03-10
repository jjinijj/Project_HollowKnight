#pragma once
#include "uiObject.h"

class uiProgress;
class uiList : public uiObject
{
protected:
	uiProgress* _progress;

public:
	uiList();
	~uiList();
};

