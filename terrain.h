#pragma once

typedef struct tagTerrainPack
{
	bool isReverse;
	
	WORD attr;
	
	UID uid;
	UINT imgUid;
	UINT layer;
	
	eTerrainType type;

	int frameX;
	int framxY;

	float x;
	float y;

	float width;
	float height;

	RECTD2D rc;
	RECTD2D collider;
	
	void clear()
	{
		isReverse = false;
		attr = NULL;
		uid = NULL;
		imgUid = NULL;
		layer = 0;
		type = eTarrain_None;
		frameX = 0;
		framxY = 0;
		x = 0.f;
		y = 0.f;
		width = 0.f;
		height = 0.f;
		rc = {};
		collider = {};
	}

}TARRAINPACK;



// ����
class terrain
{
protected:
	UID		_uid;		// ���� ��ȣ

	eTerrainType _type;

	float	_x;			// ��ġ
	float	_y;			// ��ġ
	RECTD2D	_rc;		
	RECTD2D _collision;	// �浹ü

	bool _isReverse;		// ����

	WORD _attr;	// �Ӽ�

	bool _isExistCollision;	// �浹ü ��뿩��

	terrain();

	HRESULT init();
	// ��ġ : letf top
	HRESULT init(const UID uid, const float destX, const  float destY);

public:
	~terrain();

	virtual void release()	= 0;
	virtual void update()	= 0;
	virtual void render()	= 0;

	// ũ�� �����Ͽ� ���� : ��ġ x, y, ����
	virtual void render(float destX, float destY, float percent) = 0;

	virtual TARRAINPACK* makePack() = 0;
	virtual void loadPack(TARRAINPACK* pack) = 0;
	
	// �浹ü ����
	void createCollision();
	// �浹ü ����
	void removeCollision();
	// �浹ü ����
	void setCollision(float x, float y, float width, float height);
	// �浹ü ����
	void setCollision(float width, float height);
	// �浹ü ���� : ũ�⿡ ����
	void setCollision();

	// �Ӽ� �߰�
	void addAttribute(const eAttribute attr);
	// �Ӽ� ����
	void removeAttribute(const eAttribute attr);
	// �Ӽ� ����
	void clearAttribute();
	// �Ӽ� Ȯ��
	bool checkAttribute(const eAttribute attr);

	UID getUID() {return _uid;}

	WORD getAtrribute()			{ return _attr; }
	RECTD2D getRect()			{ return _rc;	}
	RECTD2D getCollision()		{ return _collision;	}
	eTerrainType getType()		{ return _type; }
}; 


//=============================================
//  �巡�� ����
//	�巡�׸� �� �̹����� ����ϴ� ����
//=============================================
class terrainDrag: public terrain
{
private:
	image*		_img;
	eImageUID	_imgUid;

	float _sourX;
	float _sourY;

	float _width;			// ����� �̹��� ����
	float _height;			// ����� �̹��� ����

public:
	terrainDrag();
	~terrainDrag();

	// ��ġ left top, ����ũ��, ����ũ��, �̹���
	HRESULT init(const UID uid
				 , float destX, float destY
				 , float sourX, float sourY
				 , float width, float height, eImageUID imgUid);

	void release()	override;
	void update()	override;
	void render()	override;
	void render(float destX, float destY, float percent) override;
	
	TARRAINPACK* makePack() override;
	void loadPack(TARRAINPACK* pack) override;
};


//=============================================
// ������ ����
// ������ �̹��� ����ϴ� ���� : ������Ʈ, npc
//=============================================
class terrainFrame: public terrain
{
private:
	image*		_img;
	eImageUID	_imgUid;

	UINT _frameX;			// �̹��� ������ x
	UINT _frameY;			// �̹��� ������ y

public:
	terrainFrame();
	~terrainFrame();

	// ��ġ left top, ������ x, ������ y, �̹���
	HRESULT init(const UID uid
				 , float destX, float destY
				 , UINT frameX, UINT frameY, eImageUID imgUid);

	void release()	override;
	void update()	override;
	void render()	override;
	void render(float destX, float destY, float percent) override;

	TARRAINPACK* makePack() override;
	void loadPack(TARRAINPACK* pack) override;
};


//=============================================
// ���� ����
// ������ �ʴ� ����
//=============================================
class terrainClear: public terrain
{
private:
	float _width;
	float _height;

public:
	terrainClear();
	~terrainClear();

	HRESULT init(const UID uid, float destX, float destY, float width, float height);

	void release()	override;
	void update()	override;
	void render()	override;
	void render(float destX, float destY, float percent) override;

	TARRAINPACK* makePack() override;
	void loadPack(TARRAINPACK* pack) override;
};