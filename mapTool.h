#pragma once
#include "toolState.h"

#define SAMPLABOARD_WIDTH 1010.f

#define DISTANCE 10.f
#define MINIMAP_PERCENT 0.019f				// �̴ϸ� ����

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
	int _moveSnap;		// �̵� ����

	mapData* _mapData;						// �� ������
	vector<IMGLNK*> _imgLnksTerrain;		// �������� ����� �̹��� ��ȣ��
	vector<IMGLNK*> _imgLnksObject;			// �������� ����� �̹��� ��ȣ��
	vector<IMGLNK*> _imgLnksNpc;			// �������� ����� �̹��� ��ȣ��
	vector<IMGLNK*> _imgLnksEnemy;			// �������� ����� �̹��� ��ȣ��
	
	eLayer _curLayer;

	image* _sampleImg;					// ���� ������
	SAMPLE _pick;						// ������ ����
	eSceneName _curFileName;			// ���� ����
	eSceneName _selFileName;			// ������ ����
	
	bool _isPicking;
	bool _isOpenSampleBoard;
	bool _isCloseSampleBoard;
	bool _isShowAllLayer;			// ��� ���̾� ����
	bool _isReposition;				// ��ġ �ٽ�

	POINTF _pickMousePos;
	POINTF _originPos;				// �����̱� �� ��ġ
	
	uiPanel* _samplePanel;			// ���� �̹����� ui
	uiPanel* _samplecanvas;			// ���� �̹����� �׷����� �κ�
	uiImage* _sampleImage;

	uiPanel* _canvas;				// ���� ���� �׷����� �κ�

	uiPanel* _miniMap;				// �̴ϸ� 

	uiButton* _qickOpen;

	uiButton* _uiBtnDrawCollision;	// �̹��� ���� �浹ü ����
	uiButton* _uiBtnDrawTerrain;	// ����
	uiButton* _uiBtnDrawObject;		// ������Ʈ(������ �̹���)
	uiButton* _uiBtnDrawNpc;		// npc
	uiButton* _uiBtnDrawEnemy;		// enemy

	uiButton* _uiBtnDelTerrain;		// ���� ����
	uiButton* _uiBtnUpIndex;		// ���� �ø���(������ ����)
	uiButton* _uiBtnDownIndex;		// �Ʒ��� (������ ���߿�)
	
	uiButtonDrop* _uiBtnDropChangeLayer;	// ���̾� ����

	uiButton* _uiBtnViewMode[eViewMode_Count];	// View Mode
	
	uiButton* _uiBtnMoveUp;				// ���� �̵�
	uiButton* _uiBtnMoveDown;			// �Ʒ��� �̵�
	uiButton* _uiBtnMoveLeft;			// ��
	uiButton* _uiBtnMoveRight;			// ��
	uiButton* _uiBtnMoveLeftUp;			// �»�
	uiButton* _uiBtnMoveRightUp;		// ���
	uiButton* _uiBtnMoveLeftDown;		// ����
	uiButton* _uiBtnMoveRightDown;		// ����

	uiButton* _uiBtnMoveReset;			// ��ġ �ٽ� 

	uiButton* _uiBtnMoveSnapPlus;		// �̵� ���� �ø���
	uiButton* _uiBtnMoveSnapMinus;		//			���̱�

	uiText*	_uiTextMoveSnap;			// �̵� ���� ǥ��

	uiText* _uiButtonReverse;			// ���� ����

	uiText* _uiTextCurFileName;			// ���� ���� �̸�			
	uiList* _uiListFiles;				// ���� �̸���
	uiButton* _uiBtnSave;				// ����
	uiButton* _uiBtnLoad;				// �ε�;

	// hierarcy
	uiButton* _uiBtnHierarcy[eLayer_Count + 1];
	uiList* _uiListHierarcy[eLayer_Count + 1];

	// ����â(�ɼ�)
	uiText* _uiTextInspector;
	uiPanel* _uiPanelInspector;
	uiButton* _uiBtnInspectors[eAttr_Count];

	// ����â ��
	uiText* _uiTextInspectorSub;
	uiPanel* _uiPanelInspectorInfo;
	uiText* _uiTextInspectorSubInfo;

	// ���̾� ����
	uiPanel* _uiPanelPopUpLayerChange;



	RECTD2D _miniScope;
	RECTD2D _pickArea;					// �巡��
	RECTD2D _mapViewArea;				// �̴ϸʿ��� ���� �����ִ� �� ǥ��

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

	// ���� ����
	void pickSampleStart();
	void pickSampleEnd();
	// ĵ���� �׸���
	void pickcanvasStart();
	void pickcanvasEnd();
	// �巡��
	void picking();

	void pickingEnd();

	void clickingMinimap();
	
	void nextSample();
	void beforeSample();
	
	// ���
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
