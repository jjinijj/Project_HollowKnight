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
class uiPanel;
class uiImage;
class uiList;
class terrain;

// 지형 종류
enum eTerrainType
{
	eTerrain_Frame,
	eTerrain_Drag,
	eTerrain_Clear,

	eTarrain_None,
	eTerrain_Count = eTarrain_None,
};

// 레이어
enum eLayer
{
	eLayer_FarBack,
	eLayer_NearBack,
	eLayer_Play,
	eLayer_NearFront,
	eLayer_CloseFront,

	eLayer_None,
	eLayer_Count = eLayer_None,
};

enum eToolMode
{
	eToolMode_DrawTerrain,
	eToolMode_DrawCollider,
	eToolMode_DrawObject,
	eToolMode_Inspector,

	eToolMode_None,
	eToolMode_Count = eToolMode_None,
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

typedef struct tagTerrainInfo
{
	terrain*	ter;
	eLayer		layer;
	int			idx;
	bool		isSet;

	void infoSet(terrain* inTer, eLayer inLayer, int inIdx)
	{
		ter = inTer;
		layer = inLayer;
		idx = inIdx;
		isSet = true;
	}
	void clear()
	{
		ter = nullptr;
		layer = eLayer_None;
		idx -1;
		isSet = false;
	}
	void operator= (tagTerrainInfo info)
	{
		ter = info.ter;
		layer = info.layer;
		idx = info.idx;
		isSet = info.isSet;
	}
}TERRAIN;

class mapTool : public uiBase
{
private:

	eToolMode _mode;
	//toolState* _state;
	TERRAIN _terrain;
	uiButton* _curBtnTerrain;

	int _sampleIdx;

	image* _uiBG[5];

	mapData* _mapData;					// 맵 데이터
	vector<IMGLNK*> _imgLnks;			// 맵툴에서 사용할 이미지 번호들
	
	eLayer _curLayer;

	image* _sampleImg;					// 샘플 지형들
	SAMPLE _pick;						// 선택한 지형
	eTerrainType _terType;				// 지형 종류

	
	bool _isPicking;
	bool _isOpenSampleBoard;
	bool _isCloseSampleBoard;

	POINTF _pickMousePos;
	
	uiPanel* _samplePanel;			// 샘플 이미지가 ui
	uiPanel* _samplecanvas;			// 샘플 이미지가 그려지는 부분
	uiImage* _sampleImage;

	uiPanel* _canvas;				// 실제 맵이 그려지는 부분

	uiPanel* _miniMap;				// 미니맵 

	uiButton* _qickOpen;
	uiButton* _beforeSample;
	uiButton* _nextSample;
	uiButton* _createCol;

	// hierarcy
	uiButton* _uiBtnHierarcy[eLayer_Count];
	uiList* _uiListHierarcy[eLayer_Count];

	// 상태창(옵션)
	uiPanel* _uiPanelInspector;
	uiButton* _uiBtnInspectors[eAttr_Count];

	// 하위 상태창
	uiPanel* _uiPanelInspectorSub;
	uiButton* _uiBtnInstpectorSubs[10];

	



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
	void settingSampleImageLinks();
	void setSampleImage();
	void initUI();

	void updateDrawTerrain();
	void updateDrawCollider();

	void renderDrawTerrain();
	void renderDrawCollider();

	void openSampleCanvas();
	void openingSampleCanvas();
	void closingSampleCanvas();
	void closeSampleCanvas();

	void clickBtnHierarcy(eLayer layer);
	void clickUpBtnHierarcy(eLayer layer);

	void clickBtnTerrain(int idx, uiButton* btn);
	void clickUpBtnTerrain(int idx);

	void clickBtnInspector(UINT attr, uiButton* btn);
	void clickUpBtnInspector(UINT attr);
};
