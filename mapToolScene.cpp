#include "stdafx.h"
#include "mapToolScene.h"

#include "mapTool.h"
#include "button.h"

mapToolScene::mapToolScene()
: _tool(nullptr)
, _beforeSample(nullptr)
, _nextSample(nullptr)
, _sampleBoardOpenX(WINSIZEX - SAMPLABOARD_WIDTH)
, _sampleBoardCloseX(WINSIZEX - 5.f)
, _sampleBoardSpeed(10.f)
, _isOpenSampleBoard(false)

{
	_sampleBoard = {};
}


mapToolScene::~mapToolScene()
{
}

HRESULT mapToolScene::init()
{
	_sceneName = eSceneName_MapTool;

	_uiBG[0] = IMAGEMANAGER->findImage("uiBG");
	_uiBG[1] = IMAGEMANAGER->findImage("uiBG2");
	_uiBG[2] = IMAGEMANAGER->findImage("uiBG3");
	_uiBG[3] = IMAGEMANAGER->findImage("uiBG4");
	_uiBG[4] = IMAGEMANAGER->findImage("uiBG5");
	
	_tool = new mapTool;
	_tool->init();

	
	_sampleBoard = { _sampleBoardCloseX, 5.f, _sampleBoardCloseX + SAMPLABOARD_WIDTH, WINSIZEY - 5.f };
	_canvers = { UI_SPACE, UI_SPACE, WINSIZEX / 3.f * 2.f - UI_SPACE, WINSIZEY / 5.f * 4.f - UI_SPACE};
	_miniMap = {_canvers.left, _canvers.bottom + UI_SPACE, _canvers.left + (MAPSIZEX * 0.019f), _canvers.bottom + UI_SPACE + (MAPSIZEY * 0.019f)};
	_fileZone = {_miniMap.right + UI_SPACE, _miniMap.top, _canvers.right - 100.f, _miniMap.bottom};
	_hierarchy = {_canvers.right + UI_SPACE, _canvers.top, _canvers.right + 400.f, _canvers.bottom - 300.f};
	_inspector = {_hierarchy.right + UI_SPACE, _hierarchy.top, _sampleBoardCloseX - UI_SPACE - 20.f, _hierarchy.bottom};

	_tool->setCanversRect(&_canvers);
	_tool->setSampleBoardRect(&_sampleBoard);

	_miniScopeWidth = (_canvers.right - _canvers.left) * (_miniMap.right - _miniMap.left) / MAPSIZEX;
	_miniScopeHeight = (_canvers.bottom - _canvers.top) *  (_miniMap.bottom - _miniMap.top)  / MAPSIZEY;

	_miniScope = {_miniMap.left, _miniMap.top, _miniMap.left + _miniScopeWidth, _miniMap.top + _miniScopeHeight};

	_beforeSample = new button;
	_nextSample = new button;
	_qickOpen = new button;

	_beforeSample->init( "uiBG3", "uiBG"
						, _sampleBoard.left + (3.f * UI_SPACE), _sampleBoard.bottom - 100.f
						, 60.0f, 50.0f, bind(&mapTool::beforeSample, _tool));
	_beforeSample->setText(L"<<", 50);

	_nextSample->init(  "uiBG3", "uiBG"
						, _beforeSample->getRect().right + UI_SPACE
						, _beforeSample->getRect().top
						, 60.f, 50.f
						, bind(&mapTool::nextSample, _tool));
	_nextSample->setText(L">>", 50);

	_qickOpen->init( "uiBG3", "uiBG", "uiBG2"
					 , _sampleBoard.left - 20.f, _sampleBoard.top
					 , 20.f, _sampleBoard.bottom - _sampleBoard.top
					, bind(&mapToolScene::openSampleBoard, this)
					, bind(&mapToolScene::closeSampleBoard, this));
	_qickOpen->setText(L"◀\n◀\n◀", 20);
	_qickOpen->setText(L"▶\n▶\n▶", eButton_Down, 20);

	CAMERA->setScope(_canvers);

	return S_OK;
}

void mapToolScene::release()
{
	SAFE_RELEASE(_tool);
	SAFE_DELETE(_tool);
}

void mapToolScene::update()
{
	_qickOpen->update();
	_beforeSample->update();
	_nextSample->update();

	if (PtInRectD2D(_sampleBoard, _ptMouse))
	{
		_isMoveMiniScope = false;
		openningSampleBoard();
	}
	else 
	{
		if(!_tool->isSamplePicking())
			closingSampleBoard();
	}

	if (_tool)
		_tool->update();


	if (PtInRectD2D(_miniMap, _ptMouse))
	{
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			_isMoveMiniScope = true;
		}
	}

	if (_isMoveMiniScope)
	{
		if(!KEYMANAGER->isStayKeyDown(VK_LBUTTON))
			_isMoveMiniScope = false;
		else
		{
			_miniScope = { _ptMouse.x - _miniScopeWidth / 2.f, _ptMouse.y - _miniScopeHeight / 2.f
						 , _ptMouse.x + _miniScopeWidth / 2.f, _ptMouse.y + _miniScopeHeight / 2.f };

			if (_miniScope.left < _miniMap.left)
			{
				_miniScope.left = _miniMap.left;
				_miniScope.right  = _miniMap.left + _miniScopeWidth;
			}
			else if (_miniMap.right < _miniScope.right)
			{
				_miniScope.left = _miniMap.right - _miniScopeWidth;
				_miniScope.right = _miniMap.right;
			}

			if (_miniScope.top < _miniMap.top)
			{
				_miniScope.top = _miniMap.top;
				_miniScope.bottom = _miniMap.top + _miniScopeHeight;

				_miniScope = { _miniScope.left, _miniMap.top
							  ,_miniScope.right, _miniMap.top + _miniScopeHeight };
			}
			else if (_miniMap.bottom < _miniScope.bottom)
			{
				_miniScope.top = _miniMap.bottom - _miniScopeHeight;
				_miniScope.bottom = _miniMap.bottom;
			}

			float destX = (((_miniScope.left - _miniMap.left) / (_miniMap.right - _miniMap.left)) * MAPSIZEX);
			float destY = (((_miniScope.top  - _miniMap.top)/ (_miniMap.bottom - _miniMap.top)) * MAPSIZEY);

			CAMERA->setPosX(destX);
			CAMERA->setPosY(destY);
		}
	}
}

void mapToolScene::render()
{
	// canvers : 실제 맵
	D2DMANAGER->drawRectangle(_canvers);
	_tool->terrainRender();
	
	// canver 아래
	_uiBG[0]->render(_canvers.left, _canvers.bottom, _canvers.right, WINSIZEY, 1.f);
	
	// minimap
	_uiBG[4]->render(_miniMap, 1.0f);
	_tool->terrainRender(_miniMap.left, _miniMap.top, 0.019f);
	D2DMANAGER->drawRectangle(_miniMap);
	D2DMANAGER->drawRectangle(_miniScope);
	
	// canver 상 좌 우
	_uiBG[0]->render(0.f, 0.f, WINSIZEX, UI_SPACE, 1.f);
	_uiBG[0]->render(0.f, 0.f, UI_SPACE, WINSIZEY, 1.f);
	_uiBG[0]->render(_canvers.right, 0.f, WINSIZEX - _canvers.right, WINSIZEY, 1.f);

	// minimap 우 하
	_uiBG[0]->render(_miniMap.right, _canvers.bottom, WINSIZEX - _miniMap.right, WINSIZEY - _canvers.bottom, 1.f);
	_uiBG[0]->render(_miniMap.left, _miniMap.bottom, _miniMap.right - _miniMap.left, WINSIZEY - _miniMap.bottom, 1.f);

	// fileZone
	_uiBG[4]->render(_fileZone, 1.0f);

	// hierarchy
	_uiBG[4]->render(_hierarchy, 1.0f);

	// inspector
	_uiBG[4]->render(_inspector, 1.0f);

	// sampleBoard
	_uiBG[3]->render(_sampleBoard, 1.f);
	D2DMANAGER->drawRectangle(_sampleBoard);

	// 버튼
	_qickOpen->render();
	_nextSample->render();
	_beforeSample->render();

	// 툴
	_tool->render();

	//WCHAR str[128];
	//swprintf_s(str, L"%d %d", _isOpenSampleBoard, _isCloseSampleBoard);
	//D2DMANAGER->drawText(str
	//					 , WINSIZEX / 2
	//					 , WINSIZEY / 2
	//					 , 50, RGB(255, 255, 255));
}

void mapToolScene::closingSampleBoard()
{
	if (_sampleBoard.left < _sampleBoardCloseX)
	{
		_sampleBoard.left += _sampleBoardSpeed;
		_sampleBoard.right += _sampleBoardSpeed;

		_qickOpen->rePosition(_sampleBoard.left, _sampleBoard.top);
		_beforeSample->rePosition(_sampleBoard.left + (UI_SPACE * 3.f), _sampleBoard.bottom - 100.f);
		_nextSample->rePosition(_beforeSample->getRect().right + UI_SPACE, _beforeSample->getRect().top);

		_isOpenSampleBoard = false;
		_isCloseSampleBoard = false;
	}
	
	if (_sampleBoardCloseX <= _sampleBoard.left)
	{
		closeSampleBoard();
	}
}

void mapToolScene::openningSampleBoard(bool isSkip)
{
	if (_sampleBoardOpenX < _sampleBoard.left)
	{
		_sampleBoard.left -= _sampleBoardSpeed;
		_sampleBoard.right -= _sampleBoardSpeed;

		_qickOpen->rePosition(_sampleBoard.left, _sampleBoard.top);
		_beforeSample->rePosition(_sampleBoard.left +(UI_SPACE * 3.f), _sampleBoard.bottom - 100.f);
		_nextSample->rePosition(_beforeSample->getRect().right + UI_SPACE, _beforeSample->getRect().top);

		_isOpenSampleBoard = false;
		_isCloseSampleBoard = false;
	}
	
	if (_sampleBoard.left <= _sampleBoardOpenX)
	{
		openSampleBoard();
	}
}

void mapToolScene::openSampleBoard()
{
	if (!_isOpenSampleBoard)
	{
		_sampleBoard.left = _sampleBoardOpenX;
		_sampleBoard.right = _sampleBoardOpenX + SAMPLABOARD_WIDTH;

		_qickOpen->rePosition(_sampleBoard.left, _sampleBoard.top);
		_beforeSample->rePosition(_sampleBoard.left + (UI_SPACE * 3.f), _sampleBoard.bottom - 100.f);
		_nextSample->rePosition(_beforeSample->getRect().right + UI_SPACE, _beforeSample->getRect().top);

		_qickOpen->setState(eButton_Down);
		_isOpenSampleBoard = true;
		_isCloseSampleBoard = false;
	}
}

void mapToolScene::closeSampleBoard()
{
	if (!_isCloseSampleBoard)
	{
		_sampleBoard.left = _sampleBoardCloseX;
		_sampleBoard.right = _sampleBoard.left + SAMPLABOARD_WIDTH;

		_qickOpen->rePosition(_sampleBoard.left - 20.f, _sampleBoard.top);

		_beforeSample->rePosition(_sampleBoard.left + (UI_SPACE * 3.f), _sampleBoard.bottom - 100.f);
		_nextSample->rePosition(_beforeSample->getRect().right + UI_SPACE, _beforeSample->getRect().top);

		_qickOpen->setState(eButton_Up);

		_isOpenSampleBoard = false;
		_isCloseSampleBoard = true;
	}
}
