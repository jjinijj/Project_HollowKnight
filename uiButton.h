#pragma once
#include<functional>

enum eButtonState
{
	eButton_Up,
	eButton_Press,
	eButton_Down,

	eButton_None,
	eButton_Count = eButton_None,
};

class uiButton : public uiObject
{
private:
	
	eButtonState _state;
	image* _imgs[eButton_Count];
	wstring _text[eButton_Count];
	
	bool _isToggle;
	bool _isToggleOn;

	int _fontSize;

	function<void(void)> _onFunction;
	function<void(void)> _pressFunction;
	function<void(void)> _offFunction;

public:
	uiButton();
	~uiButton();

	// �Ϲ� ��ư
	HRESULT init( const char* upImgName
				 , const char* pressImgName
				 , float destX, float destY
				 , float width, float height);

	// ��� �Ӽ� ��ư
	HRESULT init( const char* upImgName
				 ,const char* pressImgName
				 ,const char* downImgName
				 ,float destX, float destY
				 ,float width, float height);


	void release();
	void update();
	void render();
	
	// �ؽ�Ʈ, ��ư ����, ũ��(default)
	void setText(wstring text);
	// �ؽ�Ʈ, ũ��
	void setText(wstring text, int fontSize);
	// �ؽ�Ʈ, ��ư ����, ũ��(default)
	void setText(wstring text, eButtonState state);
	// �ؽ�Ʈ, ��ư ����, ũ��
	void setText(wstring text, eButtonState state, int fontSize);

	// ������ �� �Լ� 
	void setOnClickFunction(function<void(void)> func);
	// ������ ���� �� �Լ�
	void sePressFunction(function<void(void)> func);
	// ������ ���� ���� �� �Լ�
	void setOnClickUPFunction(function<void(void)> func);


	// ��ư ����
	void setState(eButtonState state);
	eButtonState getState() {return _state;}

	void rePosition(float x, float y);
	void reSize(float width, float height);

	RECTD2D getRect() {return _rc;}

	float getWidth() { return _width; }
	float getHeight(){ return _height;}

private:
	void releaseInputKey() override;

	void onceKeyDownMouseL() override;
	void onceKeyUpMouseL() override;
	void stayKeyMouseL() override;

	void onceKeyDownMouseR() override;
	void onceKeyUpMouseR() override;
	void stayKeyMouseR() override;
};

