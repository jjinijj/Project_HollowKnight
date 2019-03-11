#pragma once
#include "uiObject.h"
class uiScroll: public uiObject
{
protected:
	image* _front;
	image* _back;

	bool _isHorizontal;		// 가로방향인가 false일 경우 세로방향
	bool _isPickingScroll;	// 

	float _scrollSize;
	float _value;  // 0.f ~ 1.f

	float _frontWidth;
	float _frontHeight;

	POINTF _start;
	
	RECTD2D _frontRc;


public:
	uiScroll();
	~uiScroll();

	HRESULT init(float x, float y, float width, float height, image* front, image* back);
	void release() override;
	void update()  override;
	void render()  override;

	void setScrollDirect(bool isHorizontal) { _isHorizontal = isHorizontal; }

	void setScrollSize(float size);
	void setScrollSize();
	void setValue(float value);

	float getValue() { return _value; }

private:
	void onceKeyDownMouseL();
	void onceKeyUpMouseL();
	void stayKeyMouseL();

	void onceKeyDownMouseR();
	void onceKeyUpMouseR();
	void stayKeyMouseR();
};

