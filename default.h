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

#define MAP_LAYER_COUNT 5

#define DEFAULT_FONT_SIZE 15
#define DEFAULT_FONT_COLOR RGB(0, 0, 0)

#define UI_SPACE 10.f

//=============================================
//   지형 속성
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

//=============================================
//   define enum
//=============================================
#define DEBUG_SHOW_RECT		0x000001UL // RECT 보여주기
#define DEBUG_SHOW_TEXT		0x000002UL // TEXT 보여주기
//#define ATTR_DEBUG_SHOW_CLEARTERRAIN	0x000004UL // 
//#define ATTR_DEBUG_SHOW_CLEARTERRAIN	0x000008UL // 
//#define ATTR_DEBUG_SHOW_CLEARTERRAIN	0x000010UL // 
