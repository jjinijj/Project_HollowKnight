#include "stdafx.h"
#include "mapToolScene.h"

#include "mapTool.h"
#include "uiButton.h"

mapToolScene::mapToolScene()
: _tool(nullptr)
, _beforeSample(nullptr)
, _nextSample(nullptr)
, _sampleBoardOpenX(WINSIZEX - SAMPLABOARD_WIDTH)
, _sampleBoardCloseX(WINSIZEX - 5.f)
, _sampleBoardSpeed(10.f)
, _isOpenSampleBoard(false)

{
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

	
	//_fileZone = {_miniMap.right + UI_SPACE, _miniMap.top, _canvas.right - 100.f, _miniMap.bottom};
	//_hierarchy = {_canvas.right + UI_SPACE, _canvas.top, _canvas.right + 400.f, _canvas.bottom - 300.f};
	//_inspector = {_hierarchy.right + UI_SPACE, _hierarchy.top, _sampleBoardCloseX - UI_SPACE - 20.f, _hierarchy.bottom};


	

	

	

	return S_OK;
}

void mapToolScene::release()
{
	SAFE_RELEASE(_tool);
	SAFE_DELETE(_tool);
}

void mapToolScene::update()
{
	if (!_isOpenSampleBoard && ! _isCloseSampleBoard)
	{
		if(eToolMode_DrawCollider != _tool->getToolMode())
			_createCol->setState(eButton_Up);
	}

	// 툴에서 드래그 중이 아닐 경우에만 버튼 활성화 
	if (!_tool->isPicking())
	{
		_qickOpen->update();
		_beforeSample->update();
		_nextSample->update();
		_createCol->update();
	}

	if (PtInRectD2D(_sampleBoard, _ptMouse))
	{
		_isMoveMiniScope = false;
		openningSampleBoard();
	}
	else 
	{
		if(!_tool->isPicking())
			closingSampleBoard();
	}

	if (_tool)
		_tool->update();


	
}

void mapToolScene::render()
{
	// canvas : 실제 맵
	//D2DMANAGER->drawRectangle(_canvas);
	//_tool->terrainRender();
	
	// canver 아래
	_uiBG[0]->render(_canvas.left, _canvas.bottom, _canvas.right, WINSIZEY, 1.f);
	
	// minimap
	//_uiBG[4]->render(_miniMap, 1.0f);
	//_tool->terrainRender(_miniMap.left, _miniMap.top, 0.019f);
	//D2DMANAGER->drawRectangle(_miniMap);
	//D2DMANAGER->drawRectangle(_miniScope);
	
	// canver 상 좌 우
	_uiBG[0]->render(0.f, 0.f, WINSIZEX, UI_SPACE, 1.f);
	_uiBG[0]->render(0.f, 0.f, UI_SPACE, WINSIZEY, 1.f);
	_uiBG[0]->render(_canvas.right, 0.f, WINSIZEX - _canvas.right, WINSIZEY, 1.f);

	// minimap 우 하
	_uiBG[0]->render(_miniMap.right, _canvas.bottom, WINSIZEX - _miniMap.right, WINSIZEY - _canvas.bottom, 1.f);
	_uiBG[0]->render(_miniMap.left, _miniMap.bottom, _miniMap.right - _miniMap.left, WINSIZEY - _miniMap.bottom, 1.f);

	// fileZone
	_uiBG[4]->render(_fileZone, 1.0f);

	// hierarchy
	_uiBG[4]->render(_hierarchy, 1.0f);

	// inspector
	_uiBG[4]->render(_inspector, 1.0f);


	_createCol->render();

	// sampleBoard
	_uiBG[3]->render(_sampleBoard, 1.f);
	D2DMANAGER->drawRectangle(_sampleBoard);

	// 버튼
	_qickOpen->render();
	_nextSample->render();
	_beforeSample->render();

	// 툴
	//_tool->render();

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
