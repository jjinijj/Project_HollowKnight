#pragma once
#include "uiObject.h"
class uiText: public uiObject
{
private:
	
	float _x;
	float _y;
	
	wstring					_text;
	UINT					_fontSize;
	COLORREF					_fontColor;
	d2dManager::eFontType	_fontType;

public:
	uiText();
	~uiText();

	HRESULT init(float x, float y);
	void release()	override;
	void update()	override;
	void render()	override;

	void setText(wstring text)					{ _text = text;			}
	void setFontSize(UINT size)					{ _fontSize = size;		}
	void setFontColor(COLORREF color)				{ _fontColor = color;	}
	void setFontType(d2dManager::eFontType type){ _fontType = type;		}

	wstring getText()					{ return _text;		}
	UINT getFontSize()					{ return _fontSize; }
	COLORREF getFontColor()				{ return _fontColor;}
	d2dManager::eFontType getFontType() { return _fontType;	}

};

