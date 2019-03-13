#pragma once

typedef struct tagTerrainPack
{
	UID uid;
	UINT imgUid;
	UINT layer;

	float x;
	float y;

	float destX;
	float destY;

	float width;
	float height;

	int frameX;
	int framxY;

	RECTD2D rc;
	RECTD2D collider;
	WORD attr;
	
	void clear()
	{
		uid = NULL;
		imgUid = NULL;
		layer = 0;
		x = 0.f;
		y = 0.f;
		width = 0.f;
		height = 0.f;
		frameX = 0;
		framxY = 0;
		rc = {};
		collider = {};
		attr = NULL;
	}

}TARRAINPACK;



// 지형
class terrain
{
protected:
	UID		_uid;		// 고유 번호

	float	_x;			// 위치
	float	_y;			// 위치
	RECTD2D	_rc;		
	RECTD2D _collision;	// 충돌체

	WORD _attr;	// 속성

	bool _isExistCollision;	// 충돌체 사용여부

	terrain();

	HRESULT init();
	// 위치 : letf top
	HRESULT init(const UID uid, const float destX, const  float destY);

public:
	~terrain();

	virtual void release()	= 0;
	virtual void update()	= 0;
	virtual void render()	= 0;

	// 크기 변형하여 렌더 : 위치 x, y, 비율
	virtual void render(float destX, float destY, float percent) = 0;

	virtual TARRAINPACK* makePack() = 0;
	virtual void loadPack(TARRAINPACK* pack) = 0;
	
	// 충돌체 생성
	void createCollision();
	// 충돌체 제거
	void removeCollision();
	// 충돌체 변경
	void setCollision(float x, float y, float width, float height);
	// 충돌체 변경
	void setCollision(float width, float height);
	// 충돌체 변경 : 크기에 맞춤
	void setCollision();

	// 속성 추가
	void addAttribute(const UINT attr);
	// 속성 제거
	void removeAttribute(const UINT attr);
	// 속성 제거
	void clearAttribute();
	// 속성 확인
	bool checkAttribute(const UINT attr);

	UID getUID() {return _uid;}

	WORD getAtrribute()			{ return _attr; }
	RECTD2D getRect()			{ return _rc;	}
	RECTD2D getCollision()		{ return _collision;	}
}; 


//=============================================
//  드래그 지형
//	드래그를 한 이미지를 사용하는 지형
//=============================================
class terrainDrag: public terrain
{
private:
	image*		_img;
	eImageUID	_imgUid;

	float _sourX;
	float _sourY;

	float _width;			// 출력할 이미지 가로
	float _height;			// 출력할 이미지 세로

public:
	terrainDrag();
	~terrainDrag();

	// 위치 left top, 가로크기, 세로크기, 이미지
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
// 프레임 지형
// 프레임 이미지 사용하는 지형
//=============================================
class terrainFrame: public terrain
{
private:
	image*		_img;
	eImageUID	_imgUid;

	UINT _frameX;			// 이미지 프레임 x
	UINT _frameY;			// 이미지 프레임 y

public:
	terrainFrame();
	~terrainFrame();

	// 위치 left top, 프레임 x, 프레임 y, 이미지
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
// 투명 지형
// 보이지 않는 지형
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