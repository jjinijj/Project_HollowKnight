#include "stdafx.h"
#include "mapTool.h"
#include "mapData.h"
#include "uiButton.h";
#include "uiPanel.h"


mapTool::mapTool()
: _sampleIdx(0)
, _mapData(nullptr)
, _sampleImg(nullptr)

, _isPicking(false)
, _canvers(nullptr)
{
	_pickMousePos = {0.f, 0.f};
}


mapTool::~mapTool()
{
}

HRESULT mapTool::init()
{
	_sampleIdx = 0;

	_mapData = new mapData;
	_mapData->init();

	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_Floor, false);
		_imgLnks.push_back(lnk);
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_BG, false);
		_imgLnks.push_back(lnk);
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_BG_Big01, false);
		_imgLnks.push_back(lnk);
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_BG_Big02, false);
		_imgLnks.push_back(lnk);
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_Build, true);
		for (int ii = eImage_Town_Build01; ii <= eImage_Town_Build07; ++ii)
		{
			lnk->pushBack((eImageUID)ii);
		}
		_imgLnks.push_back(lnk);
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_Ruddle, false);
		_imgLnks.push_back(lnk);
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Object_Chair, false);
		_imgLnks.push_back(lnk);
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Fence, false);
		_imgLnks.push_back(lnk);
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_Town_Well, false);
		_imgLnks.push_back(lnk);
	}
	{
		IMGLNK* lnk = new IMGLNK;
		lnk->makeImageLnk(eImage_StreetLamp, false);
		_imgLnks.push_back(lnk);
	}

	setSampleImage();
	_pick.clear();

	_curLayer = eLayer_Play;
	_terType = eTerrain_Drag;

	_mode = eToolMode_DrawTerrain;

	// 샘플판
	{
		_samplePanel = new uiPanel;
		_samplePanel->init(0.f, 0.f, 0.f, 0.f, nullptr);

		_sampleCanvers = new uiPanel;
		_sampleCanvers->init(3.f * DISTANCE, DISTANCE, 0.f, 0.f, _sampleImg);
		_sampleCanvers->setOnClickFunction(std::bind(&mapTool::pickSampleStart, this));
		_sampleCanvers->setPressFunction(std::bind(&mapTool::picking, this));
		_sampleCanvers->setOnClickUPFunction(std::bind(&mapTool::pickSampleEnd, this));

		_samplePanel->insertChild(_sampleCanvers);
	}

	//_state = nullptr;

	return S_OK;
}

void mapTool::release()
{
}

void mapTool::update()
{
	switch (_mode)
	{
		case eToolMode_DrawTerrain: { updateDrawTerrain();	break;}
		case eToolMode_DrawCollider:{ updateDrawCollider(); break;}
		case eToolMode_DrawObject:  { break; }
		case eToolMode_Inspector:   { break; }
	}

	picking();
	if (_isPicking)
	{
		pickSample();
		pickCanvers();
	}

	_samplePanel->update();
}

void mapTool::render()
{
	_samplePanel->render();

	if (_isPicking)
		D2DMANAGER->drawRectangle(_pickArea);

	switch (_mode)
	{
		case eToolMode_DrawTerrain: { renderDrawTerrain();	break; }
		case eToolMode_DrawCollider:{ renderDrawCollider(); break; }
		case eToolMode_DrawObject:  { break; }
		case eToolMode_Inspector:   { break; }
	}
}

void mapTool::terrainRender()
{
	if (_mapData)
		_mapData->render();
}

void mapTool::terrainRender(float destX, float destY, float percent)
{
	vector<terrain*>* terrains = _mapData->getTerrains();
	vector<terrain*>::iterator iter = terrains->begin();
	vector<terrain*>::iterator end = terrains->end();

	for (; iter != end; ++iter)
	{
		(*iter)->render(destX, destY, percent);
	}
}

void mapTool::pickSampleStart()
{
	_isPicking = true;
	_pickArea = { _ptMouse.x, _ptMouse.y, _ptMouse.x + 1.f, _ptMouse.y + 1.f };
	_mode = eToolMode_DrawTerrain;
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
		int frameX = (int)(_ptMouse.x - _sampleCanvers->getPositionX()) / _sampleImg->GetFrameWidth();
		int frameY = (int)(_ptMouse.y - _sampleCanvers->getPositionY()) / _sampleImg->GetFrameHeight();

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

		POINTF pf = _samplePanel->getWorldPosition();

		_pick.x = _pickArea.left - pf.x;
		_pick.y = _pickArea.top - pf.y;

		_pick.width = width;
		_pick.height = height;

		_pick.uid = curLnk->mainUID;
	}

	_pick.isPick = true;
}

void mapTool::pickCanversStart()
{
	if (eToolMode_DrawCollider == _mode)
	{
		_pick.clear();
		_pick.isPick = true;
		_pickArea = { _ptMouse.x, _ptMouse.y, _ptMouse.x + 1.f, _ptMouse.y + 1.f };
	}
}

void mapTool::pickCanversEnd()
{
	// 0,0 기준으로 위치 지정
	float destX = 0;
	float destY = 0;
	if (eToolMode_DrawCollider == _mode)
	{
		destX = _ptMouse.x + CAMERA->getPosX() - CAMERA->getScopeRect().left;
		destY = _ptMouse.y + CAMERA->getPosY() - CAMERA->getScopeRect().top;
	}
	else
	{
		destX = (_ptMouse.x - _pick.width / 2.f) + CAMERA->getPosX() - CAMERA->getScopeRect().left;
		destY = (_ptMouse.y - _pick.height / 2.f) + CAMERA->getPosY() - CAMERA->getScopeRect().top;
	}

	switch (_terType)
	{
		case eTerrain_Frame:
		{
			_mapData->addTerrainFrame(_curLayer, destX, destY, _pick.frameX, _pick.frameY, _pick.uid);
			break;
		}

		case eTerrain_Drag:
		{
			_mapData->addTerrainDrag(_curLayer, destX, destY, _pick.x, _pick.y, _pick.width, _pick.height, _pick.uid);
			break;
		}

		case eTerrain_Clear:
		{
			_pick.clear();
			_isPicking = false;

			_pickArea.right = _ptMouse.x;
			_pickArea.bottom = _ptMouse.y;

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

			_mapData->addTerrainClear(_curLayer, destX, destY, width, height);
			break;
		}

		default:
			break;
	}
}

void mapTool::picking()
{
	_pickArea.right = _ptMouse.x;
	_pickArea.bottom = _ptMouse.y;
}

void mapTool::pickSample()
{
}

void mapTool::pickCanvers()
{
	if (!PtInRectD2D(*_canvers, _ptMouse))
		return;

	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		
	}
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
}

void mapTool::setSampleImage()
{
	_sampleImg = IMGDATABASE->getImage(_imgLnks[_sampleIdx]->mainUID);

	if(_imgLnks[_sampleIdx]->isFrameImg)
		_terType = eTerrain_Frame;
	else
		_terType = eTerrain_Drag;

	_pick.clear();
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
