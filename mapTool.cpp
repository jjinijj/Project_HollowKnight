#include "stdafx.h"
#include "mapTool.h"
#include "mapData.h"
#include "terrain.h"
#include "npc.h"
#include "uiHeaders.h"


mapTool::mapTool()
: _sampleIdx(0)
, _moveSnap(1)
, _mapData(nullptr)
, _sampleImg(nullptr)
, _isPicking(false)
, _isShowAllLayer(true)
, _canvas(nullptr)
, _sampleBoardOpenX(WINSIZEX - SAMPLABOARD_WIDTH)
, _sampleBoardCloseX(WINSIZEX - 5.f)
, _sampleBoardSpeed(15.f)
{
	_pickMousePos = {0.f, 0.f};
	_select.clear();
}


mapTool::~mapTool()
{
}

HRESULT mapTool::init()
{
	_sampleIdx = 0;

	_mapData = new mapData;
	_mapData->init();

	initTerrainImgLinks();
	initObjectImgLinks();
	initNpcImgLinks();

	initUI();
	setSampleImage();

	_pick.clear();

	_curLayer = eLayer_Play;

	_mode = eToolMode_DrawTerrain;

	_isCloseSampleBoard = true;

	CAMERA->setScope(_canvas->getRect());
	
	//_state = nullptr;

	return S_OK;
}

void mapTool::release()
{
	_select.clear();
	_pick.clear();

	_curBtnTerrain = nullptr;
	_sampleImg = nullptr;
	_samplePanel = nullptr;
	_samplecanvas = nullptr;
	_sampleImage = nullptr;
	_canvas = nullptr;
	_qickOpen = nullptr;
	_uiBtnDrawCollision = nullptr;
	_uiBtnDrawTerrain = nullptr;
	_uiBtnDrawObject = nullptr;
	_uiBtnDrawNpc = nullptr;
	_uiBtnDelTerrain = nullptr;
	_uiBtnUpIndex = nullptr;
	_uiBtnDownIndex = nullptr;
	_uiBtnDropChangeLayer = nullptr;

	for(int ii = 0; ii < eViewMode_Count; ++ii)
		_uiBtnViewMode[ii] = nullptr;

	for (int ii = 0; ii < eLayer_Count; ++ii)
	{
		_uiBtnHierarcy[ii] = nullptr;
		_uiListHierarcy[ii] = nullptr;
	}

	_uiTextInspector = nullptr;
	_uiPanelInspector = nullptr;

	for(int ii = 0 ; ii < eAttr_Count; ++ii)
		_uiBtnInspectors[ii] = nullptr;

	_uiTextInspectorSub = nullptr;
	_uiPanelInspectorInfo = nullptr;
	_uiTextInspectorSubInfo = nullptr;
	_uiPanelPopUpLayerChange = nullptr;


	SAFE_RELEASE(_mapData);
	SAFE_DELETE(_mapData);

	while (_imgLnksTerrain.size() != 0)
	{
		IMGLNK* lnk = _imgLnksTerrain.back();
		_imgLnksTerrain.pop_back();
	
		SAFE_DELETE(lnk);
	}

	while (_imgLnksObject.size() != 0)
	{
		IMGLNK* lnk = _imgLnksObject.back();
		_imgLnksObject.pop_back();

		SAFE_DELETE(lnk);
	}

	while (_imgLnksNpc.size() != 0)
	{
		IMGLNK* lnk = _imgLnksNpc.back();
		_imgLnksNpc.pop_back();

		SAFE_DELETE(lnk);
	}
}

void mapTool::update()
{
	uiBase::update();

	switch (_mode)
	{
		case eToolMode_DrawTerrain: { updateDrawTerrain();	break;}
		case eToolMode_DrawCollider:{ updateDrawCollider(); break;}
		case eToolMode_DrawObject:  { break; }
		case eToolMode_DrawNpc:   { break; }
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
	{
		if(_isShowAllLayer)
			_mapData->render();
		else
			_mapData->render(_curLayer);
	}

	if (_select.isSet)
	{
		D2DMANAGER->drawRectangle(RGB(0, 0, 255), _select.obj->getRect(), false);
		D2DMANAGER->drawRectangle(RGB(0, 255, 255), _select.obj->getCollision(), false);
	}
	
	uiBase::render();
	_sampleImage;

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

		RECTD2D rc = _miniMap->getRect();
		_miniScope.left = (CAMERA->getPosX() / MAPSIZEX) * (rc.right - rc.left) + rc.left;
		_miniScope.top = (CAMERA->getPosY() / MAPSIZEY) * (rc.bottom - rc.top) + rc.top;
		_miniScope.right = _miniScope.left + _miniScopeWidth;
		_miniScope.bottom = _miniScope.top + _miniScopeHeight;

		D2DMANAGER->drawRectangle(_miniScope);
		terrains = nullptr;
	}

	switch (_mode)
	{
		case eToolMode_DrawTerrain: { renderDrawTerrain();	break; }
		case eToolMode_DrawCollider:{ renderDrawCollider(); break; }
		case eToolMode_DrawObject:  { renderDrawObject();	break; }
		case eToolMode_DrawNpc:		{ renderDrawNpc();		break; }
	}
}

void mapTool::pickSampleStart()
{
	if(!_sampleImg)
		return;

	_isPicking = true;
	_pickArea = { _ptMouse.x, _ptMouse.y, _ptMouse.x + 1.f, _ptMouse.y + 1.f };
}

void mapTool::pickSampleEnd()
{
	_pick.clear();
	_isPicking = false;

	if(!_sampleImg)
		return;

	IMGLNK* curLnk = nullptr;
	
	_pickArea.right = _ptMouse.x;
	_pickArea.bottom = _ptMouse.y;
	
	switch (_mode)
	{
		case eToolMode_DrawTerrain :
		{
			if (_sampleImg)
			{
				curLnk = _imgLnksTerrain[_sampleIdx]; 

				pickingEnd();
				_pick.uid = curLnk->mainUID;
			}
			
			break; 
		}

		case eToolMode_DrawCollider :
		{
			break; 
		}
		case eToolMode_DrawObject :		
		{
			curLnk = _imgLnksObject[_sampleIdx]; 

			int frameX = (int)(_ptMouse.x - _samplecanvas->getWorldPosition().x) / _sampleImg->GetFrameWidth();
			int frameY = (int)(_ptMouse.y - _samplecanvas->getWorldPosition().y) / _sampleImg->GetFrameHeight();

			int idx = frameY * (_sampleImg->GetMaxFrameX() + 1) + frameX;

			if (curLnk->lnkSize <= idx)
				return;

			_pick.uid = curLnk->lnkUIDs[idx];
			_pick.isFrame = true;

			image* img = IMGDATABASE->getImage(_pick.uid);
			_pick.width = static_cast<float>(img->GetWidth());
			_pick.height = static_cast<float>(img->GetHeight());

			break; 
		}
		case eToolMode_DrawNpc :	
		{
			curLnk = _imgLnksNpc[_sampleIdx]; 

			int frameX = (int)(_ptMouse.x - _samplecanvas->getWorldPosition().x) / _sampleImg->GetFrameWidth();
			int frameY = (int)(_ptMouse.y - _samplecanvas->getWorldPosition().y) / _sampleImg->GetFrameHeight();

			int idx = 0;

			if(0 <= frameX && frameX <= 1 && 0 <= frameY && frameY < 2)
				idx = 0;
			else if(1 < frameX && frameX <= 3 && 0 <= frameY && frameY < 2)
				idx = 1;
			else if(1 < frameX && frameX <= 3 && 2 <= frameY && frameY < 4)
				idx = 2;
			else if(0 <= frameX && frameX <= 1 && 2 <= frameY && frameY < 4)
				idx = 3;
			else if(0 <= frameX && frameX <= 4 &&  4 <= frameY && frameY < 6)
				idx = 4;
			else if(3 < frameX && frameX <= 7 && 0 <= frameY && frameY < 4)
				idx = 5;

			_pick.uid = curLnk->lnkUIDs[idx];
			_pick.isFrame = true;

			image* img = IMGDATABASE->getImage(_pick.uid);
			if ( img )
			{
				_pick.width = static_cast<float>(img->GetFrameWidth());
				_pick.height = static_cast<float>(img->GetFrameHeight());
			}
			else
				return;

			break; 
		}

		default:
		{
			return;	// 모드가 없으므로
		}
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
	actorBase* actor = nullptr;
	switch (_mode)
	{
		case eToolMode_DrawTerrain:
		{
			ter = _mapData->addTerrainDrag(_curLayer, destX, destY, _pick.x, _pick.y, _pick.width, _pick.height, _pick.uid);
			break;
		}

		case eToolMode_DrawObject:
		{
			ter = _mapData->addTerrainFrame(_curLayer, destX, destY, _pick.frameX, _pick.frameY, _pick.uid);
			break;
		}

		case eToolMode_DrawCollider:
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

		case eToolMode_DrawNpc:
		{
			actor = _mapData->addNpc(destX, destY, _pick.uid);
			_pick.clear();
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
		btn->setOnClickFunction(std::bind(&mapTool::clickBtnTerrain, this, ter->getUID(), btn));
		btn->setOnClickUPFunction(std::bind(&mapTool::clickUpBtnTerrain, this, ter->getUID()));

		_uiListHierarcy[_curLayer]->insertChild(btn);
	}
	else if (actor)
	{
		uiButton* btn = new uiButton;
		btn->init("uiBG2", "uiBG3", "uiBG", 0.f, 0.f, 10.f, 10.f);
		btn->setText(actor->getName());
		btn->setOnClickFunction(std::bind(&mapTool::clickBtnActor, this, actor->getUID(), btn));
		btn->setOnClickUPFunction(std::bind(&mapTool::clickUpBtnActor, this, actor->getUID()));

		//actor는 무조건 elyaer_play에 생성
		_uiListHierarcy[eLayer_Play]->insertChild(btn);
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

	// 너무 작을경우 return
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
	
	switch (_mode)
	{
		case eToolMode_DrawTerrain:
		{
			if (_imgLnksTerrain.size() <= _sampleIdx)
				_sampleIdx = 0;
			break;
		}

		case eToolMode_DrawCollider:
		{
			break;
		}

		case eToolMode_DrawObject:
		{
			if (_imgLnksObject.size() <= _sampleIdx)
				_sampleIdx = 0;
			break;
		}
		case eToolMode_DrawNpc:
		{
			if (_imgLnksNpc.size() <= _sampleIdx)
				_sampleIdx = 0;
			break;
		}

		default:
		{
			return;	// 모드가 없으므로
		}
	}
	
	setSampleImage();
}

void mapTool::beforeSample()
{
	--_sampleIdx;
	
	switch (_mode)
	{
		case eToolMode_DrawTerrain:
		{
			if (_sampleIdx < 0)
				_sampleIdx = static_cast<int>(_imgLnksTerrain.size()) - 1;
			break;
		}

		case eToolMode_DrawCollider:
		{
			break;
		}

		case eToolMode_DrawObject:
		{
			if (_sampleIdx < 0)
				_sampleIdx = static_cast<int>(_imgLnksObject.size()) - 1;
			break;
		}
		case eToolMode_DrawNpc:
		{
			if (_sampleIdx < 0)
				_sampleIdx = static_cast<int>(_imgLnksNpc.size()) - 1;
			break;
		}
	}

	setSampleImage();
}

void mapTool::setToolMode(eToolMode mode)
{
	switch (mode)
	{
		case eToolMode_DrawTerrain:
		{
			_uiBtnDrawCollision->setState(eButton_Up);
			_uiBtnDrawObject->setState(eButton_Up);
			_uiBtnDrawNpc->setState(eButton_Up);

			break;
		}
		case eToolMode_DrawCollider:
		{
			_uiBtnDrawTerrain->setState(eButton_Up);
			_uiBtnDrawObject->setState(eButton_Up);
			_uiBtnDrawNpc->setState(eButton_Up);
			break;
		}
		case eToolMode_DrawObject:
		{
			_uiBtnDrawCollision->setState(eButton_Up);
			_uiBtnDrawTerrain->setState(eButton_Up);
			_uiBtnDrawNpc->setState(eButton_Up);
			break;
		}
		case eToolMode_DrawNpc:
		{
			_uiBtnDrawCollision->setState(eButton_Up);
			_uiBtnDrawTerrain->setState(eButton_Up);
			_uiBtnDrawObject->setState(eButton_Up);
			break;
		}
	}

	_mode = mode;
	_pick.clear();
	_isPicking = false;
	_sampleIdx = 0;

	setSampleImage();
}

void mapTool::setSampleImage()
{
	switch (_mode)
	{
		case eToolMode_DrawTerrain:
		{
			if (_sampleIdx < _imgLnksTerrain.size())
				_sampleImg = IMGDATABASE->getImage(_imgLnksTerrain[_sampleIdx]->mainUID);
			else
				_sampleImg = nullptr;

			break;
		}

		case eToolMode_DrawCollider:
		{
			_sampleImg = nullptr;
			break;
		}

		case eToolMode_DrawObject:
		{
			if(_sampleIdx < _imgLnksObject.size())
				_sampleImg = IMGDATABASE->getImage(_imgLnksObject[_sampleIdx]->mainUID);
			else
				_sampleImg = nullptr;

			break;
		}
		case eToolMode_DrawNpc:
		{
			if(_sampleIdx < _imgLnksNpc.size())
				_sampleImg = IMGDATABASE->getImage(_imgLnksNpc[_sampleIdx]->mainUID);
			else
				_sampleImg = nullptr;
			break;
		}

		default:
		{
			_sampleImg = nullptr;
			break;
		}
	}

	if (_sampleImg)
	{
		_sampleImage->init(0.f, 0.f, _sampleImg);
		_samplecanvas->setWidth(static_cast<float>(_sampleImg->GetWidth()));
		_samplecanvas->setHeight(static_cast<float>(_sampleImg->GetHeight()));

		_sampleImage->setViewing(true);
		_sampleImage->setActive(true);

	}
	else
	{
		_sampleImage->setViewing(false);
		_sampleImage->setActive(false);
	}

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
		uiButton* beforeSample = new uiButton;
		beforeSample->init("uiBG3", "uiBG"
							, 3.f * UI_SPACE, (rc.bottom - rc.top) - 80.f
							, 60.0f, 50.0f);
		beforeSample->setText(L"<<", 50);
		beforeSample->setOnClickFunction(bind(&mapTool::beforeSample, this));

		_samplePanel->insertChild(beforeSample);

		// 다음 샘플
		uiButton* nextSample = new uiButton;
		nextSample->init("uiBG3", "uiBG"
						  , beforeSample->getLocalPosition().x + 60.f + UI_SPACE
						  , beforeSample->getLocalPosition().y
						  , 60.f, 50.f);
		nextSample->setText(L">>", 50);
		nextSample->setOnClickFunction(bind(&mapTool::nextSample, this));

		_samplePanel->insertChild(nextSample);

		// 충돌체
		_uiBtnDrawCollision = new uiButton;
		_uiBtnDrawCollision->init("uiBG3", "uiBG", "uiBG2"
								  , nextSample->getRect().right + UI_SPACE
								  , nextSample->getLocalPosition().y
								  , 200.f, 50.f);
		_uiBtnDrawCollision->setText(L"Collider", 50);
		_uiBtnDrawCollision->setOnClickFunction(bind(&mapTool::setToolMode, this, eToolMode_DrawCollider));
		_uiBtnDrawCollision->setOnClickUPFunction(bind(&mapTool::setToolMode, this, _beforeMode));

		_samplePanel->insertChild(_uiBtnDrawCollision);


		// 지형
		_uiBtnDrawTerrain = new uiButton;
		_uiBtnDrawTerrain->init( "uiBG3", "uiBG", "uiBG2"
							, _uiBtnDrawCollision->getRect().right + UI_SPACE
							, nextSample->getLocalPosition().y
							,180.f, 50.f);
		_uiBtnDrawTerrain->setText(L"Terrain", 50);
		_uiBtnDrawTerrain->setOnClickFunction(bind(&mapTool::setToolMode, this, eToolMode_DrawTerrain));
		_uiBtnDrawTerrain->setOnClickUPFunction(bind(&mapTool::clickBtnUpNone, this, _uiBtnDrawTerrain));
		_samplePanel->insertChild(_uiBtnDrawTerrain);

		// 오브젝트
		_uiBtnDrawObject = new uiButton;
		_uiBtnDrawObject->init("uiBG3", "uiBG", "uiBG2"
							, _uiBtnDrawTerrain->getRect().right + UI_SPACE
							, _uiBtnDrawTerrain->getLocalPosition().y
							, 150.f, 50.f);
		_uiBtnDrawObject->setText(L"Object", 50);
		_uiBtnDrawObject->setOnClickFunction(bind(&mapTool::setToolMode, this, eToolMode_DrawObject));
		_uiBtnDrawObject->setOnClickUPFunction(bind(&mapTool::clickBtnUpNone, this, _uiBtnDrawObject));
		_samplePanel->insertChild(_uiBtnDrawObject);

		// npc
		_uiBtnDrawNpc = new uiButton;
		_uiBtnDrawNpc->init("uiBG3", "uiBG", "uiBG2"
						   , _uiBtnDrawObject->getRect().right + UI_SPACE
						   , _uiBtnDrawObject->getLocalPosition().y
						   , 80.f, 50.f);
		_uiBtnDrawNpc->setText(L"NPC", 50);
		_uiBtnDrawNpc->setOnClickFunction(bind(&mapTool::setToolMode, this, eToolMode_DrawNpc));
		_uiBtnDrawNpc->setOnClickUPFunction(bind(&mapTool::clickBtnUpNone, this, _uiBtnDrawNpc));
		_samplePanel->insertChild(_uiBtnDrawNpc);

		setToolMode(_mode);
		_uiBtnDrawTerrain->setState(eButton_Down);

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
	RECTD2D hieracy = _uiListHierarcy[0]->getRect();

	// 지형 관리 버튼
	{
		// 지형 삭제
		_uiBtnDelTerrain = new uiButton();
		_uiBtnDelTerrain->init("uiBG3", "uiBG"
							   ,hieracy.left, hieracy.bottom + UI_SPACE
							   ,65.f, 50.f);
		_uiBtnDelTerrain->setText(L"Del", 20);
		_uiBtnDelTerrain->setOnClickFunction(std::bind(&mapTool::clickBtnDelTerrain, this));
		insertUIObject(_uiBtnDelTerrain);

		// 순서 up
		_uiBtnUpIndex = new uiButton();
		_uiBtnUpIndex->init("uiBG3", "uiBG"
							,_uiBtnDelTerrain->getRect().right + UI_SPACE
							,_uiBtnDelTerrain->getRect().top
							,65.f, 50.f);
		_uiBtnUpIndex->setText(L"Up", 20);
		_uiBtnUpIndex->setOnClickFunction(std::bind(&mapTool::clickBtnUpIndex, this));
		insertUIObject(_uiBtnUpIndex);

		// 순서 down
		_uiBtnDownIndex = new uiButton();
		_uiBtnDownIndex->init("uiBG3", "uiBG"
							  ,_uiBtnUpIndex->getRect().right + UI_SPACE
							  ,_uiBtnUpIndex->getRect().top
							  , 65.f, 50.f);
		_uiBtnDownIndex->setText(L"Down", 20);
		_uiBtnDownIndex->setOnClickFunction(std::bind(&mapTool::clickBtnDownIndex, this));
		insertUIObject(_uiBtnDownIndex);

		// 레이어 변경
		_uiBtnDropChangeLayer = new uiButtonDrop();
		_uiBtnDropChangeLayer->init("uiBG3", "uiBG", "uiBG5"
								,_uiBtnDownIndex->getRect().right + UI_SPACE
								,_uiBtnDownIndex->getRect().top
								, 65.f, 50.f);
		_uiBtnDropChangeLayer->setText(L"Change\nLayer", 20);

		for (int ii = 0; ii < eLayer_Count; ++ii)
		{
			uiButton* btn = new uiButton;
			btn->init("uiBG3", "uiBG"
					  , _uiBtnUpIndex->getRect().right + UI_SPACE
					  , _uiBtnUpIndex->getRect().top
					  , 30.f, 30.f);
			btn->setText(format(L"%d", (ii + 1)), 20);

			btn->setActive(false);
			btn->setViewing(false);
			btn->setOnClickFunction(std::bind(&mapTool::clickBtnChangeLayer, this, (eLayer)ii));


			_uiBtnDropChangeLayer->insertChild(btn);

		}

		insertUIObject(_uiBtnDropChangeLayer);
	}

	// 뷰 모드
	{
		RECTD2D rc = _uiBtnDelTerrain->getRect();
		for (int ii = 0; ii < eViewMode_Count; ++ii)
		{
			_uiBtnViewMode[ii] = new uiButton;
			_uiBtnViewMode[ii]->init("uiBG3", "uiBG", "uiBG5"
									 ,rc.left + ((50.f + UI_SPACE) * ii)
									 ,rc.bottom + UI_SPACE
									 ,50.f, 50.f);

			_uiBtnViewMode[ii]->setOnClickFunction(std::bind(&mapTool::clickBtnViewMode, this, (eViewMode)ii));
			_uiBtnViewMode[ii]->setOnClickUPFunction(std::bind(&mapTool::clickBtnUpViewMode, this, (eViewMode)ii));

			insertUIObject(_uiBtnViewMode[ii]);

		}

		_uiBtnViewMode[eViewMode_Layer]->setText(L" View\n Layer", 20);
		_uiBtnViewMode[eViewMode_Collision]->setText(L" View\n Col", 20);
		_uiBtnViewMode[eViewMode_Rect]->setText(L" View\n Rect", 20);
		_uiBtnViewMode[eViewMode_Rect]->setState(eButton_Down);
		_uiBtnViewMode[eViewMode_InfoText]->setText(L" View \n Text", 20);
		_uiBtnViewMode[eViewMode_InfoText]->setState(eButton_Down);
		_uiBtnViewMode[eViewMode_HideImage]->setText(L" Hide \n Img", 20);
	}

	// 지형 위치 조절
	{
		// reset
		_uiBtnMoveReset = new uiButton;
		_uiBtnMoveReset->init("uiBG3", "uiBG"
						 ,hieracy.right + UI_SPACE + 50.f + UI_SPACE
						 ,hieracy.bottom + UI_SPACE + 50.f + UI_SPACE
						 ,50.f, 50.f);
		_uiBtnMoveReset->setText(L"Reset", 20);
		_uiBtnMoveReset->setOnClickFunction(std::bind(&mapTool::clickBtnMoveReset, this));
		insertUIObject(_uiBtnMoveReset);

		// 상
		_uiBtnMoveUp = new uiButton;
		_uiBtnMoveUp->init("uiBG3", "uiBG"
						  , _uiBtnMoveReset->getRect().left
						  , _uiBtnMoveReset->getRect().top - UI_SPACE - 50.f
						  , 50.f, 50.f);
		_uiBtnMoveUp->setText(L" ↑", 30);
		_uiBtnMoveUp->setOnClickFunction(std::bind(&mapTool::clickBtnMoveUp, this));
		insertUIObject(_uiBtnMoveUp);
		
		// 좌상
		_uiBtnMoveLeftUp = new uiButton;
		_uiBtnMoveLeftUp->init("uiBG3", "uiBG"
							   , _uiBtnMoveUp->getRect().left - UI_SPACE - 50.f
							   , _uiBtnMoveUp->getRect().top
							   , 50.f, 50.f);
		_uiBtnMoveLeftUp->setText(L" ↖", 30);
		_uiBtnMoveLeftUp->setOnClickFunction(std::bind(&mapTool::clickBtnMoveLeftUp, this));
		insertUIObject(_uiBtnMoveLeftUp);
		
		// 우상
		_uiBtnMoveRightUp = new uiButton;
		_uiBtnMoveRightUp->init("uiBG3", "uiBG"
								, _uiBtnMoveUp->getRect().right + UI_SPACE
								, _uiBtnMoveUp->getRect().top
								, 50.f, 50.f);
		_uiBtnMoveRightUp->setText(L" ↗", 30);
		_uiBtnMoveRightUp->setOnClickFunction(std::bind(&mapTool::clickBtnMoveRightUp, this));
		insertUIObject(_uiBtnMoveRightUp);
		
		// 하
		_uiBtnMoveDown = new uiButton;
		_uiBtnMoveDown->init("uiBG3", "uiBG"
							 , _uiBtnMoveReset->getRect().left
							 , _uiBtnMoveReset->getRect().bottom + UI_SPACE
							 , 50.f, 50.f);
		_uiBtnMoveDown->setText(L" ↓", 30);
		_uiBtnMoveDown->setOnClickFunction(std::bind(&mapTool::clickBtnMoveDown, this));
		insertUIObject(_uiBtnMoveDown);
		
		// 좌하
		_uiBtnMoveLeftUp = new uiButton;
		_uiBtnMoveLeftUp->init("uiBG3", "uiBG"
							   , _uiBtnMoveDown->getRect().left - UI_SPACE - 50.f
							   , _uiBtnMoveDown->getRect().top
							   , 50.f, 50.f);
		_uiBtnMoveLeftUp->setText(L" ↙", 30);
		_uiBtnMoveLeftUp->setOnClickFunction(std::bind(&mapTool::clickBtnMoveLeftDown, this));
		insertUIObject(_uiBtnMoveLeftUp);
		
		// 우하
		_uiBtnMoveRightDown = new uiButton;
		_uiBtnMoveRightDown->init("uiBG3", "uiBG"
								  , _uiBtnMoveDown->getRect().right + UI_SPACE
								  , _uiBtnMoveDown->getRect().top
								  , 50.f, 50.f);
		_uiBtnMoveRightDown->setText(L" ↘", 30);
		_uiBtnMoveRightDown->setOnClickFunction(std::bind(&mapTool::clickBtnMoveRightDown, this));
		insertUIObject(_uiBtnMoveRightDown);
		
		// 좌
		_uiBtnMoveLeft = new uiButton;
		_uiBtnMoveLeft->init("uiBG3", "uiBG"
							 , _uiBtnMoveReset->getRect().left - UI_SPACE - 50.f
							 , _uiBtnMoveReset->getRect().top
							 , 50.f, 50.f);
		_uiBtnMoveLeft->setText(L" ←", 30);
		_uiBtnMoveLeft->setOnClickFunction(std::bind(&mapTool::clickBtnMoveLeft, this));
		insertUIObject(_uiBtnMoveLeft);
		
		// 우
		_uiBtnMoveRight = new uiButton;
		_uiBtnMoveRight->init("uiBG3", "uiBG"
							  , _uiBtnMoveReset->getRect().right + UI_SPACE
							  , _uiBtnMoveReset->getRect().top
							  , 50.f, 50.f);
		_uiBtnMoveRight->setText(L" →", 30);
		_uiBtnMoveRight->setOnClickFunction(std::bind(&mapTool::clickBtnMoveRight, this));
		insertUIObject(_uiBtnMoveRight);
		
		_uiTextMoveSnap = new uiText;
		_uiTextMoveSnap->init(_uiBtnMoveRight->getRect().right + UI_SPACE
							  , _uiBtnMoveRight->getRect().top);
		_uiTextMoveSnap->setFontSize(20);
		_uiTextMoveSnap->setFontColor(RGB(255, 255, 255));
		_uiTextMoveSnap->setText(format(L"\n  %d", _moveSnap));
		insertUIObject(_uiTextMoveSnap);
		
		_uiBtnMoveSnapPlus = new uiButton;
		_uiBtnMoveSnapPlus->init("uiBG3", "uiBG"
								 , _uiTextMoveSnap->getWorldPosition().x
								 , _uiTextMoveSnap->getWorldPosition().y - UI_SPACE - 50.f
								 , 50.f, 50.f);
		_uiBtnMoveSnapPlus->setText(L" +", 20);
		_uiBtnMoveSnapPlus->setOnClickFunction(std::bind(&mapTool::clickBtnMoveSnapPlus, this));
		insertUIObject(_uiBtnMoveSnapPlus);
		
		_uiBtnMoveSnapMinus = new uiButton;
		_uiBtnMoveSnapMinus->init("uiBG3", "uiBG"
								  , _uiTextMoveSnap->getWorldPosition().x
								  , _uiTextMoveSnap->getWorldPosition().y + UI_SPACE + 50.f
								  , 50.f, 50.f);
		_uiBtnMoveSnapMinus->setText(L" -", 20);
		_uiBtnMoveSnapMinus->setOnClickFunction(std::bind(&mapTool::clickBtnMoveSnapMinus, this));
		insertUIObject(_uiBtnMoveSnapMinus);
	}

	// 저장&로드
	{
		// 현재 열린 파일명 
		_uiTextCurFileName = new uiText;
		_uiTextCurFileName->init(miniMap.right + UI_SPACE, miniMap.top);
		_uiTextCurFileName->setFontColor(RGB(255, 255, 255));
		_uiTextCurFileName->setFontSize(25);
		_uiTextCurFileName->setText(L"[File Name : None]");
		insertUIObject(_uiTextCurFileName);

		// 파일 리스트
		_uiListFiles = new uiList;
		_uiListFiles->init(	miniMap.right + UI_SPACE
						   ,miniMap.top + UI_SPACE + 30.f
						   ,canvas.right - 50.f - UI_SPACE - miniMap.right
						   ,miniMap.bottom - miniMap.top
						   ,IMAGEMANAGER->findImage("uiBG5"));
		_uiListFiles->setCellHeight(30.f);
		_uiListFiles->setCountPerLine(3);
		_uiListFiles->setGap(5.f, 5.f);

		insertUIObject(_uiListFiles);

		// 파일 버튼들
		// "uiBG3", "uiBG", "uiBG5"
		string fileName;
		float x = 0.f;
		float y = 0.f;
		for ( int ii = 0; ii < eSceneName_Count; ++ii )
		{
			fileName.clear();
			fileName.append(SCENEMANAGER->getSceneFileName((eSceneName)ii));

			if ( 0 < fileName.size() )
			{
				uiButton* btn = new uiButton;
				btn->init("uiBG3", "uiBG", "uiBG5"
						  ,0.f, 0.f, 0.f, 0.f);
				btn->setText(string2wstring(fileName), 30.f);
				btn->setOnClickFunction(std::bind(&mapTool::clickBtnSelectFile, this, (eSceneName)ii, btn));
				_uiListFiles->insertChild(btn);
			}
		}

		// 저장 버튼
		_uiBtnSave = new uiButton;
		_uiBtnSave->init("uiBG3", "uiBG"
						 ,_uiListFiles->getRect().right + UI_SPACE
						 ,_uiListFiles->getRect().top
						 , 50.f, 50.f);
		_uiBtnSave->setText(L"Save", 20);
		_uiBtnSave->setOnClickFunction(std::bind(&mapTool::clickBtnSaveMap, this));
		insertUIObject(_uiBtnSave);

		// 로드 버튼
		_uiBtnLoad = new uiButton;
		_uiBtnLoad->init("uiBG3", "uiBG"
						 ,_uiBtnSave->getRect().left
						 ,_uiBtnSave->getRect().bottom + UI_SPACE
						 , 50.f, 50.f);
		_uiBtnLoad->setText(L"Load", 20);
		_uiBtnLoad->setOnClickFunction(std::bind(&mapTool::clickBtnLoadMap, this));
		insertUIObject(_uiBtnLoad);
	}


	// 속성창 & 버튼
	{
		float width = _sampleBoardCloseX - _qickOpen->getWidth() - (hieracy.right + UI_SPACE) - UI_SPACE;
		_uiPanelInspector = new uiPanel;
		_uiPanelInspector->init( hieracy.right + UI_SPACE, UI_SPACE, width, width
								,IMGDATABASE->getImage(eImage_UI_BG5));
		
		_uiTextInspector = new uiText;
		_uiTextInspector->init(hieracy.right + UI_SPACE, UI_SPACE);
		_uiTextInspector->setText(L"\tAttribute");
		_uiTextInspector->setFontSize(20);
		_uiTextInspector->setFontColor(RGB(255,255,255));
		insertUIObject(_uiTextInspector);

		wstring txts[eAttr_Count] = {L"Collide"
									,L"Trigger"
									,L"Breakable"
									,L"Usable"
									,L"Trap"
									,L"Portal"
									,L"Dialog" };
		for (int ii = 0; ii < eAttr_Count; ++ii)
		{
			_uiBtnInspectors[ii] = new uiButton;
			_uiBtnInspectors[ii]->init("uiBG2", "uiBG3", "uiBG", 5.f, 25.f + (ii * 25.f), width - 10.f, 20.f);
			_uiBtnInspectors[ii]->setText(txts[ii], 15);
			_uiBtnInspectors[ii]->setOnClickFunction(std::bind(&mapTool::clickBtnInspector, this, (eAttribute)ii, _uiBtnInspectors[ii]));
			_uiBtnInspectors[ii]->setOnClickUPFunction(std::bind(&mapTool::clickUpBtnInspector, this, (eAttribute)ii));
			_uiPanelInspector->insertChild(_uiBtnInspectors[ii]);
		}
		
		insertUIObject(_uiPanelInspector);


		RECTD2D inspector = _uiPanelInspector->getRect();
		_uiPanelInspectorInfo = new uiPanel;
		_uiPanelInspectorInfo->init( inspector.left, inspector.bottom + UI_SPACE
									,_uiPanelInspector->getWidth(), hieracy.bottom - (inspector.bottom + UI_SPACE)
									,IMGDATABASE->getImage(eImage_UI_BG5));

		_uiTextInspectorSub = new uiText;
		_uiTextInspectorSub->init(inspector.left + UI_SPACE, _uiPanelInspectorInfo->getRect().top + UI_SPACE);
		_uiTextInspectorSub->setText(L"\tDetail\n--------------------------");
		_uiTextInspectorSub->setFontSize(20);
		_uiTextInspectorSub->setFontColor(RGB(255, 255, 255));
		insertUIObject(_uiTextInspectorSub);

		_uiTextInspectorSubInfo = new uiText;
		_uiTextInspectorSubInfo->init(inspector.left + UI_SPACE, _uiPanelInspectorInfo->getRect().top + 60);
		_uiTextInspectorSubInfo->setText(L"None");
		_uiTextInspectorSubInfo->setFontSize(20);
		_uiTextInspectorSubInfo->setFontColor(RGB(255, 255, 255));
		insertUIObject(_uiTextInspectorSubInfo);
		
		insertUIObject(_uiPanelInspectorInfo);
	}

	{
		uiPanel* uiImg = new uiPanel;
		uiImg->init(miniMap.right, canvas.bottom, WINSIZEX - miniMap.right, WINSIZEY - canvas.bottom, uiBG);
		insertUIObject(uiImg);
		uiImg = nullptr;
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
			uiImg->init(0.f, 0.f, static_cast<float>(WINSIZEX), UI_SPACE, uiBG);
			insertUIObject(uiImg);
			uiImg = nullptr;
		}
		{
			uiPanel* uiImg = new uiPanel;
			uiImg->init(0.f, 0.f, UI_SPACE, static_cast<float>(WINSIZEY), uiBG);
			insertUIObject(uiImg);
			uiImg = nullptr;
		}
		{
			uiPanel* uiImg = new uiPanel;
			uiImg->init(canvas.right, 0.f,  WINSIZEX - canvas.right, static_cast<float>(WINSIZEY), uiBG);
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

void mapTool::initTerrainImgLinks()
{
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_Floor, false);
		_imgLnksTerrain.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_BG, false);
		_imgLnksTerrain.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_BG_Big01, false);
		_imgLnksTerrain.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_BG_Big02, false);
		_imgLnksTerrain.push_back(lnk);
		lnk = nullptr;
	}

	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_Ruddle, false);
		_imgLnksTerrain.push_back(lnk);
		lnk = nullptr;
	}

	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Fence, false);
		_imgLnksTerrain.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_Well, false);
		_imgLnksTerrain.push_back(lnk);
		lnk = nullptr;
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_StreetLamp, false);
		_imgLnksTerrain.push_back(lnk);
		lnk = nullptr;
	}
}

void mapTool::initObjectImgLinks()
{
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_Build, true);
		for (int ii = eImage_Town_Build01; ii <= eImage_Town_Build07; ++ii)
		{
			lnk->pushBack((eImageUID)ii);
		}
		_imgLnksObject.push_back(lnk);
		lnk = nullptr;
	}

	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Object_Chair, false);
		_imgLnksObject.push_back(lnk);
		lnk = nullptr;
	}
}

void mapTool::initNpcImgLinks()
{
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Npc_All, true);
		for ( int ii = eImage_Npc_Elderbug; ii <= eImage_Npc_TheLastStag; ++ii )
			lnk->pushBack((eImageUID)ii);
		_imgLnksNpc.push_back(lnk);
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
		if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000))
			_pick.clear();
	}
}

void mapTool::updateDrawCollider()
{
}

void mapTool::renderDrawTerrain()
{
	if(!_pick.isPick || !_sampleImg)
		return;
	
	float destX = _ptMouse.x - _pick.width / 2.f;
	float destY = _ptMouse.y - _pick.height / 2.f;
	_sampleImg->render(destX, destY, _pick.x, _pick.y
						   , static_cast<int>(_pick.width)
						   , static_cast<int>(_pick.height)
						   , 0.5f, true);
}

void mapTool::renderDrawCollider()
{
}

void mapTool::renderDrawObject()
{
	if (!_pick.isPick || !_sampleImg)
		return;

	float destX = _ptMouse.x - _pick.width / 2.f;
	float destY = _ptMouse.y - _pick.height / 2.f;
	
	IMGDATABASE->getImage(_pick.uid)->render(destX, destY, 0.5f, true);
}

void mapTool::renderDrawNpc()
{
	if (!_pick.isPick || !_sampleImg)
		return;

	float destX = _ptMouse.x - _pick.width / 2.f;
	float destY = _ptMouse.y - _pick.height / 2.f;
	
	IMGDATABASE->getImage(_pick.uid)->frameRender(destX, destY, 0, 0, 0.5f);
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

void mapTool::clickBtnTerrain(UID uid, uiButton* btn)
{
	terrain* ter = _mapData->getTerrain(_curLayer, uid);
	if (ter)
	{
		_select.infoSet(ter, ter->getAtrribute(), _curLayer);
		
		if (_curBtnTerrain)
		{
			if(_curBtnTerrain != btn)
				_curBtnTerrain->setState(eButton_Up);
		}
		_curBtnTerrain = btn;

		_pick.clear();

		// 상태창
		UINT attr = 0;
		for (int ii = 0; ii < eAttr_Count; ++ii)
		{
			if (checkAttribute(_select.attr, ii))
				_uiBtnInspectors[ii]->setState(eButton_Down);
			else
				_uiBtnInspectors[ii]->setState(eButton_Up);
		}

		// 카메라 밖에 있다면 카메라 이동
		RECTD2D rc = ter->getRect();
		if (!CAMERA->isRangeInCamera(rc.left, rc.top
									, static_cast<int>(rc.right - rc.left)
									, static_cast<int>(rc.bottom - rc.top)))
		{
			float offsetX = _canvas->getWidth() / 2.f;
			float offsetY = _canvas->getHeight() / 2.f;

			CAMERA->setPosX(rc.left - offsetX);
			CAMERA->setPosY(rc.top - offsetY);
		}
	}
	else
	{
		for (int ii = 0; ii < eAttr_Count; ++ii)
		{
			_uiBtnInspectors[ii]->setState(eButton_Up);
		}
	}

	_isTerrainReposition = false;
	refreshDetailText();
}

void mapTool::clickUpBtnTerrain(UID uid)
{
	_select.clear();
	_curBtnTerrain = nullptr;
	for (int ii = 0; ii < eAttr_Count; ++ii)
	{
		_uiBtnInspectors[ii]->setState(eButton_Up);
	}

	refreshDetailText();
}

void mapTool::clickBtnActor(UID uid, uiButton* btn)
{
}

void mapTool::clickUpBtnActor(UID uid)
{
}

void mapTool::clickBtnInspector(eAttribute attr, uiButton* btn)
{
	if (_select.obj)
	{
		_mapData->addTerrainAttribute(_select.layer, _select.obj->getUID(), attr);
	}
	else
	{
		btn->setState(eButton_Up);
	}

	refreshDetailText();
}

void mapTool::clickUpBtnInspector(eAttribute attr)
{
	if (_select.obj)
	{
		_mapData->removeTerrainAttribute(_select.layer, _select.obj->getUID(), attr);
		refreshDetailText();
	}
}


void mapTool::clickBtnUpNone(uiButton * btn)
{
	btn->setState(eButton_Down);
}

void mapTool::clickBtnDelTerrain()
{
	if (_select.isSet)
	{
		if (_select.obj)
		{
			_mapData->deleteTerrain(_select.layer, _select.obj->getUID());
			_uiListHierarcy[_select.layer]->removeChild(_curBtnTerrain);

			_curBtnTerrain = nullptr;

			_select.clear();
			refreshDetailText();
		}
	}
}

void mapTool::clickBtnUpIndex()
{
	if (!_select.isSet || !_select.obj)
	{
		_select.clear();
		return;
	}

	int idx = _mapData->getTerrainIndex(_curLayer, _select.obj->getUID());
	if (0 < idx)
	{
		_mapData->terrainUp(_select.layer, _select.obj->getUID());

		_uiListHierarcy[_curLayer]->removeChildAll();
		vector<terrain*>* vTerrains = _mapData->getLayerTerrains(_curLayer);
		
		vector<terrain*>::iterator iter = vTerrains->begin();
		vector<terrain*>::iterator end = vTerrains->end();
		
		for (; iter != end; ++iter)
		{
			terrain* ter = (*iter);
			uiButton* btn = new uiButton;
		
			btn->init("uiBG2", "uiBG3", "uiBG", 0.f, 0.f, 10.f, 10.f);
			btn->setText(format(L"%d", ter->getUID()));
			btn->setOnClickFunction(std::bind(&mapTool::clickBtnTerrain, this, ter->getUID(), btn));
			btn->setOnClickUPFunction(std::bind(&mapTool::clickUpBtnTerrain, this, ter->getUID()));
		
			_uiListHierarcy[_curLayer]->insertChild(btn);

			if (ter->getUID() == _select.obj->getUID())
			{
				_curBtnTerrain = btn;
				_curBtnTerrain->setState(eButton_Down);
			}
		}
	}

	refreshDetailText();
}

void mapTool::clickBtnDownIndex()
{
	if (!_select.isSet || !_select.obj)
	{
		_select.clear();
		return;
	}

	int idx = _mapData->getTerrainIndex(_curLayer, _select.obj->getUID());
	if (0 < idx)
	{
		_mapData->terrainDown(_select.layer, _select.obj->getUID());

		_uiListHierarcy[_curLayer]->removeChildAll();
		vector<terrain*>* vTerrains = _mapData->getLayerTerrains(_curLayer);

		if (idx < vTerrains->size() - 1)
		{
			vector<terrain*>::iterator iter = vTerrains->begin();
			vector<terrain*>::iterator end = vTerrains->end();

			for (; iter != end; ++iter)
			{
				terrain* ter = (*iter);
				uiButton* btn = new uiButton;

				btn->init("uiBG2", "uiBG3", "uiBG", 0.f, 0.f, 10.f, 10.f);
				btn->setText(format(L"%d", ter->getUID()));
				btn->setOnClickFunction(std::bind(&mapTool::clickBtnTerrain, this, ter->getUID(), btn));
				btn->setOnClickUPFunction(std::bind(&mapTool::clickUpBtnTerrain, this, ter->getUID()));

				_uiListHierarcy[_curLayer]->insertChild(btn);

				if (ter->getUID() == _select.obj->getUID())
				{
					_curBtnTerrain = btn;
					_curBtnTerrain->setState(eButton_Down);
				}
			}
		}
	}

	refreshDetailText();
}

void mapTool::clickBtnChangeLayer(UINT layer)
{
	if (!_select.isSet || !_select.obj)
	{
		_select.clear();
		return;
	}

	if(layer == _select.layer)
		return;

	_mapData->changeLayer(_select.layer, layer, _select.obj->getUID());

	_uiListHierarcy[_select.layer]->removeChildAll();
	vector<terrain*>* vTerrains = _mapData->getLayerTerrains(_select.layer);
	vector<terrain*>::iterator iter = vTerrains->begin();
	vector<terrain*>::iterator end = vTerrains->end();

	for (; iter != end; ++iter)
	{
		terrain* ter = (*iter);
		uiButton* btn = new uiButton;

		btn->init("uiBG2", "uiBG3", "uiBG", 0.f, 0.f, 10.f, 10.f);
		btn->setText(format(L"%d", ter->getUID()));
		btn->setOnClickFunction(std::bind(&mapTool::clickBtnTerrain, this, ter->getUID(), btn));
		btn->setOnClickUPFunction(std::bind(&mapTool::clickUpBtnTerrain, this, ter->getUID()));

		_uiListHierarcy[_select.layer]->insertChild(btn);

		if (ter->getUID() == _select.obj->getUID())
		{
			_curBtnTerrain = btn;
			_curBtnTerrain->setState(eButton_Down);

			_select.layer = (eLayer)layer;
		}
	}

	_uiListHierarcy[layer]->removeChildAll();
	vTerrains = _mapData->getLayerTerrains(layer);
	iter = vTerrains->begin();
	end = vTerrains->end();

	for (; iter != end; ++iter)
	{
		terrain* ter = (*iter);
		uiButton* btn = new uiButton;

		btn->init("uiBG2", "uiBG3", "uiBG", 0.f, 0.f, 10.f, 10.f);
		btn->setText(format(L"%d", ter->getUID()));
		btn->setOnClickFunction(std::bind(&mapTool::clickBtnTerrain, this, ter->getUID(), btn));
		btn->setOnClickUPFunction(std::bind(&mapTool::clickUpBtnTerrain, this, ter->getUID()));

		_uiListHierarcy[layer]->insertChild(btn);

		if (ter->getUID() == _select.obj->getUID())
		{
			_curBtnTerrain = btn;
			_curBtnTerrain->setState(eButton_Down);
			_select.layer = (eLayer)layer;
		}
	}

	clickBtnHierarcy((eLayer)layer);
	_uiBtnHierarcy[layer]->setState(eButton_Down);
}

void mapTool::clickBtnViewMode(eViewMode mode)
{
	switch (mode)
	{
		case eViewMode_Layer:		{ _isShowAllLayer = false; break; 					 }
		case eViewMode_Collision:	{ DEVTOOL->setDebugMode(DEBUG_SHOW_COLLISON); break; }
		case eViewMode_Rect:		{ DEVTOOL->setDebugMode(DEBUG_SHOW_RECT); break;	 }
		case eViewMode_InfoText:	
		{
			DEVTOOL->setDebugMode(DEBUG_SHOW_POSITION); 
			DEVTOOL->setDebugMode(DEBUG_SHOW_UID); 
			break; 
		}
		case eViewMode_HideImage:	{ DEVTOOL->setDebugMode(DEBUG_HIDE_IMAGE); break;	 }
	}
}

void mapTool::clickBtnUpViewMode(eViewMode mode)
{
	switch (mode)
	{
		case eViewMode_Layer:		{ _isShowAllLayer = true; break;					 }
		case eViewMode_Collision:	{ DEVTOOL->delDebugMode(DEBUG_SHOW_COLLISON); break; }
		case eViewMode_Rect:		{ DEVTOOL->delDebugMode(DEBUG_SHOW_RECT); break;	 }
		case eViewMode_InfoText:
		{
			DEVTOOL->delDebugMode(DEBUG_SHOW_POSITION);
			DEVTOOL->delDebugMode(DEBUG_SHOW_UID);
			break;
		}
		case eViewMode_HideImage:	{ DEVTOOL->delDebugMode(DEBUG_HIDE_IMAGE); break;	 }
	}
}

void mapTool::clickBtnMoveReset()
{
	if (checkSelectingTerrain())
	{
		startMoveTerrain();

		_select.obj->setPosition(_originPos.x, _originPos.y);
		_isTerrainReposition = false;
	}
}

void mapTool::clickBtnMoveUp()
{
	if (checkSelectingTerrain())
	{
		startMoveTerrain();

		POINTF pf = { _select.obj->getPosX(), _select.obj->getPosY() };
		pf.y -= _moveSnap;
		_select.obj->setPosition(pf.x, pf.y);
	}
}

void mapTool::clickBtnMoveDown()
{
	if (checkSelectingTerrain())
	{
		startMoveTerrain();

		POINTF pf = { _select.obj->getPosX(), _select.obj->getPosY() };
		pf.y += _moveSnap;
		_select.obj->setPosition(pf.x, pf.y);
	}
}

void mapTool::clickBtnMoveLeft()
{
	if (checkSelectingTerrain())
	{
		startMoveTerrain();

		POINTF pf = { _select.obj->getPosX(), _select.obj->getPosY() };
		pf.x -= _moveSnap;
		_select.obj->setPosition(pf.x, pf.y);
	}
}

void mapTool::clickBtnMoveRight()
{
	if (checkSelectingTerrain())
	{
		startMoveTerrain();

		POINTF pf = { _select.obj->getPosX(), _select.obj->getPosY() };
		pf.x += _moveSnap;
		_select.obj->setPosition(pf.x, pf.y);
	}
}

void mapTool::clickBtnMoveLeftUp()
{
	if (checkSelectingTerrain())
	{
		startMoveTerrain();

		POINTF pf = { _select.obj->getPosX(), _select.obj->getPosY() };
		pf.x -= _moveSnap;
		pf.y -= _moveSnap;
		_select.obj->setPosition(pf.x, pf.y);
	}
}

void mapTool::clickBtnMoveLeftDown()
{
	if (checkSelectingTerrain())
	{
		startMoveTerrain();

		POINTF pf = { _select.obj->getPosX(), _select.obj->getPosY() };
		pf.x -= _moveSnap;
		pf.y += _moveSnap;
		_select.obj->setPosition(pf.x, pf.y);
	}
}

void mapTool::clickBtnMoveRightUp()
{
	if (checkSelectingTerrain())
	{
		startMoveTerrain();

		POINTF pf = { _select.obj->getPosX(), _select.obj->getPosY() };
		pf.x += _moveSnap;
		pf.y -= _moveSnap;
		_select.obj->setPosition(pf.x, pf.y);
	}
}

void mapTool::clickBtnMoveRightDown()
{
	if (checkSelectingTerrain())
	{
		startMoveTerrain();

		POINTF pf = { _select.obj->getPosX(), _select.obj->getPosY() };
		pf.x += _moveSnap;
		pf.y += _moveSnap;
		_select.obj->setPosition(pf.x, pf.y);
	}
}

void mapTool::clickBtnMoveSnapPlus()
{
	++_moveSnap;
	_uiTextMoveSnap->setText(format(L"\n  %d", _moveSnap));
}

void mapTool::clickBtnMoveSnapMinus()
{
	--_moveSnap;
	if(_moveSnap <= 0)
		_moveSnap = 1;

	_uiTextMoveSnap->setText(format(L"\n  %d", _moveSnap));
}

void mapTool::refreshDetailText()
{
	wstring txt;
	txt.clear();


	if (!_select.isSet || !_select.obj)
	{
		_select.clear();
		txt.append(L"None");
	}
	else
	{
		txt.append(format(L"[UID : %d] \n", _select.obj->getUID()));
		for (int ii = 0; ii < eAttr_Count; ++ii)
		{
			if (!checkAttribute(_select.attr, ii))
				continue;

			switch (ii)
			{
				case eAttr_Collide:			{ txt.append(L"Collide\n");break; }
				case eAttr_Trigger:			
				{
					int uid = _mapData->getTriggerIndex(_select.obj->getUID());
					txt.append(format(L"Trigger : %d\n", uid));
					break; 
				}
				case eAttr_Breakable:		{ txt.append(L"Breakable\n");break; }
				case eAttr_Usable:			{ txt.append(L"Usable\n");break; }
				case eAttr_Trap:			{ txt.append(L"Trap\n");break; }
				case eAttr_Portal:			
				{
					eSceneName name = eSceneName_None;
					txt.append(format(L"Portal : %d\n", name));
					break; 
				}
				case eAttr_Dialog:			{ txt.append(L"Dialog");break; }

				default:
					break;
			}
		}
	}

	_uiTextInspectorSubInfo->setText(txt);
}

bool mapTool::checkSelectingTerrain()
{
	if (!_select.isSet || !_select.obj)
	{
		_select.clear();
		return false;
	}

	return true;
}

void mapTool::startMoveTerrain()
{
	if (!_isTerrainReposition)
	{
		_originPos = { _select.obj->getPosX(), _select.obj->getPosY() };
		_isTerrainReposition = true;
	}
}

void mapTool::clickBtnSelectFile(eSceneName sceneName, uiButton* btn)
{
	_selFileName = sceneName;
	_selBtnFileName = btn;
}

void mapTool::clickBtnSaveMap()
{
	if( eSceneName_None == _selFileName )
		return;

	string fileNamestr = SCENEMANAGER->getSceneFileName(_selFileName);
	wstring fileName;
	fileName.clear();
	fileName.append(string2wstring(fileNamestr));
	_uiTextCurFileName->setText(format(L"[File Name : %s]", fileName.c_str()));

	_mapData->save(fileNamestr);

	_selFileName = eSceneName_None;
	if(_selBtnFileName)
		_selBtnFileName->setState(eButton_Up);
}

void mapTool::clickBtnLoadMap()
{
	if(eSceneName_None == _selFileName)
		return;

	_select.clear();

	_curFileName = _selFileName;
	
	string fileNamestr = SCENEMANAGER->getSceneFileName(_curFileName);
	wstring fileName;
	fileName.clear();
	fileName.append(string2wstring(fileNamestr));
	_uiTextCurFileName->setText(format(L"[File Name : %s]", fileName.c_str()));

	_mapData->load(fileNamestr);


	for(int layer = 0 ; layer < eLayer_Count; ++layer )
	{
		_uiListHierarcy[layer]->removeChildAll();
		vector<terrain*>* vTerrains = _mapData->getLayerTerrains(layer);

		vector<terrain*>::iterator iter = vTerrains->begin();
		vector<terrain*>::iterator end = vTerrains->end();

		for (; iter != end; ++iter)
		{
			terrain* ter = (*iter);
			uiButton* btn = new uiButton;

			btn->init("uiBG2", "uiBG3", "uiBG", 0.f, 0.f, 10.f, 10.f);
			btn->setText(format(L"%d", ter->getUID()));
			btn->setOnClickFunction(std::bind(&mapTool::clickBtnTerrain, this, ter->getUID(), btn));
			btn->setOnClickUPFunction(std::bind(&mapTool::clickUpBtnTerrain, this, ter->getUID()));

			_uiListHierarcy[layer]->insertChild(btn);
		}
	}

	_selFileName = eSceneName_None;
	if(_selBtnFileName)
		_selBtnFileName->setState(eButton_Up);
}
