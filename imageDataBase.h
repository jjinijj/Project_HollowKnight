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

	eImage_Npc_All = 1500,
	eImage_Npc_Elderbug,
	eImage_Npc_Sly,
	eImage_Npc_Quirrel,
	eImage_Npc_Iselda,
	eImage_Npc_Cornifer,
	eImage_Npc_TheLastStag,

	eImage_Enemy_All = 2000,
	eImage_Enemy_Gruzzer,
	eImage_Enemy_TikTik,

	eImage_Enemy_StartId = eImage_Enemy_Gruzzer,
	eImage_Enemy_EndId = eImage_Enemy_TikTik,

	eImage_UI_BG = 10000,
	eImage_UI_BG1,
	eImage_UI_BG2,
	eImage_UI_BG3,
	eImage_UI_BG4,
	eImage_UI_BG5,
	eImage_UI_BG6,

	eImage_UI_Scroll_Back_Default = 10100,
	eImage_UI_Scroll_Front_Default,

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

