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

typedef struct tagSampleTerrain
{
	float x;
	float y;

	float frameX;
	float framxY;

	float width;
	float height;

	eImageUID uid;

	bool isPick;

	void clear()
	{
		x = 0.f;
		y = 0.f;
		width = 0.f;
		height = 0.f;
		uid = eImage_None;
		isPick = false;
	}

}SAMPLE;

class mapData;
class terrain;
class mapTool
{
private:

	int _sampleIdx;

	mapData* _mapData;					// 맵 데이터
	vector<eImageUID> _imgUids;			// 맵툴에서 사용할 이미지 번호들
	
	eLayer _curLayer;

	image* _samples;					// 샘플 지형들
	SAMPLE _pick;						// 선택한 지형
	eTerrainType _terType;				// 지형 종류

	bool _isPicking;
	POINTF _pickMousePos;
	
	RECTD2D* _canvers;					// 실제 맵이 그려지는 부분
	RECTD2D* _canversSample;			// 샘플 이미지가 그려지는 부분
	RECTD2D* _miniMap;

	RECTD2D _pickArea;
	RECTD2D _mapViewArea;

public:
	mapTool();
	~mapTool();

	HRESULT init();
	void release();
	void update();
	void render();
	void terrainRender();
	void terrainRender(float destX, float destY, float percent);

	// 캔버스 지정
	void setCanversRect(RECTD2D* rc)		{ _canvers = rc; }
	void setCanversSampleRect(RECTD2D* rc)	{ _canversSample = rc; }
	void setMiniMapRect(RECTD2D* rc)		{ _miniMap = rc;}

	// 샘플 선택
	void pickSample();
	// 캔버스 그리기
	void pickCanvers();


	void nextSample();
	void beforeSample();
	image* getSample() { return _samples; }

	bool isSamplePicking() {return _isPicking;}
};

