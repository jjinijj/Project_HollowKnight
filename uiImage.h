#pragma once
#include "uiObject.h"
class uiImage : public uiObject
{
private:
	image* _img;

public:
	uiImage();
	~uiImage();

	HRESULT init(float x, float y, image* img);
	void release();
	void update();
	void render();
};

