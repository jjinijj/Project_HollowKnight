#pragma once
#include "uiObject.h"
class exitPopup: public uiBase
{
private:
	bool _isExit;
	
	image* _background;
	image* _question;
	image* _menu[2];
	image* _cursor;
	animation* _cursorAni;

public:
	exitPopup();
	~exitPopup();

	HRESULT init();
	void release();
	void update();
	void render();

	void uiOpen();
	void uiClose();
};

