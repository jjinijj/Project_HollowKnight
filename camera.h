#pragma once
#include "gameNode.h"
#include "singletonBase.h"
#include <functional>

class camera : public singletonBase<camera>
{
private:
	float* _targetX;
	float* _targetY;

	float _x;
	float _y;

	int _width;
	int _height;

	float _snap;

	RECTD2D _scope;
	int _scopeWidth;
	int _scopeHeight;

	function<void(void)> _moveFunc;

public:
	camera();
	~camera();

	float getPosX() { return _x; }
	float getPosY() { return _y; }

	void setPosX(float x) { _x = x; }
	void setPosY(float y) { _y = y; }

	HRESULT init();
	HRESULT init(POINTFLOAT viewSize, RECTD2D scope, float snap);
	HRESULT init(POINTFLOAT viewSize, RECTD2D scope, float snap, float* targetX, float* targetY);
	void release();
	void update();
	void render();

	void setViewSize(float viewWidth, float viewHeight);
	void setScope(RECTD2D scope);
	void setMoveSnap(float value) { _snap = value; };

	bool isRangeInCamera(float left, float top, int width, int height);
	void getPosInCamera(float* x, float* y);

	RECTD2D getScopeRect() { return _scope; }

private:
	void fixCameroPos();

	void move();
	void moveTraceTarget();
};

