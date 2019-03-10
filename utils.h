#pragma once
#include <stdarg.h>

#define PI 3.141592f
#define PI2 2 * PI


typedef struct tagELLIPSE
{
	POINTF p;
	float r;
} ELLIPSE, *LPELLIPSE;

namespace JJIN_UTIL
{
	const wstring MakeImageNamePNG(const WCHAR* imageName, const WCHAR* folderName = nullptr);
	const wstring MakeImageNameBMP(const WCHAR* imageName, const WCHAR* folderName = nullptr);

	const wstring AppendInt(const WCHAR* imageName, int value);

	POINT PointMake(int x, int y);
	POINTF PointFMake(float x, float y);

	void LineMake(HDC hdc, int startX, int startY, int endX, int endY, bool isAbsolute = true);
	void LineMake(HDC hdc, POINTF start, POINTF end, bool isAbsolute = true);


	//================= 렉트 관련 함수 ========================
	//    렉트를 만든다(레프트, 탑, 가로크기, 세로크기)
	RECT RectMake(int x, int y, int width, int height);
	//    렉트를 만든다(중점X좌표, 중점Y좌표, 가로크기, 세로크기)
	RECT RectMakeCenter(int x, int y, int width, int height);

	bool PtInRectD2D(RECTD2D& rc,  POINTF ptf);

	RECTD2D RectMake(float x, float y, float width, float height);

	//====================== 동 그 라 미 관련 ===============================
	//        원 생성함수 (hdc, left, top, 가로크기, 세로크기)
	ELLIPSE EllipseMake(float x, float y, float r);

	//라디안 각도를 디그리로
	float RadToDeg(float angle);

	//디그리 각도를 라디안으로
	float DegToRad(float angle);

	//거리구하기
	float getDistance(float x1, float y1, float x2, float y2);
	float getDistance(POINTF& p1, POINTF& p2);

	//원충돌했는지
	bool EllipseCollision(ELLIPSE& e1, ELLIPSE& e2);

	//중점구하기
	float getCenter(float a, float b);

	//렉트 중점 반환함수
	POINT getRectCenter(RECT& rc);

	//렉트 x중점 반환 함수
	float getRectXCenter(RECT& rc);

	//렉트 y중점 반환 함수
	float getRectYCenter(RECT& rc);

	//렉트 width 반환 함수
	float getRectWidth(RECT& rc);

	//렉트 height 반환 함수
	float getRectHeight(RECT& rc);

	//원 점 충돌
	bool PtInEllipse(ELLIPSE& e, POINTF p);

	//원 점 충돌
	bool PtInEllipse(ELLIPSE& e, float x, float y);

	//디그리 코사인
	float dCosf(float angle);

	//디그리 사인
	float dSinf(float angle);

	//원두개 박아서 각도받아오기 아크탄젠트 반환형 라디안
	float getAngle(POINTF p, POINTF target);

	float getAngle(float x, float y, float x2, float y2);

	//원 두개 박아서 각도받아오기 아크코사인 반환형 라디안
	float getAngle2(POINTF p, POINTF target);

	float getAngle2(float x, float y, float x2, float y2);

	//원두개 박아서 각도받아오기 아크탄젠트 반환형 디그리
	float dGetAngle(POINTF target, POINTF p);

	float dGetAngle(float x2, float y2, float x, float y);

	//원 두개 박아서 각도받아오기 아크코사인 반환형 디그리
	float dGetAngle2(POINTF target, POINTF p);

	float dGetAngle2(float x2, float y2, float x, float y);

	//원 사각형 충돌
	bool RectEllipseCollition(RECT& rc, ELLIPSE& e);

	//=========================== string wstring 변환 ===============================

	wstring string2wstring(string str);

	// ===========================
	string format(const char *fmt, ...);
	string format_arg_list(const char *fmt, va_list args);

	wstring format(const WCHAR *fmt, ...);
	wstring format_arg_list(const WCHAR *fmt, va_list args);

}