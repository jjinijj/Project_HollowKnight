#pragma once
#include "animation.h"

class image
{
public:

	typedef struct tagImageInfo
	{
		DWORD					resID;
		IWICImagingFactory*		WICImagingFactory;		//Windows Imaging Component를 위한 Factory 인터페이스
		IWICFormatConverter*	WICFormatConverter;		//Format Converter
		IWICBitmapDecoder*		WICDecoder;				//Bitmap Decoder
		IWICBitmapFrameDecode*	WICFrameDecoder;		//프레임 단위로 디코딩할 Decoder 인터페이스
		ID2D1Bitmap*			bitmap;					//D2D용 비트맵
		int						width;					//원본 이미지 폭
		int						height;					//원본 이미지 높이
		int						currentFrameX;			//현재 프레임X(번째)
		int						currentFrameY;			//현재 프레임Y(번째)
		int						maxFrameX;				//최대프레임 
		int						maxFrameY;
		int						frameWidth;				//프레임 가로 크기
		int						frameHeight;			//프레임 세로 크기
		tagImageInfo()
		{
			resID				= 0;
			WICImagingFactory	= nullptr;
			WICFormatConverter	= nullptr;
			WICDecoder			= nullptr;
			WICFrameDecoder		= nullptr;
			bitmap				= nullptr;
			width				= 0;
			height				= 0;
			currentFrameX		= 0;
			currentFrameY		= 0;
			maxFrameX			= 0;
			maxFrameY			= 0;
			frameWidth			= 0;
			frameHeight			= 0;
		}
	}IMAGE_INFO, *LPIMAGE_INFO;

private:
	LPIMAGE_INFO		_imageInfo;		//이미지 정보를 담은 구조체
	LPWSTR				_fileName;		//파일이름에 사용할 캐릭터포인터


public:
	image();
	~image();

	
	//===================================================================
	//							이미지 초기화
	//===================================================================
	
	// 일반 이미지를 파일에서 가져올 때, 파일명, 원본 너비, 원본 높이
	HRESULT init(LPCWSTR fileName, int width, int height);

	// 프레임 이미지를 파일에서 가져올 때, 파일명, 원본 너비, 원본 높이, 가로 프레임 수, 세로 프레임 수
	HRESULT init(LPCWSTR fileName, int width, int height, int frameNumX, int frameNumY);

	// 이미지 릴리즈
	void release(void);


	
	//===================================================================
	//						D2D 사용 렌더 함수
	//- 카메라 이동 보정 & 범위에 들어가는 내용만 렌더
	//- Left Top 기준
	//===================================================================
	

	//기본 출력
	void render(float alpha, bool isAbsolute = true);

	//원래 크기 출력 - 그려줄 x좌표, y좌표, 투명도, 절대값 좌표로 출력 여부
	void render(float destX, float destY, float alpha, bool isAbsolute = true);

	//변형 크기 출력 - 그려줄 x좌표, y좌표, 출력할 너비, 출력할 높이, 투명도, 절대값 좌표로 출력 여부
	void render(float destX, float destY, int showWidth, int showHeight, float alpha, bool isAbsolute = true);

	void render(RECTD2D rc, float alpha, bool isAbsolute = true);

	//원래 크기로 잘라서 가져오기 - 그려줄 x좌표, y좌표 -> 가져올 곳의 left, top, 가로 크기, 세로 크기, 투명도, 절대값 좌표로 출력 여부
	void render(float destX, float destY, float sourX, float sourY,
		int sourWidth, int sourHeight, float alpha, bool isAbsolute = true);

	//변형 크기로 잘라서 가져오기 - 그려줄 x좌표, y좌표, 출력할 너비, 출력할 높이 -> 가져올 곳의 left, top, 가로 크기, 세로 크기, 투명도, 절대값 좌표로 출력 여부
	void render(float destX, float destY, int showWidth, int showHeight, float sourX, float sourY,
		int sourWidth, int sourHeight, float alpha, bool isAbsolute = true);


	//원래 크기 프레임렌더 - 그려줄 x, y 좌표, 가로 방향 프레임 인덱스, 세로 방향 프레임 인덱스, 투명도, 절대값 좌표로 출력 여부
	void frameRender(float destX, float destY, int currentFrameX, int currentFrameY, float alpha = 1.0f);


	void frameRenderAngle(float destX, float destY, int currentFrameX, int currentFrameY, float angle, float alpha = 1.0f);

	//변형 크기 프레임렌더 - 그려줄 x, y 좌표, 출력할 너비, 출력할 높이, 가로 방향 프레임 인덱스, 세로 방향 프레임 인덱스, 투명도, 절대값 좌표로 출력 여부
	void frameRender(float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, float alpha = 1.0f);

	//원래 크기로 잘라서 가져오기 프레임렌더 - 그려줄 x, y 좌표, 출력할 너비, 출력할 높이, 가로 방향 프레임 인덱스, 세로 방향 프레임 인덱스, 투명도, 절대값 좌표로 출력 여부
	void frameRender(float destX, float destY, int sourX, int sourY, int sourWidth, int sourHeight, int currentFrameX, int currentFrameY, float alpha = 1.0f);


	//좌우반전
	void frameRenderReverseX(float destX, float destY, int currentFrameX, int currentFrameY, float alpha = 1.0f, bool isAbsolute = true);
	void frameRenderReverseX(float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, float alpha = 1.0f, bool isAbsolute = true);

	//루프 렌더 - 수정 필요
	void loopRender(D2D1_RECT_F drawArea, int offSetX = 0, int offSetY = 0, float opacity = 1.0f, bool isAbsolute = true);

	void aniRender(int destX, int destY, animation* ani, bool isAbsolute = true);
	void aniRenderReverseX(int destX, int destY, animation* ani, bool isAbsolute = true);

	
	//===================================================================
	//						이미지 정보 Get, Set
	//===================================================================
	

	//가로 길이~
	inline int GetWidth(void) { return _imageInfo->width; }

	//세로 길이~
	inline int GetHeight(void) { return _imageInfo->height; }

	//프레임 이미지 정보
	inline void SetFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;
		if (frameX > _imageInfo->maxFrameX) _imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}

	inline void SetFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;
		if (frameY > _imageInfo->maxFrameY) _imageInfo->currentFrameY = _imageInfo->maxFrameY;
	}

	inline int GetMaxFrameX(void) { return _imageInfo->maxFrameX; }
	inline int GetMaxFrameY(void) { return _imageInfo->maxFrameY; }

	inline int GetFrameX(void) { return _imageInfo->currentFrameX; }
	inline int GetFrameY(void) { return _imageInfo->currentFrameY; }

	inline int GetFrameWidth(void) { return _imageInfo->frameWidth; }
	inline int GetFrameHeight(void) { return _imageInfo->frameHeight; }

	inline LPWSTR GetFileName(void) {return _fileName; }


	//===================================================================
	//						내부 함수
	//===================================================================
private:
	POINTFLOAT GetRenderPosition(float destX, float destY);
	bool IsRenderPositionInWindow(POINTFLOAT pf, int width, int height);
	bool IsRenderPositionInWindow(POINTFLOAT pf, float width, float height);
};

