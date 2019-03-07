#pragma once


#define DISTANCE 10.f

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

class mapData;
class terrain;
class mapTool
{
private:

	eToolMode _mode;

	int _sampleIdx;

	mapData* _mapData;					// 맵 데이터
	vector<IMGLNK*> _imgLnks;			// 맵툴에서 사용할 이미지 번호들
	
	eLayer _curLayer;

	image* _sampleImg;					// 샘플 지형들
	SAMPLE _pick;						// 선택한 지형
	eTerrainType _terType;				// 지형 종류

	
	bool _isPicking;
	POINTF _pickMousePos;
	
	RECTD2D* _canvers;					// 실제 맵이 그려지는 부분
	RECTD2D  _canversSample;			// 샘플 이미지가 그려지는 부분
	
	RECTD2D* _miniMap;					// 미니맵 
	RECTD2D* _sampleBoard;				// 샘플 이미지가 ui

	RECTD2D _pickArea;					// 드래그
	RECTD2D _mapViewArea;				// 미니맵에서 현재 보여주는 맵 표시

public:
	mapTool();
	~mapTool();

	HRESULT init();
	void release();
	void update();
	void render();
	void terrainRender();
	void terrainRender(float destX, float destY, float percent);

	// 캔버스 위치 지정
	void setSampleBoardRect(RECTD2D* rc);
	
	void setCanversRect(RECTD2D* rc)		{ _canvers = rc; }
	void setMiniMapRect(RECTD2D* rc)		{ _miniMap = rc;}

	// 샘플 선택
	void pickSample();
	// 캔버스 그리기
	void pickCanvers();


	void nextSample();
	void beforeSample();
	
	// 모드
	void setToolMode(eToolMode mode);

	image* getSample() { return _sampleImg; }
	eToolMode getToolMode() { return _mode; }

	bool isSamplePicking() {return _isPicking;}

private:
	void setSampleImage();

	void updateDrawTerrain();
	void updateDrawCollider();

	void renderDrawTerrain();
	void renderDrawCollider();
};

