#include "stdafx.h"
#include "uiObject.h"


uiObject::uiObject()
: _uid(0)
, _isActive(false)
, _isCollider(false)
, _x(0.f)
, _y(0.f)
, _width(0.f)
, _height(0.f)
, _childCount(0)
{
	_childs.clear();
	_rc = {};
}


uiObject::~uiObject()
{
}

HRESULT uiObject::init()
{
	_isActive = true;
	_isCollider = true;
	return S_OK;
}

HRESULT uiObject::init(float x, float y, float width, float height)
{
	init();
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	_rc = { _x, _y, _x + width, _y + _height };

	vector<pair<int, float>> v;

	v.push_back(make_pair(0, 0.f));

	return S_OK;
}

void uiObject::release()
{
	while (0 != _childs.size())
	{
		uiObject* obj = _childs.back();
		_childs.pop_back();

		SAFE_RELEASE(obj);
		SAFE_DELETE(obj);
	}

	_childs.clear();
}

void uiObject::update()
{
	if(!_isActive)
		return;

	// 제일 위에 있는 ui부터 실행
	for (int ii = _childCount - 1; 0 <= ii; --ii)
	{
		_childs[ii]->update();
	}

	if (_isCollider)
	{
		if (PtInRectD2D(_rc, _ptMouse))
			inputKey();
		else
			releaseInputKey();
	}
}

void uiObject::render()
{
	// 제일 밑에 있는 ui부터 렌더
	for (int ii = 0; ii < _childCount; ++ii)
	{
		_childs[ii]->render();
	}
}

void uiObject::insertChild(uiObject* ui)
{
	for (int ii = 0; ii < _childCount; ++ii)
	{
		if(ui == _childs[ii])
			return;
	}

	_childs.push_back(ui);
	_childCount = _childs.size();
}

uiObject* uiObject::getChild(int idx)
{
	if(idx < 0 || _childs.size() < idx )
		return nullptr;
	else
		return _childs[idx];
}


POINTF uiObject::getWorldPosition()
{
	POINTF pf = {_x, _y};
	if (_parent)
	{
		POINTF ppf = _parent->getWorldPosition();
		pf.x += ppf.x;
		pf.y += ppf.y;
	}

	return pf;
}

POINTF uiObject::getLocalPosition()
{
	return PointFMake(_x, _y);
}

void uiObject::inputKey()
{
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		onceKeyDownMouseL();
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
		onceKeyUpMouseL();
	if(KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		stayKeyMouseL();

	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
		onceKeyDownMouseR();
	if (KEYMANAGER->isOnceKeyUp(VK_RBUTTON))
		onceKeyUpMouseR();
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
		stayKeyMouseR(); 
}

void uiObject::releaseInputKey()
{
}

void uiObject::onceKeyDownMouseL()
{
}

void uiObject::onceKeyUpMouseL()
{
}

void uiObject::stayKeyMouseL()
{
}

void uiObject::onceKeyDownMouseR()
{
}

void uiObject::onceKeyUpMouseR()
{
}

void uiObject::stayKeyMouseR()
{
}
