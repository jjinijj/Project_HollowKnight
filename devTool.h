#pragma once
#include "singletonBase.h"
class devTool: public singletonBase<devTool>
{
private:
	list<wstring> _debugText;

public:
	devTool();
	~devTool();

	HRESULT init();
	void release();
	void update();
	void render();

	void pushBackDebugText(string str);
	void pushBackDebugText(wstring str);

private:
	void debugTextClear();
};

