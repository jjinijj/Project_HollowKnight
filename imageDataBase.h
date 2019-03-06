#pragma once
#include "singletonBase.h"

enum eImageUID
{
	eImage_Town_Floor = 1,
	eImage_Town_BG,
	eImage_Town_BG_Big01,
	eImage_Town_BG_Big02,
	eImage_Town_Build,
	eImage_Town_Ruddle,

	eImage_Town_Build01 = 50,
	eImage_Town_Build02,
	eImage_Town_Build03,
	eImage_Town_Build04,
	eImage_Town_Build05,
	eImage_Town_Build06,
	eImage_Town_Build07,

	eImage_Cross_ = 100,


	eImage_Object_Chair = 1000,
	eImage_Fence,
	eImage_Town_Well,
	eImage_StreetLamp,



	eImage_None,
};

class imageDataBase : public singletonBase<imageDataBase>
{
private:
	typedef map<eImageUID, image*>				mImage;
	typedef map<eImageUID, image*>::iterator	iterMImage;


private:
	mImage _imgMap;


public:
	imageDataBase();
	~imageDataBase();

	HRESULT init();
	void release();

	void insertImage(eImageUID uid, image* img);
	image* getImage(eImageUID uid);
};

