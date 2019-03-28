#include "stdafx.h"
#include "uiList.h"
#include "uiScroll.h"


uiList::uiList()
: _scroll(nullptr)
, _countPerLine(1)
, _linePerPage(0)
, _countLine(0)
, _curLine(0)
, _gapX(0.f)
, _gapY(0.f)
{
}


uiList::~uiList()
{
}

HRESULT uiList::init(float x, float y, float width, float height)
{
	uiObject::init(x, y, width, height);
	_img = IMGDATABASE->getImage(eImage_UI_BG5);

	_cellWidth = width;
	_cellHeight = 10.f;
	return S_OK;
}

HRESULT uiList::init(float x, float y, float width, float height, image* img)
{
	uiObject::init(x, y, width, height);
	_img = img;
	if(!_img)
		_img = IMGDATABASE->getImage(eImage_UI_BG5);

	return S_OK;
}

void uiList::release()
{
}

void uiList::update()
{
	if(!_isActive)
		return;

	if (_scroll)
	{
		_scroll->update();
		if (0 < _childCount)
		{
			int value = (_childCount / _countPerLine) - _linePerPage;

			if(value < 0)
				_curLine = 0;
			else
				_curLine = static_cast<int>(_scroll->getValue() * value );
			
		}
	}
	reSort();

	uiObject::update();
	DEVTOOL->pushBackDebugText(format(L"list Count : %d", _childCount));
}

void uiList::render()
{
	if(!_isViewing)
		return;

	_img->render(_rc, 1.f, true);
	int count = (_linePerPage + _curLine + 1) * _countPerLine - 1;

	int idx = 0;
	for (int ii = _curLine; ii <= count && ii < _childCount; ++ii)
	{
		_childs[ii]->render();
	}

	if(_scroll)
		_scroll->render();
}

void uiList::insertChild(uiObject* ui)
{
	uiObject::insertChild(ui);

	ui->setWidth(_cellWidth);
	ui->setHeight(_cellHeight);

	if (_scroll)
	{
		float scrollSize;
		float totalSize = ((_childCount / _countPerLine) * (_gapY + _cellHeight));
		if(totalSize <= _height)
			scrollSize = _height;
		else
			scrollSize = (_height * _height) / totalSize ;

	//w : _height = _height : x
	//		_height * _height = w * x

		_scroll->setScrollSize(scrollSize);
	}
}

void uiList::removeChild(uiObject* ui)
{
	uiObject::removeChild(ui);

	if (_scroll)
	{
		float scrollSize;
		float totalSize = ((_childCount / _countPerLine) * (_gapY + _cellHeight));
		if (totalSize <= _height)
			scrollSize = _height;
		else
			scrollSize = (_height * _height) / totalSize;
		
		_scroll->setScrollSize(scrollSize);
	}
}

void uiList::reSort()
{
	POINTF pf = { -_cellWidth, -_cellHeight };
	
	//_curLine -= count;
	//if (_curLine < 0)
	//	_curLine = 0;

	int idx = 0;
	int idxX = 0;
	int idxY = 0;
	int count = _linePerPage * (_countPerLine + 1) - 1;
	for (int ii = (_curLine * _countPerLine), idx = 0; idx <= count && ii < _childCount; ++ii, ++idx)
	{
		idxX = idx % _countPerLine;
		idxY = idx / _countPerLine;

		pf.x = _gapX + (_gapX + _cellWidth) * idxX;
		pf.y = _gapY + (_gapY + _cellHeight) * idxY;

		_childs[ii]->setLocalPosition(pf.x, pf.y);
	}
}

void uiList::setScroll(uiScroll* scroll)
{
	_scroll = scroll;
	if (_scroll)
	{
		_scroll->setParent(this);
		_cellWidth = (_width - ((_countPerLine + 1) * _gapX) - _scroll->getWidth()) / _countPerLine;
	}
}

void uiList::setGap(float gapX, float gapY)
{
	_gapX = gapX;
	_gapY = gapY;
}

void uiList::setCountPerLine(int count)
{
	_countPerLine = count;

	if(_scroll)
		_cellWidth = (_width - ((count + 1) * _gapX) - _scroll->getWidth()) / count;
	else
		_cellWidth = (_width - ((count + 1) * _gapX)) / count;
}

void uiList::setCellHeight(float height)
{
	_cellHeight = height;
	_linePerPage = static_cast<int>(_height / (_gapY + _cellHeight));
}
