#pragma once
#include "uiObject.h"

class uiProgress : public uiObject
{
protected:
	image* _front;
	image* _back;

	RECTD2D _frontRc;

	bool _isFrontSizeFix;	// ��ũ�� ũ�� ������ �Ұ��ΰ�
	bool _isHorizontal;		// ���ι����ΰ� false�� ��� ���ι���
	bool _isScroll;			// ��ũ�� �����ΰ�

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

