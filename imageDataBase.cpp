#include "stdafx.h"
#include "imageDataBase.h"


imageDataBase::imageDataBase()
{
	_imgMap.clear();
}


imageDataBase::~imageDataBase()
{
}

HRESULT imageDataBase::init()
{
	return S_OK;
}

void imageDataBase::release()
{
	iterMImage iter = _imgMap.begin();
	iterMImage end = _imgMap.end();

	for (; iter != end; ++iter)
	{
		iter->second = nullptr;
	}

	_imgMap.clear();
}

void imageDataBase::insertImage(eImageUID uid, image* img)
{
	if(!img)
		return;

	if(_imgMap.find(uid) != _imgMap.end())
		return;

	_imgMap.insert(make_pair(uid, img));
}

image* imageDataBase::getImage(eImageUID uid)
{
	image* img = nullptr;

	if (_imgMap.find(uid) != _imgMap.end())
		img = _imgMap[uid];

	return img;
}
