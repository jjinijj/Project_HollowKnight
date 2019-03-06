#include "stdafx.h"
#include "imageManager.h"


imageManager::imageManager()
{
}


imageManager::~imageManager()
{
}

HRESULT imageManager::init(void)
{
	return S_OK;
}

void imageManager::release(void)
{
	deleteAll();
}

//===================================================================
//						이미지 추가
//===================================================================
image* imageManager::addImage(string strKey, LPCWSTR fileName, int width, int height)
{
	//먼저 동일한 키 값을 가진 이미지가 있는지 검색
	image* img = findImage(strKey);

	// 이미지가 있으면 그 이미지를 반환
	if (img) return img;

	// 이미지가 없으면 메모리 동적 할당
	img = new image;

	//이미지를 초기화하고 실패했을 경우 메모리 해제
	if (FAILED(img->init(fileName, width, height)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//맵에 추가
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image* imageManager::addFrameImage(string strKey, LPCWSTR fileName, int width, int height, int frameNumX, int frameNumY)
{
	//먼저 동일한 키 값을 가진 이미지가 있는지 검색
	image* img = findImage(strKey);

	// 이미지가 있으면 그 이미지를 반환
	if (img) return img;

	// 이미지가 없으면 메모리 동적 할당
	img = new image;

	//이미지를 초기화하고 실패했을 경우 메모리 해제
	if (FAILED(img->init(fileName, width, height, frameNumX, frameNumY)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//맵에 추가
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image* imageManager::findImage(string strKey)
{
	mapImageIter key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		return key->second;
	}

	return NULL;
}

BOOL imageManager::deleteImage(string strKey)
{
	//해당 키 값이 존재하는지 검사한다
	mapImageIter key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		key->second->release();
		SAFE_DELETE(key->second);
		_mImageList.erase(key);

		return true;
	}

	return false;
}

BOOL imageManager::deleteAll(void)
{
	mapImageIter iter = _mImageList.begin();

	for (; iter != _mImageList.end();)
	{
		if (iter->second != NULL)
		{
			iter->second->release();
			SAFE_DELETE(iter->second);
			iter = _mImageList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	_mImageList.clear();

	return TRUE;
}



//===================================================================
//			렌더 함수 - Left Top 기준으로 렌더
//===================================================================

void imageManager::render(string strKey, float alpha)
{
	image* img = findImage(strKey);
	if (img) img->render(alpha);
}

void imageManager::render(string strKey, float destX, float destY, float alpha)
{
	image* img = findImage(strKey);
	if (img) img->render(destX, destY, alpha);
}

void imageManager::render(string strKey, float destX, float destY, int showWidth, int showHeight, float alpha)
{
	image* img = findImage(strKey);
	if (img) img->render(destX, destY, showWidth, showHeight, alpha);
}

void imageManager::render(string strKey, float destX, float destY, float sourX, float sourY, int sourWidth, int sourHeight, float alpha)
{
	image* img = findImage(strKey);
	if (img) img->render(destX, destY, sourX, sourY, sourWidth, sourHeight, alpha);
}

void imageManager::render(string strKey, float destX, float destY, int showWidth, int showHeight, float sourX, float sourY, int sourWidth, int sourHeight, float alpha)
{
	image* img = findImage(strKey);
	if (img) img->render(destX, destY, showWidth, showHeight, sourX, sourY, sourWidth, sourHeight, alpha);
}

void imageManager::loopRender(string strKey, D2D1_RECT_F drawArea, int offSetX, int offSetY, float opacity)
{
	image* img = findImage(strKey);

	if (img) img->loopRender(drawArea, offSetX, offSetY);
}

void imageManager::frameRender(string strKey, float destX, float destY, int currentFrameX, int currentFrameY, float alpha)
{
	image* img = findImage(strKey);
	if (img) img->frameRender(destX, destY, currentFrameX, currentFrameY, alpha);
}

void imageManager::frameRender(string strKey, float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, float alpha)
{
	image* img = findImage(strKey);
	if (img) img->frameRender(destX, destY, showWidth, showHeight, currentFrameX, currentFrameY, alpha);
}

void imageManager::frameRenderReverseX(string strKey, float destX, float destY, int currentFrameX, int currentFrameY, float alpha)
{
	image* img = findImage(strKey);
	if (img) img->frameRenderReverseX(destX, destY, currentFrameX, currentFrameY, alpha);
}

void imageManager::frameRenderReverseX(string strKey, float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, float alpha)
{
	image* img = findImage(strKey);
	if (img) img->frameRenderReverseX(destX, destY, showWidth, showHeight, currentFrameX, currentFrameY, alpha);
}
