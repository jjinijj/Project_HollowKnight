#include "stdafx.h"
#include "keyAniManager.h"
#include "image.h"
#include "animation.h"


keyAniManager::keyAniManager()
{
}


keyAniManager::~keyAniManager()
{
}

HRESULT keyAniManager::init()
{
	return S_OK;
}

void keyAniManager::release()
{
	deleteAll();
}

void keyAniManager::update(string type)
{
	iterAnimationType typeIter = _mTotalAnimation.find(type);

	if (typeIter != _mTotalAnimation.end())
	{
		iterAnimation iter = typeIter->second.begin();

		for (iter; iter != typeIter->second.end(); ++iter)
		{
			if (!iter->second->isPlay()) continue;

			iter->second->frameUpdate(TIMEMANAGER->getElapsedTime() * 1.0f);
		}
	}
}

void keyAniManager::render()
{
}

void keyAniManager::addAnimationType(string type)
{
	arrAnimation arrAni;
	_mTotalAnimation.insert(pair<string, arrAnimation>(type, arrAni));
}

void keyAniManager::addDefaultFrameAnimation(string type, string animationKeyName, const char * imageKeyName, int fps, bool reverse, bool loop)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);

	animation* ani = new animation;

	ani->init(img->GetWidth(), img->GetHeight(), img->GetFrameWidth(), img->GetFrameHeight());
	ani->setDefPlayFrame(reverse, loop);
	ani->setFPS(fps);

	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		typeIter->second.insert(pair<string, animation*>(animationKeyName, ani));
	}
}

void keyAniManager::addDefaultFrameAnimation(string type, string animationKeyName, const char * imageKeyName, int fps, bool reverse, bool loop, void * cbFunction)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);

	animation* ani = new animation;

	ani->init(img->GetWidth(), img->GetHeight(), img->GetFrameWidth(), img->GetFrameHeight());
	ani->setDefPlayFrame(reverse, loop, CALLBACK_FUNCTION(cbFunction));
	ani->setFPS(fps);
	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		typeIter->second.insert(pair<string, animation*>(animationKeyName, ani));
	}

}

void keyAniManager::addDefaultFrameAnimation(string type, string animationKeyName, const char * imageKeyName, int fps, bool reverse, bool loop, void * cbFunction, void * obj)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);

	animation* ani = new animation;

	ani->init(img->GetWidth(), img->GetHeight(), img->GetFrameWidth(), img->GetFrameHeight());
	ani->setDefPlayFrame(reverse, loop, CALLBACK_FUNCTION_PARAMETER(cbFunction), obj);
	ani->setFPS(fps);
	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		typeIter->second.insert(pair<string, animation*>(animationKeyName, ani));
	}
}

void keyAniManager::addArrayFrameAnimation(string type, string animationKeyName, const char * imageKeyName, int * arr, int arrLen, int fps, bool loop)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);

	animation* ani = new animation;

	ani->init(img->GetWidth(), img->GetHeight(), img->GetFrameWidth(), img->GetFrameHeight());
	ani->setPlayFrame(arr, arrLen, loop);
	ani->setFPS(fps);
	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		typeIter->second.insert(pair<string, animation*>(animationKeyName, ani));
	}
}

void keyAniManager::addArrayFrameAnimation(string type, string animationKeyName, const char * imageKeyName, int * arr, int arrLen, int fps, bool loop, void * cbFunction)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);

	animation* ani = new animation;

	ani->init(img->GetWidth(), img->GetHeight(), img->GetFrameWidth(), img->GetFrameHeight());
	ani->setPlayFrame(arr, arrLen, loop, CALLBACK_FUNCTION(cbFunction));
	ani->setFPS(fps);
	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		typeIter->second.insert(pair<string, animation*>(animationKeyName, ani));
	}
}

void keyAniManager::addArrayFrameAnimation(string type, string animationKeyName, const char * imageKeyName, int * arr, int arrLen, int fps, bool loop, void * cbFunction, void * obj)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);

	animation* ani = new animation;

	ani->init(img->GetWidth(), img->GetHeight(), img->GetFrameWidth(), img->GetFrameHeight());
	ani->setPlayFrame(arr, arrLen, loop, (CALLBACK_FUNCTION_PARAMETER)cbFunction, obj);
	ani->setFPS(fps);
	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		typeIter->second.insert(pair<string, animation*>(animationKeyName, ani));
	}
}

void keyAniManager::addCoordinateFrameAnimation(string type, string animationKeyName, const char * imageKeyName, int start, int end, int fps, bool reverse, bool loop)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);

	animation* ani = new animation;

	ani->init(img->GetWidth(), img->GetHeight(), img->GetFrameWidth(), img->GetFrameHeight());
	ani->setPlayFrame(start, end, reverse, loop);
	ani->setFPS(fps);
	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		typeIter->second.insert(pair<string, animation*>(animationKeyName, ani));
	}
}

void keyAniManager::addCoordinateFrameAnimation(string type, string animationKeyName, const char * imageKeyName, int start, int end, int fps, bool reverse, bool loop, void * cbFunction)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);

	animation* ani = new animation;

	ani->init(img->GetWidth(), img->GetHeight(), img->GetFrameWidth(), img->GetFrameHeight());
	ani->setPlayFrame(start, end, reverse, loop, (CALLBACK_FUNCTION)cbFunction);
	ani->setFPS(fps);
	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		typeIter->second.insert(pair<string, animation*>(animationKeyName, ani));
	}
}

void keyAniManager::addCoordinateFrameAnimation(string type, string animationKeyName, const char * imageKeyName, int start, int end, int fps, bool reverse, bool loop, void * cbFunction, void * obj)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);

	animation* ani = new animation;

	ani->init(img->GetWidth(), img->GetHeight(), img->GetFrameWidth(), img->GetFrameHeight());
	ani->setPlayFrame(start, end, reverse, loop, CALLBACK_FUNCTION_PARAMETER(cbFunction), obj);
	ani->setFPS(fps);
	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		typeIter->second.insert(pair<string, animation*>(animationKeyName, ani));
	}
}

void keyAniManager::start(string type, string animationKeyName)
{
	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		iterAnimation iter = typeIter->second.find(animationKeyName);
		if (iter != typeIter->second.end())
			iter->second->start();
	}

}

void keyAniManager::stop(string type, string animationKeyName)
{
	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		iterAnimation iter = typeIter->second.find(animationKeyName);
		if (iter != typeIter->second.end())
			iter->second->stop();
	}
}

void keyAniManager::pause(string type, string animationKeyName)
{
	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		iterAnimation iter = typeIter->second.find(animationKeyName);
		if (iter != typeIter->second.end())
			iter->second->pause();
	}
}

void keyAniManager::resume(string type, string animationKeyName)
{
	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		iterAnimation iter = typeIter->second.find(animationKeyName);
		if (iter != typeIter->second.end())
			iter->second->resume();
	}
}

animation* keyAniManager::findAnimation(string type, string animationKeyName)
{
	iterAnimationType typeIter = _mTotalAnimation.find(type);
	if (typeIter != _mTotalAnimation.end())
	{
		iterAnimation iter = typeIter->second.find(animationKeyName);
		if (iter != typeIter->second.end())
			return iter->second;
	}
	return nullptr;
}

void keyAniManager::deleteAll()
{
	for (iterAnimationType typeIter = _mTotalAnimation.begin(); typeIter != _mTotalAnimation.end();)
	{
		for (iterAnimation iter = typeIter->second.begin(); iter != typeIter->second.end();)
		{
			if (iter->second != NULL)
			{
				iter->second->release();
				SAFE_DELETE(iter->second);
				iter = typeIter->second.erase(iter);
			}
			else ++iter;
		}
		typeIter->second.clear();
		typeIter = _mTotalAnimation.erase(typeIter);
	}
	_mTotalAnimation.clear();
}
