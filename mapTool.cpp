#include "stdafx.h"
#include "mapTool.h"
#include "mapData.h"
#include "uiButton.h";
#include "uiPanel.h"
#include "uiImage.h"
#include "uiList.h"
#include "uiScroll.h"
#include "terrain.h"


mapTool::mapTool()
: _sampleIdx(0)
, _mapData(nullptr)
, _sampleImg(nullptr)
, _isPicking(false)
, _canvas(nullptr)
, _sampleBoardOpenX(WINSIZEX - SAMPLABOARD_WIDTH)
, _sampleBoardCloseX(WINSIZEX - 5.f)
, _sampleBoardSpeed(15.f)
{
	_pickMousePos = {0.f, 0.f};
	_terrain.clear();
}


mapTool::~mapTool()
{
}

HRESULT mapTool::init()
{
	_sampleIdx = 0;

	_mapData = new mapData;
	_mapData->init();

	settingSampleImageLinks();
	initUI();
	setSampleImage();

	_pick.clear();

	_curLayer = eLayer_Play;
	_terType = eTerrain_Drag;

	_mode = eToolMode_DrawTerrain;

	_isCloseSampleBoard = true;

	CAMERA->setScope(_canvas->getRect());
	
	//_state = nullptr;

	return S_OK;
}

void mapTool::release()
{
}

void mapTool::update()
{
	uiBase::update();

	switch (_mode)
	{
		case eToolMode_DrawTerrain: { updateDrawTerrain();	break;}
		case eToolMode_DrawCollider:{ updateDrawCollider(); break;}
		case eToolMode_DrawObject:  { break; }
		case eToolMode_Inspector:   { break; }
	}

	_samplePanelRc = _samplePanel->getRect();

	if (!_isCloseSampleBoard && !PtInRectD2D(_samplePanelRc, _ptMouse) )
	{
		if ( !_isPicking )
		{
			closingSampleCanvas();
		}
	}
}

void mapTool::render()
{
	// 맵
	if (_mapData)
		_mapData->render();
	if (_terrain.isSet)
	{
		D2DMANAGER->drawRectangle(RGB(0, 0, 255), _terrain.ter->getRect(), false);
		D2DMANAGER->drawRectangle(RGB(0, 255, 0), _terrain.ter->getCollider(), false);
	}
	
	uiBase::render();

	if (_isPicking)
		D2DMANAGER->drawRectangle(_pickArea);
	
	// 미니맵
	{
		vector<terrain*>* terrains = _mapData->getTerrains();
		vector<terrain*>::iterator iter = terrains->begin();
		vector<terrain*>::iterator end = terrains->end();

		for (; iter != end; ++iter)
		{
			(*iter)->render(_miniMap->getWorldPosition().x, _miniMap->getWorldPosition().y, MINIMAP_PERCENT);
		}
		D2DMANAGER->drawRectangle(_miniScope);
		terrains = nullptr;
	}

	switch (_mode)
	{
		case eToolMode_DrawTerrain: { renderDrawTerrain();	break; }
		case eToolMode_DrawCollider:{ renderDrawCollider(); break; }
		case eToolMode_DrawObject:  { break; }
		case eToolMode_Inspector:   { break; }
	}
}

void mapTool::pickSampleStart()
{
	setToolMode(eToolMode_DrawTerrain);
	_createCol->setState(eButton_Up);
	_isPicking = true;
	_pickArea = { _ptMouse.x, _ptMouse.y, _ptMouse.x + 1.f, _ptMouse.y + 1.f };
}

void mapTool::pickSampleEnd()
{
	_pick.clear();
	_isPicking = false;

	_pickArea.right = _ptMouse.x;
	_pickArea.bottom = _ptMouse.y;

	IMGLNK* curLnk = _imgLnks[_sampleIdx];

	// 프레임 이미지
	if (curLnk->isFrameImg)
	{
		int frameX = (int)(_ptMouse.x - _samplecanvas->getWorldPosition().x) / _sampleImg->GetFrameWidth();
		int frameY = (int)(_ptMouse.y - _samplecanvas->getWorldPosition().y) / _sampleImg->GetFrameHeight();

		int idx = frameY * (_sampleImg->GetMaxFrameX() + 1) + frameX;

		if (curLnk->lnkSize <= idx)
			return;

		_pick.uid = curLnk->lnkUIDs[idx];
		_pick.isFrame = true;

		image* img = IMGDATABASE->getImage(_pick.uid);
		_pick.width = img->GetWidth();
		_pick.height = img->GetHeight();
	}
	else
	{
		pickingEnd();
		_pick.uid = curLnk->mainUID;
	}

	_pick.isPick = true;
}

void mapTool::pickcanvasStart()
{
	if (eToolMode_DrawCollider == _mode)
	{
		_isPicking = true;
		_pick.clear();
		_pick.isPick = true;
		_pickArea = { _ptMouse.x, _ptMouse.y, _ptMouse.x + 1.f, _ptMouse.y + 1.f };
	}
}

void mapTool::pickcanvasEnd()
{
	if(!_pick.isPick)
		return;

	// 0,0 기준으로 위치 지정
	float destX = (_ptMouse.x - _pick.width / 2.f) + CAMERA->getPosX() - CAMERA->getScopeRect().left;
	float destY = (_ptMouse.y - _pick.height / 2.f) + CAMERA->getPosY() - CAMERA->getScopeRect().top;

	terrain* ter = nullptr;
	switch (_terType)
	{
		case eTerrain_Frame:
		{
			ter = _mapData->addTerrainFrame(_curLayer, destX, destY, _pick.frameX, _pick.frameY, _pick.uid);
			break;
		}

		case eTerrain_Drag:
		{
			ter = _mapData->addTerrainDrag(_curLayer, destX, destY, _pick.x, _pick.y, _pick.width, _pick.height, _pick.uid);
			break;
		}

		case eTerrain_Clear:
		{
			_pick.clear();
			_isPicking = false;

			_pickArea.right = _ptMouse.x;
			_pickArea.bottom = _ptMouse.y;

			pickingEnd();

			destX = _pickArea.left + CAMERA->getPosX() - CAMERA->getScopeRect().left;
			destY = _pickArea.top + CAMERA->getPosY() - CAMERA->getScopeRect().top;

			ter = _mapData->addTerrainClear(_curLayer, destX, destY, _pick.width, _pick.height);
			break;
		}

		default:
			break;
	}


	if (ter)
	{
		uiButton* btn = new uiButton;
		btn->init("uiBG2", "uiBG3", "uiBG", 0.f, 0.f, 10.f, 10.f);
		btn->setText(format(L"%d", ter->getUID()));
		btn->setOnClickFunction(std::bind(&mapTool::clickBtnTerrain, this, _uiListHierarcy[_curLayer]->getChildCount(), btn));
		btn->setOnClickUPFunction(std::bind(&mapTool::clickUpBtnTerrain, this, _uiListHierarcy[_curLayer]->getChildCount()));

		_uiListHierarcy[_curLayer]->insertChild(btn);

		btn = nullptr;
	}

}

void mapTool::picking()
{
	_pickArea.right = _ptMouse.x;
	_pickArea.bottom = _ptMouse.y;
}

void mapTool::pickingEnd()
{
	float width = _pickArea.right - _pickArea.left;
	float height = _pickArea.bottom - _pickArea.top;

	if (-5.f < width &&  width <= 5.f
		|| -5.f < height && height <= 5.f)
	{
		return;
	}

	// 역사각형일경우
	if (_pickArea.right < _pickArea.left)
	{
		swap(_pickArea.left, _pickArea.right);
		width *= -1.f;
	}

	// 역사각형일경우
	if (_pickArea.bottom < _pickArea.top)
	{
		swap(_pickArea.top, _pickArea.bottom);
		height *= -1;
	}

	POINTF pf = _samplecanvas->getWorldPosition();

	_pick.x = _pickArea.left - pf.x;
	_pick.y = _pickArea.top - pf.y;

	_pick.width = width;
	_pick.height = height;
}

void mapTool::clickingMinimap()
{
	RECTD2D rc = _miniMap->getRect();
	_miniScope = { _ptMouse.x - _miniScopeWidth / 2.f, _ptMouse.y - _miniScopeHeight / 2.f
		, _ptMouse.x + _miniScopeWidth / 2.f, _ptMouse.y + _miniScopeHeight / 2.f };

	if (_miniScope.left < rc.left)
	{
		_miniScope.left = rc.left;
		_miniScope.right  = rc.left + _miniScopeWidth;
	}
	else if (rc.right < _miniScope.right)
	{
		_miniScope.left = rc.right - _miniScopeWidth;
		_miniScope.right = rc.right;
	}

	if (_miniScope.top < rc.top)
	{
		_miniScope.top = rc.top;
		_miniScope.bottom = rc.top + _miniScopeHeight;

		_miniScope = { _miniScope.left, rc.top
			,_miniScope.right, rc.top + _miniScopeHeight };
	}
	else if (rc.bottom < _miniScope.bottom)
	{
		_miniScope.top = rc.bottom - _miniScopeHeight;
		_miniScope.bottom = rc.bottom;
	}

	float destX = (((_miniScope.left - rc.left) / (rc.right - rc.left)) * MAPSIZEX);
	float destY = (((_miniScope.top  - rc.top)/ (rc.bottom - rc.top)) * MAPSIZEY);

	CAMERA->setPosX(destX);
	CAMERA->setPosY(destY);
}

void mapTool::nextSample()
{
	++_sampleIdx;
	if(_imgLnks.size() <= _sampleIdx)
		_sampleIdx = 0;
	
	setSampleImage();
}

void mapTool::beforeSample()
{
	--_sampleIdx;
	if (_sampleIdx < 0)
		_sampleIdx = _imgLnks.size() - 1;
	
	setSampleImage();
}

void mapTool::setToolMode(eToolMode mode)
{
	_mode = mode;
	_pick.clear();
	_isPicking = false;
	if(eToolMode_DrawCollider == mode)
		_terType = eTerrain_Clear;
	else
	{
		if(_imgLnks[_sampleIdx]->isFrameImg)
			_terType = eTerrain_Frame;
		else
			_terType = eTerrain_Drag;
	}
}

void mapTool::settingSampleImageLinks()
{
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_Floor, false);
		_imgLnks.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_BG, false);
		_imgLnks.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_BG_Big01, false);
		_imgLnks.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_BG_Big02, false);
		_imgLnks.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_Build, true);
		for (int ii = eImage_Town_Build01; ii <= eImage_Town_Build07; ++ii)
		{
			lnk->pushBack((eImageUID)ii);
		}
		_imgLnks.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_Ruddle, false);
		_imgLnks.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Object_Chair, false);
		_imgLnks.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Fence, false);
		_imgLnks.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_Well, false);
		_imgLnks.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_StreetLamp, false);
		_imgLnks.push_back(lnk);
		lnk = nullptr;
	}
}

void mapTool::setSampleImage()
{
	_sampleImg = IMGDATABASE->getImage(_imgLnks[_sampleIdx]->mainUID);
	_sampleImage->init(0.f, 0.f, _sampleImg);
	_samplecanvas->setWidth(_sampleImg->GetWidth());
	_samplecanvas->setHeight(_sampleImg->GetHeight());

	if(_imgLnks[_sampleIdx]->isFrameImg)
		_terType = eTerrain_Frame;
	else
		_terType = eTerrain_Drag;

	_pick.clear();
}

void mapTool::initUI()
{
	image* uiBG = IMAGEMANAGER->findImage("bg");

	// 샘플 캔버스
	{
		image* bg = IMAGEMANAGER->findImage("uiBG4");
		RECTD2D rc = {WINSIZEX - UI_SPACE, UI_SPACE / 2.f, SAMPLABOARD_WIDTH, WINSIZEY - UI_SPACE};

		// 판
		_samplePanel = new uiPanel;
		_samplePanel->init(rc.left, rc.top, SAMPLABOARD_WIDTH, WINSIZEY - UI_SPACE, bg);
		_samplePanel->setHoverFunction(std::bind(&mapTool::openingSampleCanvas, this));

		// 샘플
		_samplecanvas = new uiPanel;
		_samplecanvas->init(3.f * DISTANCE, DISTANCE, 0.f, 0.f, nullptr);
		_samplecanvas->setOnClickFunction(std::bind(&mapTool::pickSampleStart, this));
		_samplecanvas->setPressFunction(std::bind(&mapTool::picking, this));
		_samplecanvas->setOnClickUPFunction(std::bind(&mapTool::pickSampleEnd, this));

		_sampleImage = new uiImage;
		_sampleImage->init(0.f, 0.f, nullptr);
		
		_samplecanvas->insertChild(_sampleImage);
		_samplePanel->insertChild(_samplecanvas);

		// 이전 샘플
		_beforeSample = new uiButton;
		_beforeSample->init( "uiBG3", "uiBG"
							, 3.f * UI_SPACE, (rc.bottom - rc.top) - 100.f
							, 60.0f, 50.0f);
		_beforeSample->setText(L"<<", 50);
		_beforeSample->setOnClickFunction(bind(&mapTool::beforeSample, this));
		
		_samplePanel->insertChild(_beforeSample);

		// 다음 샘플
		_nextSample = new uiButton;
		_nextSample->init(  "uiBG3", "uiBG"
						  , _beforeSample->getLocalPosition().x + 60.f + UI_SPACE
						  , _beforeSample->getLocalPosition().y
						  , 60.f, 50.f);
		_nextSample->setText(L">>", 50);
		_nextSample->setOnClickFunction(bind(&mapTool::nextSample, this));

		_samplePanel->insertChild(_nextSample);

		// 샘플판 열고 닫기
		_qickOpen = new uiButton;
		_qickOpen->init( "uiBG3", "uiBG", "uiBG2"
						, -20.f, 0.f
						, 20.f, rc.bottom - rc.top);
		_qickOpen->setText(L"◀\n◀\n◀", 20);
		_qickOpen->setText(L"▶\n▶\n▶", eButton_Down, 20);
		_qickOpen->setOnClickFunction(bind(&mapTool::openSampleCanvas, this));
		_qickOpen->setOnClickUPFunction(bind(&mapTool::closeSampleCanvas, this));

		_samplePanel->insertChild(_qickOpen);

		insertUIObject(_samplePanel);
	}

	// 캔버스
	RECTD2D canvas = {};
	{
		_canvas = new uiPanel;
		_canvas->init( UI_SPACE, UI_SPACE
					  ,WINSIZEX / 3.f * 2.f - UI_SPACE, WINSIZEY / 5.f * 4.f - UI_SPACE
					  ,nullptr);
		_canvas->setOnClickFunction(std::bind(&mapTool::pickcanvasStart, this));
		_canvas->setPressFunction(std::bind(&mapTool::picking, this));
		_canvas->setOnClickUPFunction(std::bind(&mapTool::pickcanvasEnd, this));
		canvas = _canvas->getRect();

		insertUIObject(_canvas);
	}

	RECTD2D miniMap = {};
	// 미니맵
	{
		image* bg = IMAGEMANAGER->findImage("uiBG5");
		RECTD2D rc = _canvas->getRect();
		_miniMap = new uiPanel;
		_miniMap->init( rc.left, rc.bottom + UI_SPACE
					   ,MAPSIZEX * MINIMAP_PERCENT, MAPSIZEY * MINIMAP_PERCENT
					   ,bg);
		_miniMap->setPressFunction(std::bind(&mapTool::clickingMinimap, this));
		miniMap = _miniMap->getRect();

		insertUIObject(_miniMap);

		// 미니맵에서 사용
		{
			RECTD2D rc = _canvas->getRect();
			RECTD2D miniRc = _miniMap->getRect();
			_miniScopeWidth = (rc.right - rc.left) * (miniRc.right - miniRc.left) / MAPSIZEX;
			_miniScopeHeight = (rc.bottom - rc.top) *  (miniRc.bottom - miniRc.top) / MAPSIZEY;

			_miniScope = { miniRc.left, miniRc.top, miniRc.left + _miniScopeWidth, miniRc.top + _miniScopeHeight };
		}
	}

	{
		uiPanel* uiImg = new uiPanel;
		uiImg->init(miniMap.right, canvas.bottom, WINSIZEX - miniMap.right, WINSIZEY - canvas.bottom, uiBG);
		insertUIObject(uiImg);
		uiImg = nullptr;
	}

	// 레이어 별 지형목록
	{
		float btnWitdh = 60.f;
		float btnHeight = 30.f;
		for (int ii = 0; ii < eLayer_Count; ++ii)
		{
			// 레이어 버튼
			_uiBtnHierarcy[ii] = new uiButton;
			_uiBtnHierarcy[ii]->init("uiBG3", "uiBG", "uiBG5"
									, canvas.right + UI_SPACE + (btnWitdh * ii), UI_SPACE, btnWitdh, btnHeight);
			_uiBtnHierarcy[ii]->setText(format(L"%d", (ii + 1)));
			_uiBtnHierarcy[ii]->setOnClickFunction(std::bind(&mapTool::clickBtnHierarcy, this, (eLayer)ii));
			_uiBtnHierarcy[ii]->setOnClickUPFunction(std::bind(&mapTool::clickUpBtnHierarcy, this, (eLayer)ii));

			// 리스트
			_uiListHierarcy[ii] = new uiList;
			_uiListHierarcy[ii]->init( canvas.right + UI_SPACE, _uiBtnHierarcy[ii]->getRect().bottom
									 , btnWitdh * eLayer_Count, TERRAIN_LIST_HEIGHT);
			_uiListHierarcy[ii]->setGap(TERRAIN_LIST_CELL_GAP, TERRAIN_LIST_CELL_GAP);
			_uiListHierarcy[ii]->setCountPerLine(TERRAIN_COUNT_PER_LINE);

			// 리스트 스크롤
			uiScroll* scroll = new uiScroll;
			scroll->init( btnWitdh * eLayer_Count - TERRAIN_LIST_SCROLL_WIDTH
						 ,0.f
						 ,TERRAIN_LIST_SCROLL_WIDTH
						 ,TERRAIN_LIST_HEIGHT
						 ,nullptr
						 ,nullptr);
			scroll->setScrollDirect(false);

			_uiListHierarcy[ii]->setScroll(scroll);
			_uiListHierarcy[ii]->setCellHeight(30.f);

			insertUIObject(_uiBtnHierarcy[ii]);
			insertUIObject(_uiListHierarcy[ii]);

			scroll = nullptr;
		}

		clickBtnHierarcy(eLayer_Play);
		_uiBtnHierarcy[eLayer_Play]->setState(eButton_Down);
	}

	// 속성창 & 버튼
	{
		RECTD2D hieracy = _uiListHierarcy[0]->getRect();
		float width = _sampleBoardCloseX - _qickOpen->getWidth() - (hieracy.right + UI_SPACE) - UI_SPACE;
		_uiPanelInspector = new uiPanel;
		_uiPanelInspector->init(hieracy.right + UI_SPACE, UI_SPACE, width, width, IMGDATABASE->getImage(eImage_UI_BG5));
		
		UINT attr = 0;
		wstring txts[ATTR_COUNT + 1] = { L"Collistion"
										,L"Collider"
										,L"Trigger"
										,L"Breakable"
										,L"Interaction"
										,L"Trap" };
		for (int ii = 0; ii < ATTR_COUNT + 1; ++ii)
		{
			if(0 == ii)
				attr = 0;
			else
				attr = pow(2, (ii - 1));
			_uiBtnInspectors[ii] = new uiButton;
			_uiBtnInspectors[ii]->init("uiBG2", "uiBG3", "uiBG", 5.f, 5.f + (ii * 25.f), width - 10.f, 20.f);
			_uiBtnInspectors[ii]->setText(txts[ii], 15);
			_uiBtnInspectors[ii]->setOnClickFunction(std::bind(&mapTool::clickBtnInspector, this, attr, _uiBtnInspectors[ii]));
			_uiBtnInspectors[ii]->setOnClickUPFunction(std::bind(&mapTool::clickUpBtnInspector, this, attr));
			_uiPanelInspector->insertChild(_uiBtnInspectors[ii]);
		}
		
		insertUIObject(_uiPanelInspector);
	}

	// 충돌체 생성 버튼
	{
		_createCol = new uiButton;
		RECTD2D rc = _canvas->getRect();

		_createCol->init("uiBG3", "uiBG", "uiBG2"
						 , rc.right + UI_SPACE
						 , rc.bottom - 50.f
						 , 80.f, 50.f);
		_createCol->setText(L"Collider", 20);
		_createCol->setOnClickFunction(bind(&mapTool::setToolMode, this, eToolMode_DrawCollider));
		_createCol->setOnClickUPFunction(bind(&mapTool::setToolMode, this, eToolMode_DrawTerrain));

		insertUIObject(_createCol);
	}

	// canvas와 미니맵 사이
	{
		RECTD2D rc = _canvas->getRect();
		uiPanel* uiImg = new uiPanel;
		uiImg->init(rc.left, rc.bottom, rc.right - rc.left, UI_SPACE, uiBG);

		insertUIObject(uiImg);

		uiImg = nullptr;
	}

	// canvas 상좌우, 미니맵 우하
	{
		{
			uiPanel* uiImg = new uiPanel;
			uiImg->init(0.f, 0.f, WINSIZEX, UI_SPACE, uiBG);
			insertUIObject(uiImg);
			uiImg = nullptr;
		}
		{
			uiPanel* uiImg = new uiPanel;
			uiImg->init(0.f, 0.f, UI_SPACE, WINSIZEY, uiBG);
			insertUIObject(uiImg);
			uiImg = nullptr;
		}
		{
			uiPanel* uiImg = new uiPanel;
			uiImg->init(canvas.right, 0.f,  WINSIZEX - canvas.right, WINSIZEY, uiBG);
			insertUIObject(uiImg);
			uiImg = nullptr;
		}
		{
			uiPanel* uiImg = new uiPanel;
			uiImg->init(miniMap.right, canvas.bottom,  WINSIZEX - miniMap.right, WINSIZEY - canvas.bottom, uiBG);
			insertUIObject(uiImg);
			uiImg = nullptr;
		}
		{
			uiPanel* uiImg = new uiPanel;
			uiImg->init(miniMap.right, miniMap.bottom,  miniMap.right - miniMap.left, WINSIZEY - miniMap.bottom, uiBG);
			insertUIObject(uiImg);
			uiImg = nullptr;
		}
	}
}

void mapTool::updateDrawTerrain()
{
	// 마우스 입력이 없다면 picking해제
	if (_isPicking)
		_isPicking = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);

	// 마우스 우클릭하면 픽 정보 클리어
	if (_pick.isPick)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
			_pick.clear();
	}
}

void mapTool::updateDrawCollider()
{
}

void mapTool::renderDrawTerrain()
{
	if (_pick.isPick)
	{
		float destX = _ptMouse.x - _pick.width / 2.f;
		float destY = _ptMouse.y - _pick.height / 2.f;
		if (_pick.isFrame)
			IMGDATABASE->getImage(_pick.uid)->render(destX, destY, 0.5f);
		else
			_sampleImg->render(destX, destY, _pick.x, _pick.y, _pick.width, _pick.height, 0.5f);
	}
}

void mapTool::renderDrawCollider()
{
}

void mapTool::openSampleCanvas()
{
	if (!_isOpenSampleBoard)
	{
		_samplePanel->setWorldPosition(_sampleBoardOpenX, _samplePanel->getWorldPosition().y);

		_qickOpen->setLocalPosition(0.f, 0.f);

		_qickOpen->setState(eButton_Down);
		_isOpenSampleBoard = true;
		_isCloseSampleBoard = false;
	}
}

void mapTool::openingSampleCanvas()
{
	POINTF pf = _samplePanel->getWorldPosition();
	if (_sampleBoardOpenX < pf.x)
	{
		pf.x -= _sampleBoardSpeed;
		_samplePanel->setWorldPosition(pf.x, pf.y);
		
		_qickOpen->setLocalPosition(0.f, 0.f);

		_isOpenSampleBoard = false;
		_isCloseSampleBoard = false;
	}

	if (pf.x <= _sampleBoardOpenX)
	{
		openSampleCanvas();
	}
}

void mapTool::closingSampleCanvas()
{
	POINTF pf = _samplePanel->getWorldPosition();
	if (pf.x < _sampleBoardCloseX)
	{
		pf.x += _sampleBoardSpeed;
		_samplePanel->setWorldPosition(pf.x, pf.y);

		_isOpenSampleBoard = false;
		_isCloseSampleBoard = false;
	}

	if (_sampleBoardCloseX <= pf.x)
	{
		closeSampleCanvas();
	}
}

void mapTool::closeSampleCanvas()
{
	if (!_isCloseSampleBoard)
	{
		_samplePanel->setWorldPosition(_sampleBoardCloseX, _samplePanel->getWorldPosition().y);
		_qickOpen->setState(eButton_Up);

		_qickOpen->setLocalPosition(-20.f, 0.f);

		_isOpenSampleBoard = false;
		_isCloseSampleBoard = true;
	}
}

void mapTool::clickBtnHierarcy(eLayer layer)
{
	_curLayer = layer;

	for (int ii = 0; ii < eLayer_Count; ++ii)
	{
		if (ii == layer)
		{
			_uiListHierarcy[ii]->setActive(true);
			_uiListHierarcy[ii]->setViewing(true);
		}
		else
		{
			_uiListHierarcy[ii]->setActive(false);
			_uiListHierarcy[ii]->setViewing(false);

			_uiBtnHierarcy[ii]->setState(eButton_Up);
		}
	}
}

void mapTool::clickUpBtnHierarcy(eLayer layer)
{
	_uiBtnHierarcy[layer]->setState(eButton_Down);
}

void mapTool::clickBtnTerrain(int idx, uiButton* btn)
{
	int count = ATTR_COUNT + 1;
	terrain* ter = _mapData->getTerrain(_curLayer, idx);
	if (ter)
	{
		_terrain.clear();
		_terrain.infoSet(ter, _curLayer, idx);
		
		if (_curBtnTerrain)
		{
			if(_curBtnTerrain != btn)
				_curBtnTerrain->setState(eButton_Up);
		}
		_curBtnTerrain = btn;

		_pick.clear();


		// 상태창
		UINT attr = 0;
		RECTD2D col = _terrain.ter->getCollider();
		if (0 < (col.right - col.left) && 0 < (col.bottom - col.top))
			_uiBtnInspectors[0]->setState(eButton_Down);
		else
			_uiBtnInspectors[0]->setState(eButton_Up);

		for (int ii = 1; ii < count; ++ii)
		{
			if (_terrain.ter->checkAttribute(pow(2, (ii - 1))))
				_uiBtnInspectors[ii]->setState(eButton_Down);
			else
				_uiBtnInspectors[ii]->setState(eButton_Up);
		}

		// 카메라 밖에 있다면 카메라 이동
		RECTD2D rc = ter->getRect();
		if (!CAMERA->isRangeInCamera(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top))
		{
			float offsetX = _canvas->getWidth() / 2.f;
			float offsetY = _canvas->getHeight() / 2.f;

			CAMERA->setPosX(rc.left - offsetX);
			CAMERA->setPosY(rc.top - offsetY);
		}
	}
	else
	{
		for (int ii = 0; ii < count; ++ii)
		{
			_uiBtnInspectors[ii]->setState(eButton_Up);
		}
	}
}

void mapTool::clickUpBtnTerrain(int idx)
{
	_terrain.clear();
	_curBtnTerrain = nullptr;
	int count = ATTR_COUNT + 1;
	for (int ii = 0; ii < count; ++ii)
	{
		_uiBtnInspectors[ii]->setState(eButton_Up);
	}
}

void mapTool::clickBtnInspector(UINT attr, uiButton* btn)
{
	if (_terrain.ter)
	{
		if (0 == attr)
		{
			RECTD2D rc = _terrain.ter->getRect();
			_terrain.ter->setCollider(rc);
		}
		else
		{
			_terrain.ter->addAttribute(attr);
		}
		_mapData->changeTerrain(_terrain.layer, _terrain.idx, _terrain.ter);
	}
	else
	{
		btn->setState(eButton_Up);
	}
}

void mapTool::clickUpBtnInspector(UINT attr)
{
	if (_terrain.ter)
	{
		if (0 == attr)
		{
			RECTD2D rc = {};
			_terrain.ter->setCollider(rc);
		}
		else
		{
			_terrain.ter->removeAttribute(attr);
		}
		_mapData->changeTerrain(_terrain.layer, _terrain.idx, _terrain.ter);
	}
}