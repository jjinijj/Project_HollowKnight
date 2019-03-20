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

#define MAP_NAME_DIRTMOUTH		"map_dirtmouth"
#define MAP_NAME_CROSSLOAD_1	"map_crossload_01"
#define MAP_NAME_CROSSLOAD_2	"map_crossload_02"
#define MAP_NAME_CROSSLOAD_3	"map_crossload_03"

#define PLAYER_UID				9999

//=============================================
//   ���� �Ӽ�
//=============================================
enum eAttribute
{
	eAttr_Collide,		// �浹
	eAttr_Trigger,		// Ʈ����
	eAttr_Breakable,	// �ı� ����
	eAttr_Usable,		// ��ȣ�ۿ�
	eAttr_Trap,			// ����
	eAttr_Portal,		// �� �̵�
	eAttr_Dialog,		// ��ȭ ����

	eAttr_None,
	eAttr_Count = eAttr_None,
};

// ���� ����
enum eTerrainType
{
	eTerrain_Frame,
	eTerrain_Drag,
	eTerrain_Clear,

	eTarrain_None,
	eTerrain_Count = eTarrain_None,
};

// �� ����
enum eSceneName
{
	eSceneName_Loading,
	eSceneName_MapTool,
	eSceneName_Title,
	eSceneName_Load,
	eSceneName_DirtMouth,
	eSceneName_CrossLoad_01,

	eSceneName_None,
	eSceneName_Count = eSceneName_None
};

//=============================================
//   debug mode
//=============================================
#define DEBUG_SHOW_RECT		0x000001UL // RECT �����ֱ�
#define DEBUG_SHOW_TEXT		0x000002UL // TEXT �����ֱ�
#define DEBUG_SHOW_UID		0x000004UL // UID
#define DEBUG_SHOW_COLLISON 0x000008UL // Collision
#define DEBUG_SHOW_POSITION 0x000010UL // position
#define DEBUG_HIDE_IMAGE	0x000020UL // 
//#define ATTR_DEBUG_SHOW_CLEARTERRAIN	0x000004UL // 
//#define ATTR_DEBUG_SHOW_CLEARTERRAIN	0x000008UL // 
//#define ATTR_DEBUG_SHOW_CLEARTERRAIN	0x000010UL // 
