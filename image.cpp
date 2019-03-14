#include "stdafx.h"
#include "image.h"

//컴파일러 지시기 (나 라이브러리 쓸꺼야 msImg32 라이브러리)
#pragma comment(lib, "msimg32.lib")

image::image()
	: _imageInfo(NULL), _fileName(NULL)
{
}


image::~image()
{
}

HRESULT image::init(LPCWSTR fileName, int width, int height)
{
	return init(fileName, width, height, 1, 1);		//프레임 이닛으로 1 x 1 사이즈로하면 프레임이 안짤림ㅋ 즉 1장의 사진과 같다
}

HRESULT image::init(LPCWSTR fileName, int width, int height, int frameNumX, int frameNumY)
{
	if (fileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	// 이미지 정보 생성
	HRESULT hr = E_FAIL;
	_imageInfo = new IMAGE_INFO;
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->maxFrameX = frameNumX - 1;
	_imageInfo->maxFrameY = frameNumY - 1;
	_imageInfo->frameWidth = width / frameNumX;
	_imageInfo->frameHeight = height / frameNumY;

	// 파일 경로 복사
	int len;
	len = lstrlenW(fileName);
	_fileName = new WCHAR[len + 1];
	lstrcpyW(_fileName, fileName);

	
	//CLSCTX_LOCAL_SERVER
	// WIC를 사용하기 위한 Factory 객체 생성
	// CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
	//	IID_PPV_ARGS(&_imageInfo->WICImagingFactory));
	//if (_imageInfo->WICImagingFactory == nullptr)
	//{
	//	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_HANDLER,
	//	IID_PPV_ARGS(&_imageInfo->WICImagingFactory));
	//}
	//
	// 디코더 생성
	hr = D2DMANAGER->WICImagingFactory->CreateDecoderFromFilename(_fileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand,
		&(_imageInfo->WICDecoder));
	assert(hr == S_OK);

	// 첫 번째 프레임을 사용할 수 있는 객체 구성
	hr = _imageInfo->WICDecoder->GetFrame(0, &_imageInfo->WICFrameDecoder);
	assert(hr == S_OK);

	// 포맷 컨버터 생성
	hr = D2DMANAGER->WICImagingFactory->CreateFormatConverter(&_imageInfo->WICFormatConverter);
	assert(hr == S_OK);

	// 비트맵으로 변환
	hr = _imageInfo->WICFormatConverter->Initialize(_imageInfo->WICFrameDecoder, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);

	// 변환된 이미지 형식을 사용하여 D2D용 비트맵 생성
	hr = D2DMANAGER->_renderTarget->CreateBitmapFromWicBitmap(_imageInfo->WICFormatConverter, NULL, &_imageInfo->bitmap);


	// 이미지가 생성되지 않았을 경우
	if (_imageInfo->bitmap == 0)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

void image::release(void)
{
	if (_imageInfo)
	{
		SAFE_DELETE(_imageInfo);
	}
}



//===================================================================
//						D2D 사용 렌더 함수
//- 카메라 이동 보정 & 범위에 들어가는 내용만 렌더
//- Left Top 기준
//===================================================================

void image::render(float alpha, bool isAbsolute)
{
	render(0, 0, _imageInfo->width, _imageInfo->height, 0.f, 0.f, _imageInfo->width, _imageInfo->height, alpha, isAbsolute);
}

void image::render(float destX, float destY, float alpha, bool isAbsolute)
{
	render(destX, destY, _imageInfo->width, _imageInfo->height, 0.f, 0.f, _imageInfo->width, _imageInfo->height, alpha, isAbsolute);
}

void image::render(float destX, float destY, int showWidth, int showHeight, float alpha, bool isAbsolute)
{
	render(destX, destY, showWidth, showHeight, 0.f, 0.f, _imageInfo->width, _imageInfo->height, alpha, isAbsolute);
}

void image::render(RECTD2D rc, float alpha, bool isAbsolute)
{
	render(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, alpha, isAbsolute);
}

void image::render(float destX, float destY, float sourX, float sourY, int sourWidth, int sourHeight, float alpha, bool isAbsolute)
{
	render(destX, destY, sourWidth, sourHeight, sourX, sourY, sourWidth, sourHeight, alpha, isAbsolute);
}

void image::render(float destX, float destY, int showWidth, int showHeight, float sourX, float sourY, int sourWidth, int sourHeight, float alpha, bool isAbsolute)
{
	POINTFLOAT pf = { destX, destY };
	if(!isAbsolute)
		pf = GetRenderPosition(destX, destY);

	if ( _imageInfo->bitmap != NULL )
	{
		if ( !IsRenderPositionInWindow(pf, showWidth, showHeight) )
			return;

		D2D1_RECT_F dxArea	= RectF(pf.x, pf.y, pf.x + showWidth, pf.y + showHeight);
		D2D1_RECT_F dxArea2 = RectF( sourX
									,sourY
									,sourX + sourWidth
									,sourY + sourHeight);
		D2DMANAGER->_renderTarget->DrawBitmap( _imageInfo->bitmap
											   ,dxArea
											   ,alpha
											   ,D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
											   ,dxArea2);
	}
}

void image::renderReverseX(float alpha, bool isAbsolute)
{
	renderReverseX(0, 0, _imageInfo->width, _imageInfo->height, 0.f, 0.f, _imageInfo->width, _imageInfo->height, alpha, isAbsolute);
}

void image::renderReverseX(float destX, float destY, float alpha, bool isAbsolute)
{
	renderReverseX(destX, destY, _imageInfo->width, _imageInfo->height, 0.f, 0.f, _imageInfo->width, _imageInfo->height, alpha, isAbsolute);
}

void image::renderReverseX(float destX, float destY, int showWidth, int showHeight, float alpha, bool isAbsolute)
{
	renderReverseX(destX, destY, showWidth, showHeight, 0.f, 0.f, _imageInfo->width, _imageInfo->height, alpha, isAbsolute);
}

void image::renderReverseX(RECTD2D rc, float alpha, bool isAbsolute)
{
	renderReverseX(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, alpha, isAbsolute);
}

void image::renderReverseX(float destX, float destY, float sourX, float sourY, int sourWidth, int sourHeight, float alpha, bool isAbsolute)
{
	renderReverseX(destX, destY, sourWidth, sourHeight, sourX, sourY, sourWidth, sourHeight, alpha, isAbsolute);
}

void image::renderReverseX(float destX, float destY, int showWidth, int showHeight, float sourX, float sourY, int sourWidth, int sourHeight, float alpha, bool isAbsolute)
{
	D2D1_SIZE_F size = {-1.f, 1.f};
	POINTF center = {};

	if (isAbsolute)
	{
		center.x = destX + showWidth / 2.f;
		center.y = destY + showHeight / 2.f;
		
		center.x += CAMERA->getPosX();
		center.y += CAMERA->getPosY();
	}
	else
	{
		center.x = destX + CAMERA->getPosX() + showWidth / 2.f;
		center.y = destY + CAMERA->getPosY() + showHeight / 2.f;
	}

	D2DMANAGER->_renderTarget->SetTransform(D2D1::Matrix3x2F::Scale(size, Point2F(center.x, center.y)));
	render(destX, destY, showWidth, showHeight, sourX, sourY, sourWidth, sourHeight, alpha, isAbsolute);
	D2DMANAGER->_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}


//===================================================================
//						프레임 이미지 렌더 함수
//- 카메라 이동 보정 & 범위에 들어가는 내용만 렌더
//- Left Top 기준
//===================================================================

void image::frameRender(float destX, float destY, int currentFrameX, int currentFrameY, float alpha)
{
	frameRender(destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight, currentFrameX, currentFrameY, alpha);
}

void image::frameRenderAngle(float destX, float destY, int currentFrameX, int currentFrameY, float angle, float alpha)
{
	D2DMANAGER->_renderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(angle, Point2F(destX + _imageInfo->frameWidth / 2, destY + _imageInfo->frameHeight / 2)));
	frameRender(destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight, currentFrameX, currentFrameY, alpha);
	D2DMANAGER->_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void image::frameRender(float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, float alpha)
{
	POINTFLOAT pf = GetRenderPosition(destX, destY);

	if (_imageInfo->bitmap != NULL)
	{
		if( !IsRenderPositionInWindow(pf, showWidth, showHeight) )
			return;

		D2D1_RECT_F dxArea = RectF(pf.x, pf.y, pf.x + showWidth, pf.y + showHeight);
		D2D1_RECT_F dxArea2 = RectF( static_cast<float>(currentFrameX * _imageInfo->frameWidth)
									,static_cast<float>(currentFrameY * _imageInfo->frameHeight)
									,static_cast<float>((currentFrameX + 1) * _imageInfo->frameWidth)
									,static_cast<float>((currentFrameY + 1) * _imageInfo->frameHeight));
		D2DMANAGER->_renderTarget->DrawBitmap(_imageInfo->bitmap
											  ,dxArea
											  ,alpha
											  ,D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
											  ,dxArea2);
	}
}

void image::frameRender(float destX, float destY, int sourX, int sourY, int sourWidth, int sourHeight, int currentFrameX, int currentFrameY, float alpha)
{
	POINTFLOAT pf = GetRenderPosition(destX, destY);

	if (_imageInfo->bitmap != NULL)
	{
		if (!IsRenderPositionInWindow( pf, sourWidth, sourHeight ))
			return;

		D2D1_RECT_F dxArea =   RectF( pf.x, pf.y, pf.x + sourWidth, pf.y + sourHeight );
		D2D1_RECT_F dxArea2 =  RectF( static_cast<float>(currentFrameX * _imageInfo->frameWidth  + sourX )
									, static_cast<float>(currentFrameY * _imageInfo->frameHeight + sourY )
									, static_cast<float>(currentFrameX * _imageInfo->frameWidth  + sourX + sourWidth )
									, static_cast<float>(currentFrameY * _imageInfo->frameHeight + sourY + sourHeight));
		D2DMANAGER->_renderTarget->DrawBitmap( _imageInfo->bitmap
											  ,dxArea
											  ,alpha
											  ,D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
											  ,dxArea2);
	}
}

void image::frameRenderReverseX(float destX, float destY, int currentFrameX, int currentFrameY, float alpha, bool isAbsolute)
{
	frameRenderReverseX(destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight, currentFrameX, currentFrameY, alpha, isAbsolute);
}

void image::frameRenderReverseX(float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, float alpha, bool isAbsolute)
{
	POINTFLOAT pf = { destX, destY };
	if(isAbsolute)
		pf = GetRenderPosition(destX, destY);

	if (_imageInfo->bitmap != NULL)
	{
		if (!IsRenderPositionInWindow(pf, showWidth, showHeight))
			return;

		D2D1_SIZE_F size;
		size.width = -1;
		size.height = 1;
		D2DMANAGER->_renderTarget->SetTransform(D2D1::Matrix3x2F::Scale(size, Point2F(destX - CAMERA->getPosX() + _imageInfo->frameWidth / 2, destY - CAMERA->getPosY() + _imageInfo->frameHeight / 2)));
		D2D1_RECT_F dxArea = RectF(pf.x, pf.y, pf.x + showWidth, pf.y + showHeight);
		D2D1_RECT_F dxArea2 = RectF( static_cast<float>(currentFrameX * _imageInfo->frameWidth)
									,static_cast<float>(currentFrameY * _imageInfo->frameHeight)
									,static_cast<float>((currentFrameX + 1) * _imageInfo->frameWidth)
									,static_cast<float>((currentFrameY + 1) * _imageInfo->frameHeight));
		D2DMANAGER->_renderTarget->DrawBitmap(_imageInfo->bitmap
											  ,dxArea
											  ,alpha
											  ,D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
											  ,dxArea2);
		D2DMANAGER->_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}
}

void image::loopRender(D2D1_RECT_F drawArea, int offSetX, int offSetY, float opacity, bool isAbsolute)
{
	if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width);
	if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);

	int sourWidth;
	int sourHeight;

	RECT rcDest;
	RECT rcSour;

	//화면에 루프이미지 그려줄 영역을 셋팅해보자
	int drawAreaX = (&drawArea)->left;				//그려줄 영역의 Left
	int drawAreaY = (&drawArea)->top;				//그려줄 영역의 top
	int drawAreaW = (&drawArea)->right - drawAreaX; //그려줄 영역의 가로크기
	int drawAreaH = (&drawArea)->bottom - drawAreaY;//그려줄 영역의 세로크기

	//세로
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		//나머지 연산을 통해서 정확한 수치를 알아온다
		rcSour.top = (y + offSetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;

		//밀려올라간 간격 계산
		sourHeight = rcSour.bottom - rcSour.top;

		//화면밖으로 이미지가 나갔다면
		if (y + sourHeight > drawAreaH)
		{
			//그만큼 보정해줘라 - 연산때문에 실제로 -빼면 위가 아니라 밑으로간다
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//화면밖으로 나간영역을 다시 밀어올린다(다시 그려준다)
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		// X축
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			//나머지 연산을 통한 정밀 보정
			rcSour.left = (x + offSetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;

			sourWidth = rcSour.right - rcSour.left;

			if (x + sourWidth > drawAreaW)
			{
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			render(	 rcDest.left, rcDest.top
					,rcSour.left, rcSour.top
					,rcSour.right - rcSour.left
					,rcSour.bottom - rcSour.top
					,isAbsolute, true);
		}
	}


}

void image::aniRender(int destX, int destY, animation* ani, bool isAbsolute)
{
	render(destX, destY, ani->getFramePos().x, ani->getFramePos().y, ani->getFrameWidth(), ani->getFrameHeight(), isAbsolute, true);
}

void image::aniRenderReverseX(int destX, int destY, animation * ani, bool isAbsolute)
{
	D2D1_SIZE_F size;
	size.width = -1;
	size.height = 1;
	//D2DMANAGER->_renderTarget->SetTransform(D2D1::Matrix3x2F::Scale(size, Point2F(destX + _imageInfo->frameWidth / 2, destY + _imageInfo->frameHeight / 2)));
	D2DMANAGER->_renderTarget->SetTransform(D2D1::Matrix3x2F::Scale(size, Point2F(destX - CAMERA->getPosX() + _imageInfo->frameWidth / 2, destY - CAMERA->getPosY() + _imageInfo->frameHeight / 2)));
	render(destX, destY, ani->getFramePos().x, ani->getFramePos().y, ani->getFrameWidth(), ani->getFrameHeight(), 1.0f, isAbsolute);
	D2DMANAGER->_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

POINTFLOAT image::GetRenderPosition(float destX, float destY)
{
	POINTFLOAT pf = { destX - (int)CAMERA->getPosX() , destY - (int)CAMERA->getPosY() };
	return pf;
}

bool image::IsRenderPositionInWindow(POINTFLOAT pf, int width, int height)
{
	bool result = IsRenderPositionInWindow(pf, static_cast<float>(width), static_cast<float>(height));
	return result;
}

bool image::IsRenderPositionInWindow(POINTFLOAT pf, float width, float height)
{
	if ( pf.x + width < 0  ) return false;
	if ( pf.y + height < 0 ) return false;
	if ( pf.x > WINSIZEX )	 return false;
	if ( pf.y > WINSIZEY )	 return false;

	return true;
}
