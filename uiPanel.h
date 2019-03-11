#pragma once
#include "uiObject.h"

class iTween;
class uiPanel: public uiObject
{
private:
	image* _img;
	iTween* _itween;

	function<void(void)> _onFunction;
	function<void(void)> _pressFunction;
	function<void(void)> _offFunction;
	function<void(void)> _hoverFunction;

public:
	uiPanel();
	~uiPanel();

	HRESULT init(float x, float y, float width, float height, image* img);
	void release() override;
	void update()  override;
	void render()  override;

	// �̹��� ��ü
	void changeImage(image* img);
	// ũ�� ������
	void resize();
	// ������ �� �Լ� 
	void setOnClickFunction(function<void(void)> func);
	// ������ ���� �� �Լ�
	void setPressFunction(function<void(void)> func);
	// ������ ���� ���� �� �Լ�
	void setOnClickUPFunction(function<void(void)> func);
	// hover�� �� �Լ�
	void setHoverFunction(function<void(void)> func);
	
	// itween
	void setiTween(iTween* it) { _itween = it; }
	iTween* getiTween() { return _itween; }

private:
	void onceKeyDownMouseL();
	void onceKeyUpMouseL();
	void stayKeyMouseL();

	void onceKeyDownMouseR();
	void onceKeyUpMouseR();
	void stayKeyMouseR();
};

