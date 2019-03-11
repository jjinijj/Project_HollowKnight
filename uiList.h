#pragma once
#include "uiObject.h"

class uiScroll;
class uiList : public uiObject
{
protected:
	uiScroll* _scroll;

	int _countPerLine;
	int _linePerPage;
	int _countLine;

	int _curLine;

	float _gapX;
	float _gapY;

	float _cellWidth;
	float _cellHeight;

public:
	uiList();
	~uiList();

	HRESULT init(float x, float y, float width, float height) override;
	void release() override;
	void update() override;
	void render() override;

	void insertChild(uiObject* ui) override;

	// ÀçÁ¤·Ä
	void reSort();

	void setScroll(uiScroll* scroll);
	void setGap(float gapX, float gapY);
	void setCountPerLine(int count);
	void setCellHeight(float height);
};

