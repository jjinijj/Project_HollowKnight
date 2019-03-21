#include "stdafx.h"
#include "loadingScene.h"
#include "dirtMouthScene.h"


loadingScene::loadingScene()
: _iaDataAllLoad(false)
{
}


loadingScene::~loadingScene()
{
}

HRESULT loadingScene::init()
{
	baseScene::init();
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
		//SCENEMANAGER->changeScene(eSceneName_DirtMouth);
		_iaDataAllLoad = false;
	}
}

void loadingScene::render()
{
}


void loadingScene::loadImageData()
{
	// 플레이어
	IMAGEMANAGER->addFrameImage("knight_idle", L"image/knight_idle.png", 568, 266, 4, 2);
	IMAGEMANAGER->addFrameImage("knight_attack_down", L"image/knight_attack_down.png", 852, 266, 6, 2);
	IMAGEMANAGER->addFrameImage("knight_attack_up", L"image/knight_attack_up.png", 852, 266, 6, 2);
	IMAGEMANAGER->addFrameImage("knight_attack1", L"image/knight_attack1.png", 710, 266, 5, 2);
	IMAGEMANAGER->addFrameImage("knight_attack2", L"image/knight_attack2.png", 710, 266, 5, 2);
	IMAGEMANAGER->addFrameImage("knight_attack3", L"image/knight_attack3.png", 1278, 266, 9, 2);
	IMAGEMANAGER->addFrameImage("knight_dead", L"image/knight_dead.png", 1136, 266, 8, 2);
	IMAGEMANAGER->addFrameImage("knight_jump", L"image/knight_jump.png", 1562, 266, 11, 2);
	IMAGEMANAGER->addFrameImage("knight_lookup", L"image/knight_lookup.png", 994, 266, 7, 2);
	IMAGEMANAGER->addFrameImage("knight_lookdown", L"image/knight_lookdown.png", 426, 266, 3, 2);
	IMAGEMANAGER->addFrameImage("knight_sitAnddrowse", L"image/knight_sitAnddrowse.png", 568, 266, 4, 2);
	IMAGEMANAGER->addFrameImage("knight_walk", L"image/knight_walk.png", 852, 266, 6, 2);

	// enemy tiktik
	IMAGEMANAGER->addFrameImage("tiktik_move", L"image/tiktik_move.png", 416, 190, 4, 2);
	IMAGEMANAGER->addFrameImage("tiktik_move_under", L"image/tiktik_move_under.png", 416, 190, 4, 2);
	IMAGEMANAGER->addFrameImage("tiktik_move_sideup", L"image/tiktik_move_sideup.png", 416, 190, 4, 2);
	IMAGEMANAGER->addFrameImage("tiktik_move_sidedown", L"image/tiktik_move_sidedown.png", 416, 190, 4, 2);
	IMAGEMANAGER->addFrameImage("tiktik_climbup_uts", L"image/tiktik_climbup_uts.png", 416, 190, 4, 2);
	IMAGEMANAGER->addFrameImage("tiktik_climbup_stu", L"image/tiktik_climbup_stu.png", 416, 190, 4, 2);
	IMAGEMANAGER->addFrameImage("tiktik_climbdown_stu", L"image/tiktik_climbdown_stu.png", 416, 190, 4, 2);
	IMAGEMANAGER->addFrameImage("tiktik_climbdown_ots", L"image/tiktik_climbdown_ots.png", 416, 190, 4, 2);
	IMAGEMANAGER->addFrameImage("tiktik_dead", L"image/tiktik_dead.png", 208, 190, 2, 2);

	// enemy gruzze
	IMAGEMANAGER->addFrameImage("gruzzer_move", L"image/gruzzer_move.png", 524, 218, 4, 2);
	IMAGEMANAGER->addFrameImage("gruzzer_dead", L"image/gruzzer_dead.png", 655, 218, 5, 2);

	// enemy pribalAspid
	IMAGEMANAGER->addFrameImage("primalAspid_move", L"image/primalAspid_move.png", 684, 272, 4, 2);
	IMAGEMANAGER->addFrameImage("primalAspid_attack", L"image/primalAspid_attack.png", 684, 272, 4, 2);
	IMAGEMANAGER->addFrameImage("primalAspid_dead", L"image/primalAspid_dead.png", 171, 272, 1, 2);

	// enemy bullet
	IMAGEMANAGER->addFrameImage("bullet_fire", L"image/bullet_fire.png", 270, 30, 7, 1);
	IMAGEMANAGER->addFrameImage("bullet_pang", L"image/bullet_pang.png", 203, 30, 3, 1);

	// enemy boos mawlek
	IMAGEMANAGER->addFrameImage("mawlek_body", L"image/mawlek_body.png", 3476, 1884, 11, 6);
	IMAGEMANAGER->addFrameImage("mawlek_head", L"image/mawlek_head.png", 417, 428, 3, 4);
	IMAGEMANAGER->addFrameImage("mawlek_arms", L"image/mawlek_arms.png", 872, 1683, 4, 6);

	// floor
	//IMAGEMANAGER->addImage("floor", L"image/floor.png", 326, 34);
	IMAGEMANAGER->addImage("floor_stone", L"image/floor_stone.png", 296, 21);

	// wall
	IMAGEMANAGER->addImage("wall_L", L"image/wall_L.png", 105, 169);
	IMAGEMANAGER->addImage("wall_R", L"image/wall_R.png", 105, 169);

	// block
	IMAGEMANAGER->addImage("block", L"image/block.png", 390, 73);

	// block big
	IMAGEMANAGER->addImage("block_big", L"image/block_big.png", 677, 209);

	// chair
	//IMAGEMANAGER->addImage("chair", L"image/chair.png", 185, 92);

	// gold Rock
	IMAGEMANAGER->addFrameImage("goldRock", L"image/goldRock.png", 749, 109, 7, 1);

	// coin
	IMAGEMANAGER->addFrameImage("coin", L"image/coin.png", 104, 28, 4, 1);

	// ui hp
	IMAGEMANAGER->addFrameImage("hp", L"image/hp.png", 560, 292, 7, 2);

	// ui coin
	IMAGEMANAGER->addImage("coin_ui", L"image/coin_ui.png", 59, 54);

	// ui skill gauage
	IMAGEMANAGER->addFrameImage("skillGauge_in", L"image/skillGauge_in.png", 480, 156, 2, 1);
	IMAGEMANAGER->addImage("skillGauge_out", L"image/skillGauge_out.png", 480, 156);

	// ui charmSlot
	IMAGEMANAGER->addImage("charmSlot", L"image/charmSlot.png", 30, 30);

	// player bullet
	IMAGEMANAGER->addFrameImage("player_bullet_fire_L", L"image/player_bullet_fire_L.png", 540, 202, 2, 1);
	IMAGEMANAGER->addFrameImage("player_bullet_pang_L", L"image/player_bullet_pang_L.png", 1080, 202, 4, 1);
	IMAGEMANAGER->addFrameImage("player_bullet_fire_R", L"image/player_bullet_fire_R.png", 540, 202, 2, 1);
	IMAGEMANAGER->addFrameImage("player_bullet_pang_R", L"image/player_bullet_pang_R.png", 1080, 202, 4, 1);

	// ui number
	IMAGEMANAGER->addFrameImage("number", L"image/number.png", 320, 49, 10, 1);

	//
	IMAGEMANAGER->addImage("cliff_L", L"image/cliff_L.png", 513, 372);
	IMAGEMANAGER->addImage("cliff_R", L"image/cliff_R.png", 513, 372);

	// npc
	IMAGEMANAGER->addFrameImage("npc", L"image/npc.png", 800, 800, 10, 10);
	IMAGEMANAGER->addFrameImage("cornifer", L"image/cornifer.png", 2219, 376, 7, 2);
	IMAGEMANAGER->addFrameImage("elderbug", L"image/elderbug.png", 440, 160, 4, 1);

	// item
	IMAGEMANAGER->addImage("charm1", L"image/charm1_.png", 50, 50);
	IMAGEMANAGER->addImage("charm2", L"image/charm2_.png", 50, 50);


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

	IMAGEMANAGER->addImage("bg", L"image/bg.png", 100, 100);
	IMAGEMANAGER->addImage("uiBG", L"image/uibg.png", 100, 100);
	IMAGEMANAGER->addImage("uiBG2", L"image/uibg2.png", 100, 100);
	IMAGEMANAGER->addImage("uiBG3", L"image/uibg3.png", 100, 100);
	IMAGEMANAGER->addImage("uiBG4", L"image/uibg4.png", 100, 100);
	IMAGEMANAGER->addImage("uiBG5", L"image/uibg5.png", 100, 100);



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

	IMGDATABASE->insertImage(eImage_UI_BG, IMAGEMANAGER->findImage("bg"));
	IMGDATABASE->insertImage(eImage_UI_BG1, IMAGEMANAGER->findImage("uiBG"));
	IMGDATABASE->insertImage(eImage_UI_BG2, IMAGEMANAGER->findImage("uiBG2"));
	IMGDATABASE->insertImage(eImage_UI_BG3, IMAGEMANAGER->findImage("uiBG3"));
	IMGDATABASE->insertImage(eImage_UI_BG4, IMAGEMANAGER->findImage("uiBG4"));
	IMGDATABASE->insertImage(eImage_UI_BG5, IMAGEMANAGER->findImage("uiBG5"));
	
	IMGDATABASE->insertImage(eImage_UI_Scroll_Back_Default, IMAGEMANAGER->findImage("uiBG4"));
	IMGDATABASE->insertImage(eImage_UI_Scroll_Front_Default, IMAGEMANAGER->findImage("uiBG2"));

	IMGDATABASE->insertImage(eImage_Npc_All, IMAGEMANAGER->findImage("npc"));
	IMGDATABASE->insertImage(eImage_Npc_Elderbug, IMAGEMANAGER->findImage("elderbug"));
	IMGDATABASE->insertImage(eImage_Npc_Cornifer, IMAGEMANAGER->findImage("cornifer"));
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
