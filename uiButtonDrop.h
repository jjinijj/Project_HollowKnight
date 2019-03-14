#pragma once
#include "uiButton.h"
class uiButtonDrop: public uiButton
{
private:

public:
	uiButtonDrop();
	~uiButtonDrop();

	HRESULT init(const char* upImgName
				 , const char* pressImgName
				 , float destX, float destY
				 , float width, float height) override;

	HRESULT init(const char* upImgName
				 , const char* pressImgName
				 , const char* downImgName
				 , float destX, float destY
				 , float width, float height) override;

	void release() override;
	void update() override;
	void render() override;

	void insertChild(uiObject* ui) override;

	void onceKeyDownMouseL() override;
	void onceKeyUpMouseL() override;
};

