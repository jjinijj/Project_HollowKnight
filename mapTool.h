#pragma once


#define DISTANCE 10.f

// ���� ����
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

	mapData* _mapData;					// �� ������
	vector<eImageUID> _imgUids;			// �������� ����� �̹��� ��ȣ��
	
	eLayer _curLayer;

	image* _samples;					// ���� ������
	SAMPLE _pick;						// ������ ����
	eTerrainType _terType;				// ���� ����

	bool _isPicking;
	POINTF _pickMousePos;
	
	RECTD2D* _canvers;					// ���� ���� �׷����� �κ�
	RECTD2D* _canversSample;			// ���� �̹����� �׷����� �κ�
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

	// ĵ���� ����
	void setCanversRect(RECTD2D* rc)		{ _canvers = rc; }
	void setCanversSampleRect(RECTD2D* rc)	{ _canversSample = rc; }
	void setMiniMapRect(RECTD2D* rc)		{ _miniMap = rc;}

	// ���� ����
	void pickSample();
	// ĵ���� �׸���
	void pickCanvers();


	void nextSample();
	void beforeSample();
	image* getSample() { return _samples; }

	bool isSamplePicking() {return _isPicking;}
};

