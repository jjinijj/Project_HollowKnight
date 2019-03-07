#include "stdafx.h"
#include "mapTool.h"
#include "mapData.h"


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

	return S_OK;
}

void mapTool::release()
{
}

void mapTool::update()
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

	_canversSample = {_sampleBoard->left + (3.f * DISTANCE)
					, _sampleBoard->top + DISTANCE
					, _sampleBoard->left + (3.f * DISTANCE) + _sampleImg->GetWidth()
					, _sampleBoard->top + DISTANCE  + _sampleImg->GetHeight()};
	pickSample();
	pickCanvers();
}

void mapTool::render()
{
	if(_sampleImg)
		_sampleImg->render(_canversSample.left, _canversSample.top, 1.f);

	if(_isPicking)
		D2DMANAGER->drawRectangle(_pickArea);

	if (_pick.isPick) 
	{
		if (_pick.isFrame)
			IMGDATABASE->getImage(_pick.uid)->render(_ptMouse.x, _ptMouse.y, 0.5f);
		else
			_sampleImg->render(_ptMouse.x, _ptMouse.y, _pick.x, _pick.y, _pick.width, _pick.height, 0.5f);
	}

	D2DMANAGER->drawRectangle(_canversSample);
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

void mapTool::setSampleBoardRect(RECTD2D* rc)
{
	_sampleBoard = rc;
	_canversSample = { rc->left + DISTANCE, rc->top + DISTANCE, rc->right - DISTANCE, rc->bottom - DISTANCE };
}

void mapTool::pickSample()
{
	if (!PtInRectD2D(_canversSample, _ptMouse))
		return;

	// pick
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		_isPicking = true;
		_pickArea = { _ptMouse.x, _ptMouse.y, _ptMouse.x + 1.f, _ptMouse.y + 1.f };
	}

	// drag
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		_pickArea.right = _ptMouse.x;
		_pickArea.bottom = _ptMouse.y;
	}

	//up
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		_pick.clear();
		_isPicking = false;
		
		_pickArea.right = _ptMouse.x;
		_pickArea.bottom = _ptMouse.y;

		IMGLNK* curLnk = _imgLnks[_sampleIdx];

		// 프레임 이미지
		if (curLnk->isFrameImg)
		{
			int frameX = (int)(_ptMouse.x - _canversSample.left) / _sampleImg->GetFrameWidth();
			int frameY = (int)(_ptMouse.y - _canversSample.top) / _sampleImg->GetFrameHeight();

			int idx = frameY * (_sampleImg->GetMaxFrameX() + 1) + frameX;

			if(curLnk->lnkSize <= idx)
				return;

			_pick.uid = curLnk->lnkUIDs[idx];
			_pick.isFrame = true;
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

			_pick.x = _pickArea.left - _canversSample.left;
			_pick.y = _pickArea.top - _canversSample.top;

			_pick.width = width;
			_pick.height = height;

			_pick.uid = curLnk->mainUID;
		}

		_pick.isPick = true;
	}
}

void mapTool::pickCanvers()
{
	// pick 아니면 return
	if(!_pick.isPick)
		return;

	// 그리는 구역이 아니면 return
	if(!PtInRectD2D(*_canvers, _ptMouse))
		return;

	// 0,0 기준으로 위치 지정
	float destX = _ptMouse.x + CAMERA->getPosX();// - CAMERA->getScopeRect().left;
	float destY = _ptMouse.y + CAMERA->getPosY();// - CAMERA->getScopeRect().top;

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
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
				_mapData->addTerrainClear(_curLayer, destX, destY, _pick.width, _pick.height);
				break;
			}

			default:
				break;
		}
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

void mapTool::setSampleImage()
{
	_sampleImg = IMGDATABASE->getImage(_imgLnks[_sampleIdx]->mainUID);

	if(_imgLnks[_sampleIdx]->isFrameImg)
		_terType = eTerrain_Frame;
	else
		_terType = eTerrain_Drag;
}

