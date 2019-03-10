#include "stdafx.h"
#include "mapToolScene.h"

#include "mapTool.h"
#include "uiButton.h"

mapToolScene::mapToolScene()
: _tool(nullptr)
{
}


mapToolScene::~mapToolScene()
{
}

HRESULT mapToolScene::init()
{
	_sceneName = eSceneName_MapTool;

	
	_tool = new mapTool;
	_tool->init();
	UIMANAGER->insertUI(eUI_MapTool, _tool);
	_tool->uiOpen();
	_tool->setActive(true);

	
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
	//if (!_isOpenSampleBoard && ! _isCloseSampleBoard)
	//{
	//	if(eToolMode_DrawCollider != _tool->getToolMode())
	//		_createCol->setState(eButton_Up);
	//}

	// 툴에서 드래그 중이 아닐 경우에만 버튼 활성화 
	//if (!_tool->isPicking())
	//{
	//	_qickOpen->update();
	//	_beforeSample->update();
	//	_nextSample->update();
	//	_createCol->update();
	//}

	//if (PtInRectD2D(_sampleBoard, _ptMouse))
	//{
	//	_isMoveMiniScope = false;
	//	openningSampleBoard();
	//}
	//else 
	//{
	//	if(!_tool->isPicking())
	//		closingSampleBoard();
	//}

	//if (_tool)
	//	_tool->update();


	
}

void mapToolScene::render()
{
}

