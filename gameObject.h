#pragma once
class gameObject
{
protected:
	UID _uid;

	float _x;
	float _y;

	RECTD2D _collision;

public:
	gameObject();
	~gameObject();

	HRESULT init();
	HRESULT init(const UID uid, const float destX, const  float destY);
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	UINT getUID()			{ return _uid; }
	RECTD2D getCollision()	{ return _collision; }

	void setPosX(float x) { _x = x; }
	void setPosY(float y) { _y = y; }

	float getPosX() { return _x; }
	float getPosY() { return _y; }


};

