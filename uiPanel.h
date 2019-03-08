#pragma once
#include "uiObject.h"
class uiPanel: public uiObject
{
private:
	image* _img;

	function<void(void)> _onFunction;
	function<void(void)> _pressFunction;
	function<void(void)> _offFunction;

public:
	uiPanel();
	~uiPanel();

	HRESULT init(float x, float y, float width, float height, image* img);
	void release() override;
	void update()  override;
	void render()  override;

	// 이미지 교체
	void changeImage(image* img);
	// 크기 재정의
	void resize();
	// 눌렀을 때 함수 
	void setOnClickFunction(function<void(void)> func);
	// 누르고 있을 때 함수
	void setPressFunction(function<void(void)> func);
	// 누르기 해제 했을 때 함수
	void setOnClickUPFunction(function<void(void)> func);
};

