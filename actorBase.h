#pragma once
#include "actorState.h"

enum eActorType
{
	eActor_Enemy,
	eActor_Npc,

	eActor_None,
	eActor_Count = eActor_None,
};

class actorBase
{
protected:
	UID _uid;
	eActorType	_type;
	UINT		_subType;

	actorState* _state;
	actorState* _nextState;

	float _x;
	float _y;

	POINTF	_colSize;
	RECTD2D _collision;

public:
	actorBase();
	~actorBase();

	HRESULT init(UINT uid, float x, float y);
	void release();
	void update();
	void render();

	UINT getUid() { return _uid; }
	const RECTD2D& getCollision() { return _collision; }
	float getPositionX() { return _x; }
	float getPositionY() { return _y; }
	eActorType getType() { return _type; }
	UINT getSubType()	 { return _subType; }

protected:
	void chansgeState(actorState* state);
};