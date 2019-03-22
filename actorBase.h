#pragma once
#include "actorState.h"
#include "gameObject.h"

enum eActorType
{
	eActor_Enemy,
	eActor_Npc,

	eActor_None,
	eActor_Count = eActor_None,
};

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

	POINTF	_colSize;
	RECTD2D _collision;

public:
	actorBase();
	~actorBase();

	virtual HRESULT init(UINT uid, float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	eActorType getType() { return _type; }
	UINT getSubType()	 { return _subType; }
	eDirection getDirectionLR() { return _dir;}
	eDirection getDirectionUD() { return _dirUD;}
	wstring getName()			{ return _name; }

protected:
	void chansgeState(actorState* state);
};