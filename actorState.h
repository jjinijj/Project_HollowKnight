#pragma once

class actorBase;
class actorState
{
protected:
	actorBase* _actor;
	
	animation* _ani;
	image* _img;
	actorState* _next;

	bool _isEnd;
	bool _isRight;

	UINT _state;
	UINT _aniKey;

	float _width;
	float _height;

public:
	actorState();
	~actorState();

	virtual HRESULT init(actorBase* actor);
	virtual void release();
	virtual void update();
	virtual void render();
	
	virtual void start();
	virtual void end();

	UINT getState();
	actorState* nextState();

protected:
	void setAnimaion(UINT uid, UINT aniKey);
};

