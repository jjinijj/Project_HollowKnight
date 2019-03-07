#include "stdafx.h"
#include "loadingScene.h"


loadingScene::loadingScene()
: _iaDataAllLoad(false)
{
}


loadingScene::~loadingScene()
{
}

HRESULT loadingScene::init()
{
	_sceneName = eSceneName_Loading;

	_iaDataAllLoad = false;
	loadData();

	return S_OK;
}

void loadingScene::release()
{
}

void loadingScene::update()
{
	if (_iaDataAllLoad)
	{
		//SCENEMANAGER->changeScene(eSceneName_Title);
		SCENEMANAGER->changeScene(eSceneName_MapTool);
		_iaDataAllLoad = false;
	}
}

void loadingScene::render()
{
}


void loadingScene::loadImageData()
{
	IMAGEMANAGER->addFrameImage("chair", L"image/chair.png", 366, 90, 2, 1);
	IMAGEMANAGER->addFrameImage("fence", L"image/fence.png", 960, 800, 1, 1);
	IMAGEMANAGER->addFrameImage("floor", L"image/floor.png", 800, 800, 1, 1);
	IMAGEMANAGER->addFrameImage("streetLamp", L"image/streetLamp.png", 960, 480, 1, 1);
	IMAGEMANAGER->addFrameImage("town_bg", L"image/town_bg.png", 960, 960, 1, 1);
	IMAGEMANAGER->addFrameImage("town_bg_big_01", L"image/town_bg_big_01.png", 960, 960, 1, 1);
	IMAGEMANAGER->addFrameImage("town_bg_big_02", L"image/town_bg_big_02.png", 960, 960, 1, 1);
	IMAGEMANAGER->addFrameImage("town_build", L"image/town_build.png", 960, 384, 5, 2);
	IMAGEMANAGER->addFrameImage("town_build_01", L"image/town_build_01.png", 512, 394, 1, 1);
	IMAGEMANAGER->addFrameImage("town_build_02", L"image/town_build_02.png", 508, 396, 1, 1);
	IMAGEMANAGER->addFrameImage("town_build_03", L"image/town_build_03.png", 512, 473, 1, 1);
	IMAGEMANAGER->addFrameImage("town_build_04", L"image/town_build_04.png", 852, 595, 1, 1);
	IMAGEMANAGER->addFrameImage("town_build_05", L"image/town_build_05.png", 540, 373, 1, 1);
	IMAGEMANAGER->addFrameImage("town_build_06", L"image/town_build_06.png", 364, 444, 1, 1);
	IMAGEMANAGER->addFrameImage("town_build_07", L"image/town_build_07.png", 410, 539, 1, 1);
	IMAGEMANAGER->addFrameImage("town_rubble", L"image/town_rubble.png", 960, 960, 1, 1);
	IMAGEMANAGER->addFrameImage("well", L"image/well.png", 800, 800, 1, 1);

	IMAGEMANAGER->addImage("uiBG", L"image/uibg.png", 100, 100);
	IMAGEMANAGER->addImage("uiBG2", L"image/uibg2.png", 100, 100);
	IMAGEMANAGER->addImage("uiBG3", L"image/uibg3.png", 100, 100);
	IMAGEMANAGER->addImage("uiBG4", L"image/uibg4.png", 100, 100);
	IMAGEMANAGER->addImage("uiBG5", L"image/uibg5.png", 100, 100);

	// chair, 366, 90, 2, 1
	// fence, 960, 800
	// floor, 800, 800
	// streetLamp, 800, 480
	// town_bg, 960, 960
	// town_bg_big_01, 960, 960
	// town_bg_big_02, 960, 960
	// town_build, 960, 384, 5, 2
	// town_build_01, 512, 394,
	// town_build_02, 508, 396,
	// town_build_03, 512, 473,
	// town_build_04, 852, 595,
	// town_build_05, 540, 373,
	// town_build_06, 364, 444
	// town_build_07, 410, 539,
	// town_rubble, 960, 960,
	// well, 800, 800

	IMGDATABASE->insertImage(eImage_Town_Floor, IMAGEMANAGER->findImage("floor"));
	IMGDATABASE->insertImage(eImage_Town_BG, IMAGEMANAGER->findImage("town_bg"));
	IMGDATABASE->insertImage(eImage_Town_BG_Big01, IMAGEMANAGER->findImage("town_bg_big_01"));
	IMGDATABASE->insertImage(eImage_Town_BG_Big02, IMAGEMANAGER->findImage("town_bg_big_02"));
	IMGDATABASE->insertImage(eImage_Town_Build, IMAGEMANAGER->findImage("town_build"));
	IMGDATABASE->insertImage(eImage_Town_Ruddle, IMAGEMANAGER->findImage("town_rubble"));

	IMGDATABASE->insertImage(eImage_Town_Build01, IMAGEMANAGER->findImage("town_build_01"));
	IMGDATABASE->insertImage(eImage_Town_Build02, IMAGEMANAGER->findImage("town_build_02"));
	IMGDATABASE->insertImage(eImage_Town_Build03, IMAGEMANAGER->findImage("town_build_03"));
	IMGDATABASE->insertImage(eImage_Town_Build04, IMAGEMANAGER->findImage("town_build_04"));
	IMGDATABASE->insertImage(eImage_Town_Build05, IMAGEMANAGER->findImage("town_build_05"));
	IMGDATABASE->insertImage(eImage_Town_Build06, IMAGEMANAGER->findImage("town_build_06"));
	IMGDATABASE->insertImage(eImage_Town_Build07, IMAGEMANAGER->findImage("town_build_07"));

	IMGDATABASE->insertImage(eImage_Object_Chair, IMAGEMANAGER->findImage("chair"));
	IMGDATABASE->insertImage(eImage_Fence, IMAGEMANAGER->findImage("fence"));
	IMGDATABASE->insertImage(eImage_Town_Well, IMAGEMANAGER->findImage("well"));
	IMGDATABASE->insertImage(eImage_StreetLamp, IMAGEMANAGER->findImage("streetLamp"));
}

void loadingScene::loadSoundData()
{
}

void loadingScene::loadData()
{
	loadImageData();
	loadSoundData();

	_iaDataAllLoad = true;

}
