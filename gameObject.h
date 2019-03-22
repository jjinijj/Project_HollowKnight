#pragma once
class gameObject
{
protected:
	UID _uid;

	float _x;
	float _y;

	float _width;
	float _height;
	
	float _colWidth;
	float _colHeight;

	RECTD2D _rc;
	RECTD2D _collision;

public:
	gameObject();
	~gameObject();

	virtual HRESULT init();
	virtual HRESULT init(const UID uid, const float destX, const  float destY);
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	UINT getUID()			{ return _uid; }
	RECTD2D getCollision()	{ return _collision; }
	RECTD2D getRect()		{ return _rc;}

	void setPosX(float x) { _x = x; }
	void setPosY(float y) { _y = y; }
	virtual void setPosition(float x, float y);

	float getPosX() { return _x; }
	float getPosY() { return _y; }
};

