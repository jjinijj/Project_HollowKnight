#pragma once
#include "gameNode.h"
#include<functional>

enum eButtonState
{
	eButton_Up,
	eButton_Press,
	eButton_Down,

	eButton_None,
	eButton_Count = eButton_None,
};

class button : public gameNode
{
private:
	
	eButtonState _state;
	image* _imgs[eButton_Count];
	wstring _text[eButton_Count];
	
	bool _isToggle;
	bool _isToggleOn;

	int _fontSize;

	float _width;
	float _height;

	RECTD2D _rc;

	float _x, _y;		//위치 좌표
	
	function<void(void)> _onFunction;
	function<void(void)> _offFunction;


public:
	button();
	~button();

	// 일반 버튼
	HRESULT init( const char* upImgName
				 , const char* pressImgName
				 , float destX, float destY
				 , float width, float height
				 , function<void(void)> func);
				
	// 토글 속성 버튼
	HRESULT init( const char* upImgName
				 ,const char* pressImgName
				 ,const char* downImgName
				 ,float destX, float destY
				 ,float width, float height
				 ,function<void(void)> toggleOnFunc
				 , function<void(void)> toggleOffFunc);


	void release();
	void update();
	void render();
	
	// 텍스트, 버튼 상태, 크기(default)
	void setText(wstring text);
	// 텍스트, 크기
	void setText(wstring text, int fontSize);
	// 텍스트, 버튼 상태, 크기(default)
	void setText(wstring text, eButtonState state);
	// 텍스트, 버튼 상태, 크기
	void setText(wstring text, eButtonState state, int fontSize);

	// 버튼 상태
	void setState(eButtonState state);
	eButtonState getState() {return _state;}

	void rePosition(float x, float y);
	void reSize(float width, float height);

	RECTD2D getRect() {return _rc;}

	float getWidth() { return _width; }
	float getHeight(){ return _height;}
};

