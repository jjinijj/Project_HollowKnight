#pragma once
#include "singletonBase.h"
#include "fontLoader.h"

class camera;
class d2dManager : public singletonBase<d2dManager>
{
public:
	ID2D1Factory*				_d2dFactory;
	ID2D1HwndRenderTarget*		_renderTarget;
	IDWriteFactory*				_writeFactory;
	IDWriteTextFormat*			_defaultTextFormat;		//기본 텍스트 포맷
	IDWriteTextFormat*			_customTextFormat;		//커스텀 텍스트 포맷
	ID2D1SolidColorBrush*		_defaultBrush;			//기본 검정색 브러쉬

	IDWriteFontCollection*		_collection;			//폰트들이 모여있는곳~ 커스텀폰트 쓸때 씀ㅋ
	
	IWICImagingFactory*		WICImagingFactory;

	map<wstring, map<UINT, IDWriteTextFormat*>> _fontFormatMap;

public:
	d2dManager();
	~d2dManager();

	HRESULT init();
	void release();
	void update();
	void render();
	void beginDraw();
	void endDraw();
	ID2D1SolidColorBrush* createBrush(COLORREF rgb, float opacity = 1.0f);


	HRESULT createCollection(wstring path);


	
	//===================================================================
	//			Draw, Fill Method (Line, Rectangle, Ellipse)
	//- 카메라 이동 보정 & 범위에 들어가는 내용만 렌더
	//===================================================================
	

	void drawLine(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, float strokeWidth = 1.0f);
	//검은색
	void drawLine(float startX, float startY, float endX, float endY);
	//색깔선
	void drawLine(COLORREF rgb, float startX, float startY, float endX, float endY);


	void drawRectangle(D2D_RECT_F rc, bool isAbsolute = true);
	void drawRectangle(float left, float top, float right, float bottom, bool isAbsolute = true);
	void drawRectangle(COLORREF rgb, D2D_RECT_F rc, bool isAbsolute = true);
	void drawRectangle(COLORREF rgb, float left, float top, float right, float bottom, bool isAbsolute = true);


	void drawEllipse(COLORREF rgb, float left, float top, float right, float bottom);
	void drawEllipse(float left, float top, float right, float bottom);

	void drawEllipse(D2D1_ELLIPSE e);
	void drawEllipse(COLORREF rgb, D2D1_ELLIPSE e);




	void fillRectangle(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool isAbsolute = true);
	void fillRectangle(COLORREF rgb, D2D_RECT_F rc, bool isAbsolute = true);
	void fillEllipse(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY);
	void fillEllipse(COLORREF rgb, D2D1_ELLIPSE e);

	
	void drawText(LPCWSTR string, float x, float y, bool isAbsolute = true);
	void drawText(LPCWSTR string, float x, float y, int fontSize, bool isAbsolute = true);
	void drawText(LPCWSTR string, float x, float y, int fontSize, COLORREF rgb, bool isAbsolute = true);
	//출력할 문자열, 문자열을 그릴 Rect의 Left, Top, Right, Bottom, 그릴 브러쉬
	void drawTextD2D(ID2D1SolidColorBrush* brush, LPCWSTR string, float startX, float startY, float endX, float endY);

	//출력할 문자열, 문자열을 그릴 Rect의 Left, Top, Right, Bottom, 그릴 브러쉬, 폰트명, 폰트 크기
	void drawTextD2D(ID2D1SolidColorBrush* brush, LPCWSTR fontName, float fontSize, LPCWSTR string, float startX, float startY, float endX, float endY);



	//===================================================================
	//			내부 함수
	//===================================================================

private:
	D2D1_RECT_F getDrawRectfArea( float startX, float startY, float endX, float endY);
	bool		isRectFInRangeWindow(const D2D1_RECT_F& rcf );

	// 폰트
	IDWriteTextFormat* getFontFormat(wstring fontName, UINT fontSize);
};

