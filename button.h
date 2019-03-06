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
	wstring _text;
	
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
	
	void setText(wstring text, int fontSize);

	void rePosition(float x, float y);
	void reSize(float width, float height);

	RECTD2D getRect() {return _rc;}

	float getWidth() { return _width; }
	float getHeight(){ return _height;}
};

