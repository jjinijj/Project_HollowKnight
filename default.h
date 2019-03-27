#pragma once


//=============================================
//   define type
//=============================================
#define MAPSIZE			UINT
#define UID				UINT
#define DEGUGMODE		UINT

#define RECTD2D			D2D_RECT_F


//=============================================
//   define value
//=============================================

#define MAPSIZE_MAX_X 19200
#define MAPSIZE_MAX_Y 10800

#define BGCOLORCODE 0x465d8f

#define DEFAULT_FONT_SIZE 15
#define DEFAULT_FONT_COLOR RGB(0, 0, 0)

#define UI_SPACE 10.f

#define MAP_NAME_DIRTMOUTH		"map_dirtmouth"
#define MAP_NAME_CROSSLOAD_1	"map_crossload_01"
#define MAP_NAME_CROSSLOAD_2	"map_crossload_02"
#define MAP_NAME_CROSSLOAD_3	"map_crossload_03"

#define PLAYER_UID					9999
#define LOADING_UID					10000
#define MENU_CURSOR_UID				10001

//=============================================
//   enum
//=============================================
enum eAttribute
{
	eAttr_Collide,		// 충돌
	eAttr_Trigger,		// 트리거
	eAttr_Breakable,	// 파괴 가능
	eAttr_Usable,		// 상호작용
	eAttr_Trap,			// 함정
	eAttr_Portal,		// 씬 이동
	eAttr_Dialog,		// 대화 가능
	eAttr_Chair,		// 의자 속성
	eAttr_Player_Pos,	// 플레이어 시작 위치

	eAttr_None,
	eAttr_Count = eAttr_None,
};

// 지형 종류
enum eTerrainType
{
	eTerrain_Frame,
	eTerrain_Drag,
	eTerrain_Clear,

	eTarrain_None,
	eTerrain_Count = eTarrain_None,
};

// 씬 종류
enum eSceneName
{
	eSceneName_Loading,
	eSceneName_MapTool,
	eSceneName_Title,
	eSceneName_Load,
	eSceneName_DirtMouth,
	eSceneName_CrossLoad_01,
	eSceneName_Iselda_Store,

	eSceneName_Test,

	eSceneName_None,
	eSceneName_Count = eSceneName_None
};

// 적
enum eEnemy_Type
{
	eEnemy_Gruzzer,
	eEnemy_Tiktik,
	eEnemy_Primalaspid,
	eEnemy_Mawlek,

	eEnemy_None,
	eEnemy_Count = eEnemy_None,
};

// npc
enum eNpc_Type
{
	eNpc_Elderbug,
	eNpc_Sly,
	eNpc_Quirrel,
	eNpc_Iselda,
	eNpc_Cornifer,
	eNpc_TheLastStag,
	
	eNpc_None,
	eNpc_Count = eNpc_None,
};


// 레이어
enum eLayer
{
	eLayer_FarBack,
	eLayer_NearBack,
	eLayer_Play,
	eLayer_NearFront,
	eLayer_CloseFront,
	eLayer_Actor,

	eLayer_None,
	eLayer_Count = eLayer_None,
};


//=============================================
//   debug mode
//=============================================
#define DEBUG_SHOW_RECT		0x000001UL // RECT 보여주기
#define DEBUG_SHOW_TEXT		0x000002UL // TEXT 보여주기
#define DEBUG_SHOW_UID		0x000004UL // UID
#define DEBUG_SHOW_COLLISON 0x000008UL // Collision
#define DEBUG_SHOW_POSITION 0x000010UL // position
#define DEBUG_HIDE_IMAGE	0x000020UL // 
//#define ATTR_DEBUG_SHOW_CLEARTERRAIN	0x000004UL // 
//#define ATTR_DEBUG_SHOW_CLEARTERRAIN	0x000008UL // 
//#define ATTR_DEBUG_SHOW_CLEARTERRAIN	0x000010UL // 
