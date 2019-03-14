#pragma once
class uiObject
{
protected:
	
	UINT _uid;
	
	bool _isActive;
	bool _isCollider;
	bool _isViewing;

	float _x;
	float _y;

	float _width;
	float _height;

	uiObject* _parent;
	vector<uiObject*> _childs;
	int _childCount;

	POINTF _worldPosition;
	RECTD2D _rc;

	uiObject();
public:
	~uiObject();

	virtual HRESULT init();
	virtual HRESULT init(float x, float y, float width, float height);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void insertChild(uiObject* ui);
	virtual void removeChild(uiObject* ui);
	virtual void removeChildAll();
	uiObject* getChild(int idx);
	
	void setParent(uiObject* ui) { _parent = ui; }
	void setActive(bool active) { _isActive = active; }
	void setViewing(bool flag) {_isViewing = flag; }
	void setWorldPosition(float x, float y);
	void setLocalPosition(float x, float y);
	void setPositionX(float x) {_x = x;}
	void setPositionY(float y) {_y = y;}
	void setWidth(float width)	 { _width = width;	}
	void setHeight(float height) { _height = height;}

	bool isActive() { return _isActive; }
	bool isViewing(){ return _isViewing;}

	POINTF getWorldPosition();
	POINTF getLocalPosition();

	POINTF getSize()  { return PointFMake(_x, _y); }
	float getWidth()  { return _width;}
	float getHeight() { return _height;}

	RECTD2D getRect() {return _rc;}

	vector<uiObject*>* getChilds()	{ return &_childs; }
	int getChildCount()				{ return _childs.size();}

	uiObject* getParent() {return _parent; }

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

