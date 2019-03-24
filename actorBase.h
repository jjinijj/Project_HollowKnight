#pragma once
#include "actorState.h"
#include "gameObject.h"

enum eActorType
{
	eActor_Enemy,
	eActor_Npc,
	eActor_Bullet,

	eActor_None,
	eActor_Count = eActor_None,
};

typedef struct tagActorPack
{
	UINT uid;
	UINT type;
	UINT subType;
	UINT value;

	float x;
	float y;

	void clear()
	{
		uid = NULL;
		type = NULL;
		subType = NULL;
		value = NULL;
		x = 0.f;
		y = 0.f;
	}
}ACTORPACK;

class actorBase : public gameObject
{
public:
	enum eDirection
	{
		eRIGHT,
		eLEFT,
		eUP,
		eDOWN,

		eDirection_None,
		eDirection_Count = eDirection_None,
	};

protected:
	eActorType	_type;
	UINT		_subType;
	
	eDirection _dir;
	eDirection _dirUD;

	wstring		_name;

	actorState* _state;
	actorState* _nextState;

public:
	actorBase();
	~actorBase();

	virtual HRESULT init();
	virtual HRESULT init(UINT uid, float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual ACTORPACK* makePack() = 0;
	virtual void loadPack(ACTORPACK* pack) = 0;

	void setPosition(float x, float y);

	eActorType getType() { return _type; }
	UINT getSubType()	 { return _subType; }
	eDirection getDirectionLR() { return _dir;}
	eDirection getDirectionUD() { return _dirUD;}
	wstring getName()			{ return _name; }

	float getWidth()	{ return _width; }
	float getHeight()	{ return _height;}

protected:
	void changeState(actorState* state);
	virtual void updateRect();
};