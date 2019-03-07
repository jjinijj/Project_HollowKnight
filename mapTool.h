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

// ���̾�
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

// �̹��� ��ũ. ���� �̹����� ���� ��µ� �̹����� �ٸ� ��츦 ���
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

	mapData* _mapData;					// �� ������
	vector<IMGLNK*> _imgLnks;			// �������� ����� �̹��� ��ȣ��
	
	eLayer _curLayer;

	image* _sampleImg;					// ���� ������
	SAMPLE _pick;						// ������ ����
	eTerrainType _terType;				// ���� ����

	
	bool _isPicking;
	POINTF _pickMousePos;
	
	RECTD2D* _canvers;					// ���� ���� �׷����� �κ�
	RECTD2D  _canversSample;			// ���� �̹����� �׷����� �κ�
	
	RECTD2D* _miniMap;					// �̴ϸ� 
	RECTD2D* _sampleBoard;				// ���� �̹����� ui

	RECTD2D _pickArea;					// �巡��
	RECTD2D _mapViewArea;				// �̴ϸʿ��� ���� �����ִ� �� ǥ��

public:
	mapTool();
	~mapTool();

	HRESULT init();
	void release();
	void update();
	void render();
	void terrainRender();
	void terrainRender(float destX, float destY, float percent);

	// ĵ���� ��ġ ����
	void setSampleBoardRect(RECTD2D* rc);
	
	void setCanversRect(RECTD2D* rc)		{ _canvers = rc; }
	void setMiniMapRect(RECTD2D* rc)		{ _miniMap = rc;}

	// ���� ����
	void pickSample();
	// ĵ���� �׸���
	void pickCanvers();


	void nextSample();
	void beforeSample();
	
	// ���
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

