#pragma once

#include <queue>

class mapData;
class dropObject;
class dropCoin;
class objectManager
{
public:
	enum 
	{
		DROP_COIN_MAX_COUNT = 100,
	};

private:

	int _objCnt;

	//vector<dropObject*> _objs;
	list<dropCoin*>		_coins;
	queue<dropCoin*>	_coinQ;

	mapData* _mapData;


public:
	objectManager();
	~objectManager();

	HRESULT init();
	void release();
	void update();
	void render();

	void mapDataLink(mapData* data) {_mapData = data;}

	void createGameObject(float x, float y, eObject_Type type, UINT subType);
	void hitGameObject(int uid);
	void intersectObject(int uid);
	void pushBackObject(gameObject* gObject);
	void removeGameObject(int uid);

	gameObject* findGameObject(int uid);
	list<dropCoin*> getCoinList() { return _coins; }

private:
	void intersectObjectWithObject(dropCoin* obj);

	 gameObject* createDropObject(float x, float y, eDropObject_Type type);
};

