#include "stdafx.h"
#include "terrain.h"

//=============================================
//   지형
//=============================================
terrain::terrain()
: _uid(0)
, _x(0.f)
, _y(0.f)
, _attr(NULL)
{
	_rc = {};
}

terrain::~terrain()
{
}

HRESULT terrain::init()
{
	_x = 0.f;
	_y = 0.f;
	_attr = NULL;
	_rc = {};

	return S_OK;
}

HRESULT terrain::init(const UID uid, const float destX, const float destY)
{
	init();
	_uid = uid;
	_x = destX + CAMERA->getScopeRect().left;
	_y = destY + CAMERA->getScopeRect().top;

	return S_OK;
}

void terrain::render()
{
	if ((_debugMode & DEBUG_SHOW_RECT) == DEBUG_SHOW_RECT)
		D2DMANAGER->drawRectangle(_rc, false);
}

void terrain::render(float destX, float destY, float percent)
{
	if ((_debugMode & DEBUG_SHOW_RECT) == DEBUG_SHOW_RECT)
		D2DMANAGER->drawRectangle(_rc, false);
}

TARRAINPACK* terrain::makePack()
{
	TARRAINPACK* pack = new TARRAINPACK;
	pack->clear();

	pack->uid	= _uid;
	// 0,0 기준으로 저장하기 위해 카메라의 scope 범위를 빼줌
	pack->x		= _x - CAMERA->getScopeRect().left;
	pack->y		= _y - CAMERA->getScopeRect().top;
	pack->rc	= {  _rc.left - CAMERA->getScopeRect().left
					,_rc.top - CAMERA->getScopeRect().top
					,_rc.right - CAMERA->getScopeRect().left
					,_rc.bottom - CAMERA->getScopeRect().top};
	pack->attr	= _attr;

	return pack;
}

void terrain::loadPack(TARRAINPACK* pack)
{
	if (pack)
	{
		_uid	= pack->uid;
		
		// 0,0 기준으로 저장이 되어있기 때문에 카메라의 scope 범위를 더해줌
		_x		= pack->x + CAMERA->getScopeRect().left;
		_y		= pack->y + CAMERA->getScopeRect().top;
		_rc		= {	 pack->rc.left + CAMERA->getScopeRect().left
					,pack->rc.top + CAMERA->getScopeRect().top
					,pack->rc.right + CAMERA->getScopeRect().left
					,pack->rc.bottom + CAMERA->getScopeRect().top };
		_attr	= pack->attr;
	}
}

void terrain::addAttribute(const UINT attr)
{
	if(checkAttribute(attr))
		return;

	_attr |= attr;
}

void terrain::removeAttribute(const UINT attr)
{
	if (!checkAttribute(attr))
		return;

	_attr ^= attr;
}

bool terrain::checkAttribute(const UINT attr)
{
	if((attr & _attr) == attr)
		return true;
	else 
		return false;
}




//=============================================
//   드래그 지형
//=============================================
terrainDrag::terrainDrag()
{
}

terrainDrag::~terrainDrag()
{
}

HRESULT terrainDrag::init( UID uid
						  ,float destX, float destY
						  ,float sourX, float sourY
						  ,float width, float height
						  ,eImageUID imgUid)
{
	terrain::init(uid, destX, destY);
	_width = width;
	_height = height;
	_sourX = sourX;
	_sourY = sourY;

	_img = IMGDATABASE->getImage(imgUid);
	assert(_img != nullptr);

	_imgUid = imgUid;

	_rc = {_x, _y, _x + width, _y + height};

	return S_OK;
}

void terrainDrag::release()
{
	_img = nullptr;
}

void terrainDrag::update()
{
}

void terrainDrag::render()
{
	terrain::render();

	if(CAMERA->isRangeInCamera(_rc.left, _rc.top, _rc.right, _rc.bottom))
		_img->render( _x
					 ,_y
					 ,_sourX, _sourY
					 ,_width, _height
					 ,1.0f, false);
}

void terrainDrag::render(float destX, float destY, float percent)
{
	_img->render( destX + _x * percent, destY + _y * percent
				 ,_width * percent, _height * percent
				 ,_sourX, _sourY
				 ,_width, _height
				 ,1.0f);
}

TARRAINPACK* terrainDrag::makePack()
{
	TARRAINPACK* pack = terrain::makePack();

	if (pack)
	{
		pack->imgUid = _imgUid;
		pack->width	 = _width;
		pack->height = _height;
	}

	return pack;
}

void terrainDrag::loadPack(TARRAINPACK* pack)
{
	if (pack)
	{
		terrain::loadPack(pack);

		_imgUid = (eImageUID)pack->imgUid;
		_width	= pack->width;
		_height = pack->height;

		_img = IMGDATABASE->getImage(_imgUid);
		assert(_img != nullptr);
	}
}





//=============================================
//   프레임 지형
//=============================================
terrainFrame::terrainFrame()
{
}

terrainFrame::~terrainFrame()
{
}

HRESULT terrainFrame::init( UID uid
						   ,float destX, float destY
						   ,UINT frameX, UINT frameY
						   ,eImageUID imgUid)
{
	terrain::init(uid, destX, destY);
	_frameX = frameX;
	_frameY = frameY;

	_img = IMGDATABASE->getImage(imgUid);
	assert(_img != nullptr);
	_imgUid = imgUid;

	return S_OK;
}

void terrainFrame::release()
{
	_img = nullptr;
}

void terrainFrame::update()
{
}

void terrainFrame::render()
{
	terrain::render();
	_img->render( _x 
				 ,_y
				 ,1.f
				 ,false);
}

void terrainFrame::render(float destX, float destY, float percent)
{
	_img->render( _x * percent + destX
				 ,_y * percent + destY
				 ,_img->GetWidth() * percent
				 ,_img->GetHeight() * percent, 1.0f);
}

TARRAINPACK* terrainFrame::makePack()
{
	TARRAINPACK* pack = terrain::makePack();
	if (pack)
	{
		pack->imgUid = _imgUid;
		pack->frameX = _frameX;
		pack->framxY = _frameY;
	}

	return pack;
}

void terrainFrame::loadPack(TARRAINPACK* pack)
{
	if (pack)
	{
		terrain::loadPack(pack);

		_imgUid = (eImageUID)pack->imgUid;
		_frameX = pack->frameX;
		_frameY = pack->framxY;

		_img = IMGDATABASE->getImage(_imgUid);
		assert(_img != nullptr);
	}
}



//=============================================
//   투명 지형
//=============================================
terrainClear::terrainClear()
{
}

terrainClear::~terrainClear()
{
}

HRESULT terrainClear::init(const UID uid, float destX, float destY, float width, float height)
{
	terrain::init(uid, destX, destY);

	_width = width;
	_height = height;


	_rc = { _x, _y, _x + width, _y + height };

	return S_OK;
}

void terrainClear::release()
{
}

void terrainClear::update()
{
}

void terrainClear::render()
{
	D2DMANAGER->drawRectangle(_rc, false);
}

void terrainClear::render(float destX, float destY, float percent)
{
}

TARRAINPACK* terrainClear::makePack()
{
	TARRAINPACK* pack = terrain::makePack();
	return pack;
}

void terrainClear::loadPack(TARRAINPACK* pack)
{
	if (pack)
	{
		terrain::loadPack(pack);
	}
}
