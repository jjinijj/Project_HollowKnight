#pragma once
class uiObject
{
protected:
	
	UINT _uid;
	
	bool _isActive;
	bool _isCollider;

	float _x;
	float _y;

	float _width;
	float _height;

	uiObject* _parent;
	vector<uiObject*> _childs;
	int _childCount;

	RECTD2D _rc;

	uiObject();
	~uiObject();
public:

	virtual HRESULT init();
	virtual HRESULT init(float x, float y, float width, float height);
	virtual void release();
	virtual void update();
	virtual void render();

	void insertChild(uiObject* ui);
	uiObject* getChild(int idx);
	
	void setActive(bool active) { _isActive = active; }
	void setPositionX(float x) {_x = x;}
	void setPositionY(float y) {_y = y;}
	void setWidth(float width)	 { _width = width;	}
	void setHeight(float height) { _height = height;}

	bool isActive() { return _isActive; }

	POINTF getPosition() { return PointFMake(_x, _y); }
	float getPositionX() { return _x; }
	float getPositionY() { return _y; }
	
	POINTF getWorldPosition();
	POINTF getLocalPosition();

	POINTF getSize()  { return PointFMake(_x, _y); }
	float getWidth()  { return _width;}
	float getHeight() { return _height;}

	vector<uiObject*>* getChilds()	{ return &_childs; }
	int getChildCount()				{ return _childs.size();}

protected:
	virtual void inputKey();
	virtual void releaseInputKey();

	virtual void onceKeyDownMouseL();
	virtual void onceKeyUpMouseL();
	virtual void stayKeyMouseL();
	
	virtual void onceKeyDownMouseR();
	virtual void onceKeyUpMouseR();
	virtual void stayKeyMouseR();
};

