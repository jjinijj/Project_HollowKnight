#include "stdafx.h"
#include "devTool.h"


devTool::devTool()
{
	_debugText.clear();
}


devTool::~devTool()
{
}

HRESULT devTool::init()
{
	return S_OK;
}

void devTool::release()
{
	debugTextClear();
}

void devTool::update()
{
	debugTextClear();
}

void devTool::render()
{
	if (checkDebugMode(DEBUG_SHOW_TEXT))
	{
		list<wstring>::iterator iter = _debugText.begin();
		list<wstring>::iterator end = _debugText.end();

		for (int ii = 0; end != iter; ++iter, ++ii)
		{
			D2DMANAGER->drawText((*iter).c_str(), 20, 20 + ii * 20.f);
		}
	}
}


void devTool::pushBackDebugText(string str)
{
	wstring wstr = string2wstring(str);
	_debugText.push_back(wstr);
}

void devTool::pushBackDebugText(wstring str)
{
	_debugText.push_back(str);
}

void devTool::clearDebugMode()
{
	_debugMode = NULL;
}

void devTool::setDebugMode(WORD mode)
{
	_debugMode |= mode;
}

void devTool::delDebugMode(WORD mode)
{
	_debugMode ^= mode;
}

bool devTool::checkDebugMode(WORD mode)
{
	return ((_debugMode & mode) == mode);
}

void devTool::debugTextClear()
{
	list<wstring>::iterator iter = _debugText.begin();
	list<wstring>::iterator end = _debugText.end();

	for (; end != iter; ++iter)
	{
		(*iter).clear();
	}

	_debugText.clear();
}
