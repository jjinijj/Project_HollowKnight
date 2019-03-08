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
};

