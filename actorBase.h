#pragma once
class actorBase
{
protected:
	UID _uid;

	image*		_img;
	animation*	_ani;

public:
	actorBase();
	~actorBase();

	HRESULT init();
	void release();
	void update();
	void render();

	void changeState();
};

