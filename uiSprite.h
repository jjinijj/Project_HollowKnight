#pragma once
#include "uiObject.h"

class uiSprite : public uiObject
{
private:
	UINT		_aniUID;
	WORD		_aniKey;
	animation*	_ani;
	image*		_img;

public:
	uiSprite();
	~uiSprite();

	HRESULT init(float x, float y, const char* imgName);
	void release();
	void update();
	void render();

	void setAnimationUid(UINT uid) { _aniUID = uid; }
	void setAnimation(WORD aniKey);

	WORD getAniKey() {return _aniKey;}
};

