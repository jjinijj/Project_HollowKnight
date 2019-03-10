#pragma once
#include "uiObject.h"

class uiProgress : public uiObject
{
protected:
	image* _front;
	image* _back;

	RECTD2D _frontRc;

	bool _isFrontSizeFix;	// 스크롤 크기 고정을 할것인가
	bool _isHorizontal;		// 가로방향인가 false일 경우 세로방향
	bool _isScroll;			// 스크롤 형식인가

	float _value;  // 0.f ~ 1.f

public:
	uiProgress();
	~uiProgress();

	HRESULT init(float x, float y, float width, float height, image* front, image* back);
	void release() override;
	void update()  override;
	void render()  override;

	void setScrollDirect(bool isHorizontal) {_isHorizontal = isHorizontal;}

	void setScrollSize(float size);
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

