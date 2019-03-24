#pragma once
#include "toolState.h"

#define SAMPLABOARD_WIDTH 1010.f

#define DISTANCE 10.f
#define MINIMAP_PERCENT 0.019f				// 미니맵 배율

#define TERRAIN_COUNT_PER_LINE 5
#define TERRAIN_LIST_CELL_GAP 2.f
#define TERRAIN_LIST_HEIGHT 700.f
#define TERRAIN_LIST_SCROLL_WIDTH 10.f

class mapData;
class terrain;
class uiButton;
class uiButtonDrop;
class uiPanel;
class uiImage;
class uiList;
class uiText;
class terrain;
class gameObject;

enum eToolMode
{
	eToolMode_DrawTerrain,
	eToolMode_DrawCollider,
	eToolMode_DrawObject,
	eToolMode_DrawNpc,
	eToolMode_DrawEnemy,

	eToolMode_None,
	eToolMode_Count = eToolMode_None,
};

enum eViewMode
{
	eViewMode_Layer,
	eViewMode_Collision,
	eViewMode_Rect,
	eViewMode_InfoText,
	eViewMode_HideImage,

	eViewMode_None,
	eViewMode_Count = eViewMode_None,
};

typedef struct tagSampleTerrain
{
	float x;
	float y;

	int frameX;
	int frameY;

	float width;
	float height;

	eImageUID uid;

	bool isPick;
	bool isFrame;

	void clear()
	{
		x = 0.f;
		y = 0.f;
		width = 0.f;
		height = 0.f;
		uid = eImage_None;
		isPick = false;
		isFrame = false;
	}

}SAMPLE;

// 이미지 링크. 샘플 이미지와 실제 출력될 이미지가 다를 경우를 고려
typedef struct tagImageLink
{
	eImageUID mainUID;

	bool isFrameImg;
	int lnkSize;

	vector<eImageUID> lnkUIDs;

	void makeImageLnk(eImageUID mainUid, bool isFrame)
	{
		mainUID = mainUid;
		isFrameImg = isFrame;
		lnkSize = 0;
		lnkUIDs.clear();
	}

	void pushBack(eImageUID uid)
	{
		lnkUIDs.push_back(uid);
		++lnkSize;
	}

	void operator= (tagImageLink lnk)
	{
		makeImageLnk(lnk.mainUID, lnk.isFrameImg);
		if (0 < lnk.lnkSize && 0 < lnk.lnkUIDs.size())
		{
			for (int ii = 0; ii < lnk.lnkSize; ++ii)
			{
				pushBack(lnk.lnkUIDs[ii]);
			}
		}
	}

}IMGLNK;

typedef struct tagSelectInfo
{
	gameObject* obj;
	eLayer		layer;
	WORD		attr;
	bool		isSet;
	bool		isActor;


	void infoSet(gameObject* inObj, eLayer inLayer)
	{
		clear();
		obj = inObj;
		layer = inLayer;
		isSet = true;
		isActor = true;
	}
	void infoSet(gameObject* inObj, WORD inattr, eLayer inLayer)
	{
		clear();
		obj = inObj;
		attr = inattr;
		layer = inLayer;
		isSet = true;
	}
	void clear()
	{
		obj = nullptr;
		attr = NULL;
		layer = eLayer_None;
		isSet = false;
		isActor = false;
	}
	void operator= (tagSelectInfo info)
	{
		obj = info.obj;
		attr = info.attr;
		layer = info.layer;
		isSet = info.isSet;
		isActor = info.isActor;
	}
}SELECT;

class mapTool : public uiBase
{
private:

	eToolMode _mode;
	eToolMode _beforeMode;
	//toolState* _state;
	SELECT _select;
	uiButton* _curBtnSelect;
	uiButton* _selBtnFileName;

	int _sampleIdx;
	int _moveSnap;		// 이동 간격

	mapData* _mapData;						// 맵 데이터
	vector<IMGLNK*> _imgLnksTerrain;		// 맵툴에서 사용할 이미지 번호들
	vector<IMGLNK*> _imgLnksObject;			// 맵툴에서 사용할 이미지 번호들
	vector<IMGLNK*> _imgLnksNpc;			// 맵툴에서 사용할 이미지 번호들
	vector<IMGLNK*> _imgLnksEnemy;			// 맵툴에서 사용할 이미지 번호들
	
	eLayer _curLayer;

	image* _sampleImg;					// 샘플 지형들
	SAMPLE _pick;						// 선택한 지형
	eSceneName _curFileName;			// 현재 파일
	eSceneName _selFileName;			// 선택한 파일
	
	bool _isPicking;
	bool _isOpenSampleBoard;
	bool _isCloseSampleBoard;
	bool _isShowAllLayer;			// 모든 레이어 렌더
	bool _isReposition;				// 위치 다시

	POINTF _pickMousePos;
	POINTF _originPos;				// 움직이기 전 위치
	
	uiPanel* _samplePanel;			// 샘플 이미지가 ui
	uiPanel* _samplecanvas;			// 샘플 이미지가 그려지는 부분
	uiImage* _sampleImage;

	uiPanel* _canvas;				// 실제 맵이 그려지는 부분

	uiPanel* _miniMap;				// 미니맵 

	uiButton* _qickOpen;

	uiButton* _uiBtnDrawCollision;	// 이미지 없는 충돌체 생성
	uiButton* _uiBtnDrawTerrain;	// 지형
	uiButton* _uiBtnDrawObject;		// 오브젝트(프레임 이미지)
	uiButton* _uiBtnDrawNpc;		// npc
	uiButton* _uiBtnDrawEnemy;		// enemy

	uiButton* _uiBtnDelTerrain;		// 지형 삭제
	uiButton* _uiBtnUpIndex;		// 위로 올리기(렌더를 먼저)
	uiButton* _uiBtnDownIndex;		// 아래로 (렌더를 나중에)
	
	uiButtonDrop* _uiBtnDropChangeLayer;	// 레이어 변경

	uiButton* _uiBtnViewMode[eViewMode_Count];	// View Mode
	
	uiButton* _uiBtnMoveUp;				// 위로 이동
	uiButton* _uiBtnMoveDown;			// 아래로 이동
	uiButton* _uiBtnMoveLeft;			// 좌
	uiButton* _uiBtnMoveRight;			// 우
	uiButton* _uiBtnMoveLeftUp;			// 좌상
	uiButton* _uiBtnMoveRightUp;		// 우상
	uiButton* _uiBtnMoveLeftDown;		// 좌하
	uiButton* _uiBtnMoveRightDown;		// 우하

	uiButton* _uiBtnMoveReset;			// 위치 다시 

	uiButton* _uiBtnMoveSnapPlus;		// 이동 간격 늘리기
	uiButton* _uiBtnMoveSnapMinus;		//			줄이기

	uiText*	_uiTextMoveSnap;			// 이동 간격 표시

	uiText* _uiButtonReverse;			// 지형 반전

	uiText* _uiTextCurFileName;			// 현재 파일 이름			
	uiList* _uiListFiles;				// 파일 이름들
	uiButton* _uiBtnSave;				// 저장
	uiButton* _uiBtnLoad;				// 로드;

	// hierarcy
	uiButton* _uiBtnHierarcy[eLayer_Count + 1];
	uiList* _uiListHierarcy[eLayer_Count + 1];

	// 상태창(옵션)
	uiText* _uiTextInspector;
	uiPanel* _uiPanelInspector;
	uiButton* _uiBtnInspectors[eAttr_Count];

	// 상태창 상세
	uiText* _uiTextInspectorSub;
	uiPanel* _uiPanelInspectorInfo;
	uiText* _uiTextInspectorSubInfo;

	// 레이어 변경
	uiPanel* _uiPanelPopUpLayerChange;



	RECTD2D _miniScope;
	RECTD2D _pickArea;					// 드래그
	RECTD2D _mapViewArea;				// 미니맵에서 현재 보여주는 맵 표시

	RECTD2D _samplePanelRc;
	
	float _miniScopeWidth;
	float _miniScopeHeight;

	const float _sampleBoardOpenX;
	const float _sampleBoardCloseX;

	const float _sampleBoardSpeed;

public:
	mapTool();
	~mapTool();

	HRESULT init();
	void release();
	void clear();
	void update();
	void render();

	// 샘플 선택
	void pickSampleStart();
	void pickSampleEnd();
	// 캔버스 그리기
	void pickcanvasStart();
	void pickcanvasEnd();
	// 드래그
	void picking();

	void pickingEnd();

	void clickingMinimap();
	
	void nextSample();
	void beforeSample();
	
	// 모드
	void setToolMode(eToolMode mode);

	image* getSample() { return _sampleImg; }
	eToolMode getToolMode() { return _mode; }

	bool isPicking() {return _isPicking;}

private:
	void setSampleImage();
	void initUI();
	void initTerrainImgLinks();
	void initObjectImgLinks();
	void initNpcImgLinks();
	void initEnemyImgLinks();

	void updateDrawTerrain();
	void updateDrawCollider();

	void renderDrawTerrain();
	void renderDrawCollider();
	void renderDrawObject();
	void renderDrawNpc();
	void renderDrawEnemy();

	void openSampleCanvas();
	void openingSampleCanvas();
	void closingSampleCanvas();
	void closeSampleCanvas();

	void clickBtnHierarcy(eLayer layer);
	void clickUpBtnHierarcy(eLayer layer);

	void clickBtnTerrain(UID uid, uiButton* btn);
	void clickUpBtnTerrain(UID uid);

	void clickBtnActor(UID uid, uiButton* btn);
	void clickUpBtnActor(UID uid);

	void clickBtnInspector(eAttribute attr, uiButton* btn);
	void clickUpBtnInspector(eAttribute attr);

	void clickBtnUpNone(uiButton* btn);

	void clickBtnDelTerrain();
	void clickBtnUpIndex();
	void clickBtnDownIndex();
	void clickBtnChangeLayer(UINT layer);

	void clickBtnViewMode(eViewMode mode);
	void clickBtnUpViewMode(eViewMode mode);

	void clickBtnMoveReset();
	void clickBtnMoveUp();
	void clickBtnMoveDown();
	void clickBtnMoveLeft();
	void clickBtnMoveRight();

	void clickBtnMoveLeftUp();
	void clickBtnMoveLeftDown();
	void clickBtnMoveRightUp();
	void clickBtnMoveRightDown();

	void clickBtnMoveSnapPlus();
	void clickBtnMoveSnapMinus();

	void refreshDetailText();

	bool checkSelectingTerrain();
	void startMoveTerrain();

	void clickBtnSelectFile(eSceneName sceneName, uiButton* btn);

	void clickBtnSaveMap();
	void clickBtnLoadMap();
};
