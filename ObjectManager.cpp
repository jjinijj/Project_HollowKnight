#include "stdafx.h"
#include "objectManager.h"
#include "dropObject.h"
#include "mapData.h"
#include "terrain.h"

objectManager::objectManager()
: _mapData(nullptr)
{
}


objectManager::~objectManager()
{
}

HRESULT objectManager::init()
{
	_coins.clear();

	_objCnt = OBJECT_UID;

	for (int ii = 0; ii < DROP_COIN_MAX_COUNT; ++ii)
	{
		dropCoin* coin = new dropCoin;
		coin->init(_objCnt);
		_coinQ.push(coin);

		++_objCnt;
	}

	return S_OK;
}

void objectManager::release()
{
	list<dropCoin*>::iterator iter = _coins.begin();
	for(iter; _coins.end() != iter; )
	{
		dropCoin* coin = (*iter);
		iter = _coins.erase(iter);

		SAFE_RELEASE(coin);
		SAFE_DELETE(coin);
	}
	_coins.clear();

	while (!_coinQ.empty())
	{
		dropCoin* coin = _coinQ.front();
		_coinQ.pop();

		SAFE_RELEASE(coin);
		SAFE_DELETE(coin);
	}

	_mapData = nullptr;
}

void objectManager::update()
{
	list<dropCoin*>::iterator iter = _coins.begin();
	list<dropCoin*>::iterator end = _coins.end();
	
	for(iter; end != iter; ++iter)
	{
		dropCoin* coin = (*iter);
		coin->update();
		intersectObjectWithObject(coin);
	}
}

void objectManager::render()
{
	list<dropCoin*>::iterator iter = _coins.begin();
	list<dropCoin*>::iterator end = _coins.end();

	for(iter; end != iter; ++iter)
	{
		(*iter)->render();
	}
}

void objectManager::createGameObject(float x, float y, eObject_Type type, UINT subType)
{
	gameObject* obj = nullptr;

	switch (type)
	{
		case eObject_DopObject: { obj = createDropObject(x, y, (eDropObject_Type)subType); break; }
	}

	//pushback 
}

void objectManager::hitGameObject(int uid)
{
}

void objectManager::intersectObject(int uid)
{
}

void objectManager::pushBackObject(gameObject * gObject)
{
}

void objectManager::removeGameObject(int uid)
{
	// check type findGameObject(uid)

	list<dropCoin*>::iterator iter = _coins.begin();
	list<dropCoin*>::iterator end = _coins.end();

	for(iter; end != iter; ++iter)
	{
		dropCoin* coin = (*iter);
		if (coin->getUID() == uid)
		{
			_coins.erase(iter);
			
			coin->clear();
			_coinQ.push(coin);
			break;
		}
	}
}

gameObject* objectManager::findGameObject(int uid)
{
	return nullptr;
}

void objectManager::intersectObjectWithObject(dropCoin* obj)
{
	vector<terrain*> ters = *_mapData->getColliderTerrains();

	vector<terrain*>::iterator iter = ters.begin();
	vector<terrain*>::iterator end = ters.end();

	for (iter; end != iter; ++iter)
	{
		terrain* ter = (*iter);
		if (CheckIntersectRect(obj->getCollision(), ter->getCollision()))
		{
			obj->intersectWithObject(ter);
				break;
		}
	}
}

gameObject* objectManager::createDropObject(float x, float y, eDropObject_Type type)
{
	dropCoin* coin = _coinQ.front();
	_coinQ.pop();

	coin->start(x, y);
	_coins.push_back(coin);

	return coin;
}
