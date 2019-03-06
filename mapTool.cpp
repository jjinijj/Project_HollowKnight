#include "stdafx.h"
#include "mapTool.h"
#include "mapData.h"


mapTool::mapTool()
: _sampleIdx(0)
, _mapData(nullptr)
, _samples(nullptr)

, _isPicking(false)
, _canvers(nullptr)
, _canversSample(nullptr)
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

	_imgUids.push_back(eImage_Town_Floor);
	_imgUids.push_back(eImage_Town_BG);
	_imgUids.push_back(eImage_Town_BG_Big01);
	_imgUids.push_back(eImage_Town_BG_Big02);
	_imgUids.push_back(eImage_Town_Build);
	_imgUids.push_back(eImage_Town_Ruddle);
	_imgUids.push_back(eImage_Object_Chair);
	_imgUids.push_back(eImage_Fence);
	_imgUids.push_back(eImage_Town_Well);
	_imgUids.push_back(eImage_StreetLamp);


		//eImage_Town_Build01 = 50,
		//eImage_Town_Build02,
		//eImage_Town_Build03,
		//eImage_Town_Build04,
		//eImage_Town_Build05,
		//eImage_Town_Build06,
		//eImage_Town_Build07,




	_samples = IMGDATABASE->getImage(_imgUids[_sampleIdx]);
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

	pickSample();
	pickCanvers();
}

void mapTool::render()
{
	if(_samples)
		_samples->render(_canversSample->left + 20.f, _canversSample->top + DISTANCE, 1.f);

	if(_isPicking)
		D2DMANAGER->drawRectangle(_pickArea);

	if (_pick.isPick)
	{
		_samples->render(_ptMouse.x, _ptMouse.y, _pick.x, _pick.y, _pick.width, _pick.height, 0.5f);
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

void mapTool::pickSample()
{
	if (!PtInRectD2D(*_canversSample, _ptMouse))
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

		float width = _pickArea.right - _pickArea.left;
		float height = _pickArea.bottom - _pickArea.top;

		if (	-5.f < width &&  width <= 5.f
			||  -5.f < height && height <= 5.f)
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

		_pick.x = _pickArea.left - (_canversSample->left + DISTANCE);
		_pick.y = _pickArea.top - (_canversSample->top + DISTANCE);

		_pick.width = width;
		_pick.height = height;

		_pick.uid = _imgUids[_sampleIdx];

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
	float destX = _ptMouse.x + CAMERA->getPosX() - CAMERA->getScopeRect().left;
	float destY = _ptMouse.y + CAMERA->getPosY() - CAMERA->getScopeRect().top;

	//float miniDestX = destX / MAPSIZEX * (_miniMap->right - _miniMap->left);
	//float miniDestY = destY / MAPSIZEX * (_miniMap->bottom - _miniMap->top);

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		switch (_terType)
		{
			case eTerrain_Frame:
			{
				_mapData->addTerrainFrame(_curLayer, destX, destY, _pick.frameX, _pick.framxY, _pick.uid);
				terrainFrame* frame = new terrainFrame;
				//frame->init(NULL, miniDestX, miniDestY, _pick.frameX, _pick.framxY, _pick.uid );
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
}

void mapTool::beforeSample()
{
}
