#pragma once


//=============================================
//   define type
//=============================================
#define TERRAINATTR		UINT
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
#define ATTR_COLLIDER	0x000001UL		// 충돌
#define ATTR_TRIGGER	0x000002UL		// 트리거
#define ATTR_BREAKABLE	0x000004UL		// 파괴 가능
#define ATTR_INTERATION	0x000008UL		// 상호작용
#define ATTR_TRAP		0x000010UL		// 함정


//=============================================
//   define enum
//=============================================
#define DEBUG_SHOW_RECT		0x000001UL // RECT 보여주기
#define DEBUG_SHOW_TEXT		0x000002UL // TEXT 보여주기
//#define ATTR_DEBUG_SHOW_CLEARTERRAIN	0x000004UL // 
//#define ATTR_DEBUG_SHOW_CLEARTERRAIN	0x000008UL // 
//#define ATTR_DEBUG_SHOW_CLEARTERRAIN	0x000010UL // 
