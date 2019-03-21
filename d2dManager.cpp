#include "stdafx.h"
#include "d2dManager.h"


d2dManager::d2dManager()
{
}


d2dManager::~d2dManager()
{
}

HRESULT d2dManager::init()
{
	_d2dFactory = nullptr;
	_renderTarget = nullptr;

	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&WICImagingFactory));

	HRESULT hr = E_FAIL;

	//---------------------------------------------------------------------------
	//	D2D Factory 생성
	//---------------------------------------------------------------------------
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_d2dFactory);
	assert(hr == S_OK);

	//---------------------------------------------------------------------------
	//	Hwnd Render Target 생성
	//---------------------------------------------------------------------------
	hr = _d2dFactory->CreateHwndRenderTarget(RenderTargetProperties(),
		HwndRenderTargetProperties(_hWnd, SizeU(WINSIZEX, WINSIZEY)),
		&_renderTarget);
	assert(hr == S_OK);

	//---------------------------------------------------------------------------
	//	IDWriteFactory 생성
	//---------------------------------------------------------------------------
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(_writeFactory),
		reinterpret_cast<IUnknown **>(&_writeFactory));
	assert(hr == S_OK);
	
	// 브러쉬 생성
	_defaultBrush = createBrush(DEFAULT_FONT_COLOR);

	//	기본 텍스트 포맷 생성
	_writeFactory->CreateTextFormat(L"맑은고딕", NULL, DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, DEFAULT_FONT_SIZE, L"", &_defaultTextFormat);

	hr = createCollection(L"font/DungGeunMo.ttf");
	assert(hr == S_OK);

	// COM 라이브러리 초기화 -> 호출하지 않으면 CoCreateInstance가 제대로 수행되지 않음
	CoInitialize(NULL);


	return S_OK;
}

void d2dManager::release()
{
	for (iterMBrush iter = _brushMap.begin(); _brushMap.end() != iter;)
	{
		ID2D1SolidColorBrush* brush = (*iter).second;
		iter = _brushMap.erase(iter);

		SAFE_RELEASE2(brush);
	}

	map<wstring, mTextFormat>::iterator iter = _fontFormatMap.begin();
	for (; _fontFormatMap.end() != iter; )
	{
		mTextFormat tf = (*iter).second;
		for (iterMTextFormat it = tf.begin(); tf.end() != it; )
		{
			IDWriteTextFormat* wft = (*it).second;
			it = tf.erase(it);

			SAFE_RELEASE2(wft);
		}
		iter = _fontFormatMap.erase(iter);
	}

	_defaultBrush = nullptr;

	SAFE_RELEASE2(_defaultTextFormat);
	SAFE_RELEASE2(_writeFactory);
	SAFE_RELEASE2(_renderTarget);
	SAFE_RELEASE2(_d2dFactory);
	
	SAFE_RELEASE2(WICImagingFactory);
	SAFE_RELEASE2(_collection);
}

void d2dManager::update()
{
}

void d2dManager::render()
{

}

void d2dManager::beginDraw()
{
	_renderTarget->BeginDraw();
	_renderTarget->Clear(ColorF(BGCOLORCODE));
}

void d2dManager::endDraw()
{
	_renderTarget->EndDraw();
}

ID2D1SolidColorBrush* d2dManager::createBrush(COLORREF rgb, float opacity)
{
	HRESULT hr;
	ID2D1SolidColorBrush* brush = nullptr;
	
	if(_brushMap.find(rgb) != _brushMap.end())
		return _brushMap[rgb];

	hr = _renderTarget->CreateSolidColorBrush(ColorF(rgb, opacity), &brush);
	if(S_OK == hr)
	{
		_brushMap.insert(make_pair(rgb, brush));

		return brush;
	}
	else
	{
		return _defaultBrush;
	}
}

HRESULT d2dManager::createCollection(wstring path)
{
	HRESULT hr;
	// 내 폰트 컬렉션 생성
	MFFontContext fContext(_writeFactory);
	std::vector<std::wstring> filePaths; // vector containing ABSOLUTE file paths of the font files which are to be added to the collection
	std::wstring fontFileFilePath = path;
	filePaths.push_back(fontFileFilePath);
	hr = fContext.CreateFontCollection(filePaths, &_collection); // create custom font collection

	return hr;
}



//===================================================================
//			Draw, Fill Method (Line, Rectangle, Ellipse)
//- 카메라 이동 보정 & 범위에 들어가는 내용만 렌더
//===================================================================

void d2dManager::drawLine(ID2D1SolidColorBrush * brush, float startX, float startY, float endX, float endY, float strokeWidth)
{
	D2D1_RECT_F rcf = getDrawRectfArea(startX, startY, endX, endY);

	if ( !isRectFInRangeWindow(rcf) )
		return;

	_renderTarget->DrawLine(Point2F(rcf.left, rcf.top), Point2F(rcf.right, rcf.bottom), brush, strokeWidth);
}

void d2dManager::drawLine(float startX, float startY, float endX, float endY)
{
	D2D1_RECT_F rcf = getDrawRectfArea(startX, startY, endX, endY);

	if (!isRectFInRangeWindow(rcf))
		return;
	_renderTarget->DrawLine(Point2F(rcf.left, rcf.top), Point2F(rcf.right, rcf.bottom), _defaultBrush, 1);
}

void d2dManager::drawLine(COLORREF rgb, float startX, float startY, float endX, float endY)
{
	D2D1_RECT_F rcf = getDrawRectfArea(startX, startY, endX, endY);

	if (!isRectFInRangeWindow(rcf))
		return;
	_renderTarget->DrawLine(Point2F(rcf.left, rcf.top), Point2F(rcf.right, rcf.bottom), createBrush(rgb, 1), 1);
}



void d2dManager::drawRectangle(D2D_RECT_F rc, bool isAbsolute)
{
	drawRectangle(rc.left, rc.top, rc.right, rc.bottom, isAbsolute);
}

void d2dManager::drawRectangle(float left, float top, float right, float bottom, bool isAbsolute)
{
	D2D1_RECT_F rcf = {left, top, right, bottom};
	if (!isAbsolute)
		rcf = getDrawRectfArea(rcf.left, rcf.top, rcf.right, rcf.bottom);

	if (!isRectFInRangeWindow(rcf))
		return;

	_renderTarget->DrawRectangle(rcf, _defaultBrush, 1);
}

void d2dManager::drawRectangle(COLORREF rgb, D2D_RECT_F rc, bool isAbsolute)
{
	drawRectangle(rgb, rc.left, rc.top, rc.right, rc.bottom, isAbsolute);
}

void d2dManager::drawRectangle(COLORREF rgb, float left, float top, float right, float bottom, bool isAbsolute)
{
	D2D1_RECT_F rcf = { left, top, right, bottom };
	if(!isAbsolute)
		rcf = getDrawRectfArea(left, top, right, bottom);

	if ( !isRectFInRangeWindow(rcf) )
		return;

	_renderTarget->DrawRectangle(rcf, createBrush(rgb, 1), 1);
}


void d2dManager::drawEllipse(COLORREF rgb, float left, float top, float right, float bottom)
{
	D2D1_RECT_F rcf = getDrawRectfArea(left, top, right, bottom);

	if ( !isRectFInRangeWindow(rcf) )
		return;

	float width = right - left;
	float height = bottom - top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rcf.left + width * 0.5;
	ellipse.point.y = rcf.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	_renderTarget->DrawEllipse(&ellipse, createBrush(rgb, 1), 1);
}

void d2dManager::drawEllipse(float left, float top, float right, float bottom)
{
	D2D1_RECT_F rcf = getDrawRectfArea(left, top, right, bottom);

	if (!isRectFInRangeWindow(rcf))
		return;

	float width = right - left;
	float height = bottom - top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rcf.left + width * 0.5f;
	ellipse.point.y = rcf.top + height * 0.5f;
	ellipse.radiusX = width * 0.5f;
	ellipse.radiusY = height * 0.5f;

	_renderTarget->DrawEllipse(&ellipse, _defaultBrush, 1);
}

void d2dManager::drawEllipse(D2D1_ELLIPSE e)
{
	D2D1_RECT_F rcf = getDrawRectfArea(e.point.x - e.radiusX, e.point.y - e.radiusY, e.point.x + e.radiusX, e.point.y + e.radiusY);

	if (!isRectFInRangeWindow(rcf))
		return;

	_renderTarget->DrawEllipse(&e, _defaultBrush, 1);
}

void d2dManager::drawEllipse(COLORREF rgb, D2D1_ELLIPSE e)
{
	D2D1_RECT_F rcf = getDrawRectfArea(e.point.x - e.radiusX, e.point.y - e.radiusY, e.point.x + e.radiusX, e.point.y + e.radiusY);

	if (!isRectFInRangeWindow(rcf))
		return;

	_renderTarget->DrawEllipse(&e, createBrush(rgb, 1), 1);
}

void d2dManager::fillRectangle(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool isAbsolute)
{
	D2D1_RECT_F rcf = { startX, startY, endX, endY};
	if(!isAbsolute)
		rcf = getDrawRectfArea(startX, startY, endX, endY);

	if (!isRectFInRangeWindow(rcf))
		return;

	_renderTarget->FillRectangle( rcf, brush);
}

void d2dManager::fillRectangle(COLORREF rgb, D2D_RECT_F rc, bool isAbsolute)
{
	D2D1_RECT_F rcf = rc;
	
	if(!isAbsolute)
		rcf = getDrawRectfArea(rc.left, rc.top, rc.right, rc.bottom);

	if (!isRectFInRangeWindow(rcf))
		return;

	_renderTarget->FillRectangle(rcf, createBrush(rgb, 1));
}

void d2dManager::fillEllipse(ID2D1SolidColorBrush * brush, float startX, float startY, float endX, float endY)
{
	D2D1_RECT_F rcf = getDrawRectfArea(startX, startY, endX, endY);

	if (!isRectFInRangeWindow(rcf))
		return;

	float width = endX - startX;
	float height = endY - startY;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rcf.left + width * 0.5;
	ellipse.point.y = rcf.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	_renderTarget->FillEllipse(&ellipse, brush);

}

void d2dManager::fillEllipse(COLORREF rgb, D2D1_ELLIPSE e)
{
	D2D1_RECT_F rcf = getDrawRectfArea(e.point.x - e.radiusX, e.point.y - e.radiusY, e.point.x + e.radiusX, e.point.y + e.radiusY);

	if (!isRectFInRangeWindow(rcf))
		return;
	_renderTarget->FillEllipse(&e, createBrush(rgb, 1));

}

void d2dManager::drawText(LPCWSTR string, float x, float y, bool isAbsolute)
{
	drawText(string, x, y, DEFAULT_FONT_SIZE, isAbsolute);
}

void d2dManager::drawText(LPCWSTR string, float x, float y, UINT fontSize, bool isAbsolute)
{
	drawText(string, x, y, fontSize, DEFAULT_FONT_COLOR, isAbsolute);
}

void d2dManager::drawText(LPCWSTR string, float x, float y, UINT fontSize, COLORREF rgb, bool isAbsolute)
{
	D2D1_RECT_F rcf = { x, y, x + lstrlenW(string) * fontSize, y + 20 };

	if (!isAbsolute)
		rcf = getDrawRectfArea(x, y, x + lstrlenW(string) * fontSize, y + 20);

	if (!isRectFInRangeWindow(rcf))
		return;

	IDWriteTextFormat* font = getFontFormat(L"둥근모꼴", fontSize);
	_renderTarget->DrawTextA(string, lstrlenW(string), font, rcf, createBrush(rgb, 1));
}

void d2dManager::drawTextD2D(ID2D1SolidColorBrush * brush, LPCWSTR string, float startX, float startY, float endX, float endY)
{
	D2D1_RECT_F rcf = getDrawRectfArea(startX, startY, endX, endY);

	if (!isRectFInRangeWindow(rcf))
		return;

	_renderTarget->DrawTextA(string, lstrlenW(string), _defaultTextFormat, rcf, brush);
}

void d2dManager::drawTextD2D(ID2D1SolidColorBrush * brush, LPCWSTR fontName, float fontSize, LPCWSTR string, float startX, float startY, float endX, float endY)
{
	D2D1_RECT_F rcf = getDrawRectfArea(startX, startY, endX, endY);
	
	if( !isRectFInRangeWindow(rcf) )
		return;
	
	//	TextFormat 생성
	_writeFactory->CreateTextFormat(fontName, NULL, DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &_customTextFormat);


	_renderTarget->DrawTextA(string, lstrlenW(string), _customTextFormat, rcf, brush);

	SAFE_RELEASE2(_customTextFormat);
}

D2D1_RECT_F d2dManager::getDrawRectfArea( float startX, float startY, float endX, float endY)
{
	D2D1_RECT_F rc;
	rc = {	  startX - CAMERA->getPosX()
			, startY - CAMERA->getPosY()
			, endX	 - CAMERA->getPosX()
			, endY	 - CAMERA->getPosY()
		};

	return rc;
}

bool d2dManager::isRectFInRangeWindow(const D2D1_RECT_F& rcf)
{
	if ( rcf.left < 0 && rcf.right < 0 ) return false;
	if ( rcf.top < 0 && rcf.bottom < 0 ) return false;
	if ( WINSIZEX < rcf.left && WINSIZEX < rcf.right ) return false;
	if ( WINSIZEY < rcf.top &&  WINSIZEY < rcf.bottom) return false;

	return true;
}

IDWriteTextFormat* d2dManager::getFontFormat(wstring fontName, UINT fontSize)
{
	IDWriteTextFormat* font = nullptr;

	map<UINT, IDWriteTextFormat*> formats;
	if (_fontFormatMap.find(fontName) == _fontFormatMap.end())
		_fontFormatMap.insert(make_pair(fontName, formats));
	
	formats = _fontFormatMap[fontName];
	
	if (formats.find(fontSize) == formats.end())
	{
		_writeFactory->CreateTextFormat( fontName.c_str()
										, _collection
										,DWRITE_FONT_WEIGHT_REGULAR
										,DWRITE_FONT_STYLE_NORMAL
										,DWRITE_FONT_STRETCH_NORMAL
										,fontSize
										,L"", &font);
		_fontFormatMap[fontName].insert(make_pair(fontSize, font));
		formats = _fontFormatMap[fontName];
	}

	font = formats[fontSize];

	return font;
}
