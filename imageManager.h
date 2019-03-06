#pragma once

#include "singletonBase.h"
#include "image.h"
#include <map> //사실 안해도 되는데 선언해봄 ㅋ _ㅋ

class imageManager : public singletonBase<imageManager>
{
private:
	//map<string, image*> <-- 얘를          mapImageList라 부르겠다.
	typedef map<string, image*>				mapImageList;
	typedef map<string, image*>::iterator	mapImageIter;

private:
	mapImageList _mImageList;


public:
	imageManager();
	~imageManager();


	HRESULT init(void);
	void release(void);


	
	//===================================================================
	//							이미지 추가
	//===================================================================
	
	//일반 이미지 추가
	image* addImage(string strKey, LPCWSTR fileName, int width, int height);

	//프레임 이미지 추가
	image* addFrameImage(string strKey, LPCWSTR fileName, int width, int height, int frameNumX, int frameNumY);




	//이미지 찾는 함수
	image* findImage(string strKey);

	BOOL deleteImage(string strKey);

	BOOL deleteAll(void);

	//===================================================================
	//				렌더 함수 - Left Top 기준으로 렌더
	//===================================================================

	//기본 출력
	void render(string strKey, float alpha = 1.0f);

	//원래 크기 - 키값, 그려줄 x좌표, y좌표, 투명도
	void render(string strKey, float destX, float destY, float alpha = 1.0f);

	//변형 크기 - 키값, 그려줄 x좌표, y좌표, 출력할 너비, 출력할 높이ㅡ 투명도
	void render(string strKey, float destX, float destY, int showWidth, int showHeight, float alpha = 1.0f);

	//원래 크기로 잘라서 가져오기 - 키값, 그려줄 x좌표, y좌표 -> 가져올 곳의 left, top, 가로 크기, 세로 크기, 투명도
	void render(string strKey, float destX, float destY, float sourX, float sourY,
		int sourWidth, int sourHeight, float alpha = 1.0f);

	//변형 크기로 잘라서 가져오기 - 키값, 그려줄 x좌표, y좌표, 출력할 너비, 출력할 높이 -> 가져올 곳의 left, top, 가로 크기, 세로 크기, 투명도
	void render(string strKey, float destX, float destY, int showWidth, int showHeight, float sourX, float sourY,
		int sourWidth, int sourHeight, float alpha = 1.0f);

	//루프렌더
	void loopRender(string strKey, D2D1_RECT_F drawArea, int offSetX, int offSetY, float opacity = 1.0f);

	//원래 크기 프레임렌더 - 키값, 그려줄 x, y 좌표, 가로 방향 프레임 인덱스, 세로 방향 프레임 인덱스, 투명도
	void frameRender(string strKey, float destX, float destY, int currentFrameX, int currentFrameY, float alpha = 1.0f);

	//변형 크기 프레임렌더 - 키값, 그려줄 x, y 좌표, 출력할 너비, 출력할 높이, 가로 방향 프레임 인덱스, 세로 방향 프레임 인덱스, 투명도
	void frameRender(string strKey, float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, float alpha = 1.0f);

	//좌우반전
	void frameRenderReverseX(string strKey, float destX, float destY, int currentFrameX, int currentFrameY, float alpha = 1.0f);
	void frameRenderReverseX(string strKey, float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, float alpha = 1.0f);
};

