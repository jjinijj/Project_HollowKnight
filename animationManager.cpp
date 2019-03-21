#include "stdafx.h"
#include "animationManager.h"
#include "animation.h"

animationManager::animationManager()
{
}

animationManager::~animationManager()
{
}

HRESULT animationManager::init()
{
	return S_OK;
}

void animationManager::release()
{
	clear();
}

void animationManager::addDefaultFrameAnimation(UINT uid, UINT key, const char* imageKeyName, int fps, bool reverse, bool loop)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);
	assert(nullptr != img);

	aniMap* aniM = nullptr;
	if (_totalAniMap.find(uid) == _totalAniMap.end())
	{
		aniM = new aniMap;
		_totalAniMap.insert(make_pair(uid, aniM));
	}
	else
		aniM = _totalAniMap[uid];

	if(aniM->find(key) != aniM->end())
		return;

	animation* ani = new animation;
	ani->init(img->GetWidth(), img->GetHeight(), img->GetFrameWidth(), img->GetFrameHeight());
	ani->setDefPlayFrame(reverse, loop);
	ani->setFPS(fps);
	ani->setImage(img);

	aniM->insert(make_pair(key, ani));
}

void animationManager::addArrayFrameAnimation(UINT uid, UINT key, const char* imageKeyName, int startIdx, int endIdx, int fps, bool loop)
{
	addArrayFrameAnimation(uid, key, imageKeyName, startIdx, endIdx, -1, fps, loop);
}

void animationManager::addArrayFrameAnimation(UINT uid, UINT key, const char* imageKeyName, int* arr, int arrLen, int fps, bool loop)
{
	addArrayFrameAnimation(uid, key, imageKeyName, arr, arrLen, -1, fps, loop);
}

void animationManager::addArrayFrameAnimation(UINT uid, UINT key, const char* imageKeyName, int startIdx, int endIdx, int eventFrame, int fps, bool loop)
{
	int size = endIdx - startIdx + 1;
	int* arr = new int[size];

	for (int ii = 0; ii < size; ++ii)
		arr[ii] = startIdx + ii;

	addArrayFrameAnimation(uid, key, imageKeyName, arr, size, eventFrame, fps, loop);
}

void animationManager::addArrayFrameAnimation(UINT uid, UINT key, const char* imageKeyName, int* arr, int arrLen, int eventFrame, int fps, bool loop)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);
	assert(nullptr != img);

	aniMap* aniM = nullptr;
	if (_totalAniMap.find(uid) == _totalAniMap.end())
	{
		aniM = new aniMap;
		_totalAniMap.insert(make_pair(uid, aniM));
	}
	else
		aniM = _totalAniMap[uid];

	if (aniM->find(key) != aniM->end())
		return;

	animation* ani = new animation;
	ani->init(img->GetWidth(), img->GetHeight(), img->GetFrameWidth(), img->GetFrameHeight());
	ani->setPlayFrame(arr, arrLen, loop);
	ani->setFPS(fps);
	ani->setEventFrame(eventFrame);
	ani->setImage(img);

	aniM->insert(make_pair(key, ani));
}


void animationManager::start(UINT uid, UINT key)
{
	animation* ani = findAnimation(uid, key);
	if (ani)
		ani->start();
}

void animationManager::stop(UINT uid, UINT key)
{
	animation* ani = findAnimation(uid, key);
	if (ani)
		ani->stop();
}

void animationManager::pause(UINT uid, UINT key)
{
	animation* ani = findAnimation(uid, key);
	if (ani)
		ani->pause();
}

void animationManager::resume(UINT uid, UINT key)
{
	animation* ani = findAnimation(uid, key);
	if (ani)
		ani->resume();
}

animation* animationManager::findAnimation(UINT uid, UINT key)
{
	aniMap*		aniM = nullptr;
	animation*	ani	 = nullptr;

	if(_totalAniMap.find(uid) != _totalAniMap.end())
		aniM = _totalAniMap[uid];

	if (nullptr != aniM)
	{
		if(aniM->find(key) != aniM->end())
			ani = aniM->find(key)->second;
	}

	return ani;
}

void animationManager::clear()
{
	iterTM	iter = _totalAniMap.begin();
	iterM	it;
	for (iter; _totalAniMap.end() != iter;)
	{
		aniMap* aniM = iter->second;
		it = aniM->begin();
		for (it; aniM->end() != it; )
		{
			animation* ani = it->second;
			it = aniM->erase(it);
			
			SAFE_RELEASE(ani);
			SAFE_DELETE(ani);
		}

		aniM->clear();

		iter = _totalAniMap.erase(iter);
		SAFE_DELETE(aniM);
	}

	_totalAniMap.clear();
}
