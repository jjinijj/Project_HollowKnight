#pragma once
#include "actorBase.h"


class bullet : public actorBase
{
public:
	enum eBULLET_TYPE
	{
		eLINEARBULLET,
		eARCBULLET,

		eBULLET_NONE,
		eBULLET_TYPE_COUNT = eBULLET_NONE,
	};

	enum eSTATE
	{
		eMOVE,
		ePANG,

		eNONE,
		eCOUNT = eNONE,
	};

protected:

	float _angle;
	float _speed;
	float _radius;

	POINTF _colPos;
	bool _isAppear;

public:

	HRESULT init();
	virtual HRESULT init( UINT uid
						 ,float x, float y
						 ,float angle, float speed, float radius
						 ,const char* moveimgName, const char* pangimgName);
	void release();
	void update();
	void render();

	ACTORPACK* makePack();
	void loadPack(ACTORPACK* pack);

	virtual void move();
	virtual void clear();
	void hitSomething();
	void setDisappear()			{ _isAppear = false; }

	void setColPos(POINTF pos) {_colPos = pos;}
	POINTF getColPos() {return _colPos;}

	bool isPang();
	float getRadius()			{ return _radius;}
	bool isAppear()				{ return _isAppear;}
	eBULLET_TYPE getBulletType(){ return (eBULLET_TYPE)_subType;}

	void updateCollision() override;
};

class linearBullet : public bullet
{
public:
	HRESULT init( UINT uid
				 ,float x, float y
				 ,float angle, float speed, float radius
				 ,const char* moveimgName, const char* pangimgName);
	void update();
	void move();

};

class arcBullet : public bullet
{
private:
	POINTF _startPos;
	float _time;
	float _gravity;

public:
	HRESULT init( UINT uid
				 ,float x, float y
				 ,float angle, float speed, float radius
				 ,const char* moveimgName, const char* pangimgName);
	void update();
	void move();
	void clear();
};