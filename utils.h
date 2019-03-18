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

	void LineMake(HDC hdc, int startX, int startY, int endX, int endY, bool isAbsolute = true);
	void LineMake(HDC hdc, POINTF start, POINTF end, bool isAbsolute = true);


	//================= ��Ʈ ���� �Լ� ========================
	//    ��Ʈ�� �����(����Ʈ, ž, ����ũ��, ����ũ��)
	RECT RectMake(int x, int y, int width, int height);
	//    ��Ʈ�� �����(����X��ǥ, ����Y��ǥ, ����ũ��, ����ũ��)
	RECT RectMakeCenter(int x, int y, int width, int height);

	bool PtInRectD2D(RECTD2D& rc,  POINTF ptf);

	RECTD2D RectMake(float x, float y, float width, float height);
	RECTD2D RectMakeCenter(float x, float y, float width, float height);

	//====================== �� �� �� �� ���� ===============================
	//        �� �����Լ� (hdc, left, top, ����ũ��, ����ũ��)
	ELLIPSE EllipseMake(float x, float y, float r);

	//���� ������ ��׸���
	float RadToDeg(float angle);

	//��׸� ������ ��������
	float DegToRad(float angle);

	//�Ÿ����ϱ�
	float getDistance(float x1, float y1, float x2, float y2);
	float getDistance(POINTF& p1, POINTF& p2);

	//���浹�ߴ���
	bool EllipseCollision(ELLIPSE& e1, ELLIPSE& e2);

	//�������ϱ�
	float getCenter(float a, float b);

	//��Ʈ ���� ��ȯ�Լ�
	POINT getRectCenter(RECT& rc);

	//��Ʈ x���� ��ȯ �Լ�
	float getRectXCenter(RECT& rc);

	//��Ʈ y���� ��ȯ �Լ�
	float getRectYCenter(RECT& rc);

	//��Ʈ width ��ȯ �Լ�
	float getRectWidth(RECT& rc);

	//��Ʈ height ��ȯ �Լ�
	float getRectHeight(RECT& rc);

	//�� �� �浹
	bool PtInEllipse(ELLIPSE& e, POINTF p);

	//�� �� �浹
	bool PtInEllipse(ELLIPSE& e, float x, float y);

	//��׸� �ڻ���
	float dCosf(float angle);

	//��׸� ����
	float dSinf(float angle);

	//���ΰ� �ھƼ� �����޾ƿ��� ��ũź��Ʈ ��ȯ�� ����
	float getAngle(POINTF p, POINTF target);

	float getAngle(float x, float y, float x2, float y2);

	//�� �ΰ� �ھƼ� �����޾ƿ��� ��ũ�ڻ��� ��ȯ�� ����
	float getAngle2(POINTF p, POINTF target);

	float getAngle2(float x, float y, float x2, float y2);

	//���ΰ� �ھƼ� �����޾ƿ��� ��ũź��Ʈ ��ȯ�� ��׸�
	float dGetAngle(POINTF target, POINTF p);

	float dGetAngle(float x2, float y2, float x, float y);

	//�� �ΰ� �ھƼ� �����޾ƿ��� ��ũ�ڻ��� ��ȯ�� ��׸�
	float dGetAngle2(POINTF target, POINTF p);

	float dGetAngle2(float x2, float y2, float x, float y);

	//�� �簢�� �浹
	bool RectEllipseCollition(RECT& rc, ELLIPSE& e);

	//=========================== string wstring ��ȯ ===============================

	wstring string2wstring(string str);

	// ===========================
	string format(const char *fmt, ...);
	string format_arg_list(const char *fmt, va_list args);

	wstring format(const WCHAR *fmt, ...);
	wstring format_arg_list(const WCHAR *fmt, va_list args);

	WORD attribute(eAttribute attr);

	inline bool CheckIntersectRect(const RECTD2D& rc1, const RECTD2D& rc2)
	{
		int hightL	= rc1.left < rc2.left ? rc2.left : rc1.left;
		int lowR	= rc1.right < rc2.right ? rc1.right : rc2.right;
		int highT	= rc1.top < rc2.top ? rc2.top : rc1.top;
		int lowB	= rc1.bottom < rc2.bottom ? rc1.bottom : rc2.bottom;

		for (int xx = hightL; xx <= lowR; ++xx)
		{
			for (int yy = highT; yy <= lowB; ++yy)
			{
				return true;
			}
		}

		return false;
	}

	inline float GetIntersectOffsetX(const RECTD2D& moveRc, const RECTD2D& sourcRc)
	{
		float hightL = moveRc.left < sourcRc.left ? sourcRc.left : moveRc.left;
		float lowR = moveRc.right < sourcRc.right ? moveRc.right : sourcRc.right;

		if (moveRc.left < hightL)
			return hightL - moveRc.right;
		else if (lowR < moveRc.right)
			return lowR - moveRc.left;

		return 0.f;
	}

	inline float GetIntersectOffsetY(const RECTD2D& moveRc, const RECTD2D& sourcRc)
	{
		float highT = moveRc.top < sourcRc.top ? sourcRc.top : moveRc.top;
		float lowB = moveRc.bottom < sourcRc.bottom ? moveRc.bottom : sourcRc.bottom;

		if (moveRc.top < highT)
			return highT - moveRc.bottom;
		else if (lowB < moveRc.bottom)
			return lowB - moveRc.top + 1;

		return 0.f;
	}

	inline float GetIntersectOffsetX_doNotBoard(const RECTD2D& moveRc, const RECTD2D& sourcRc)
	{
		float hightL = moveRc.left < sourcRc.left ? sourcRc.left : moveRc.left;
		float lowR = moveRc.right < sourcRc.right ? moveRc.right : sourcRc.right;

		if (moveRc.left < hightL)
			return hightL - moveRc.right - 1.f;
		else if (lowR < moveRc.right)
			return lowR - moveRc.left + 1.f;

		return 0.f;
	}

	inline float GetIntersectOffsetY_doNotBoard(const RECTD2D& moveRc, const RECTD2D& sourcRc)
	{
		float highT = moveRc.top < sourcRc.top ? sourcRc.top : moveRc.top;
		float lowB = moveRc.bottom < sourcRc.bottom ? moveRc.bottom : sourcRc.bottom;

		if (moveRc.top < highT)
			return highT - moveRc.bottom - 1;
		else if (lowB < moveRc.bottom)
			return lowB - moveRc.top + 1;

		return 0.f;
	}

	inline float CheckInRange(POINTF target, POINTF sour, float range)
	{
		float distance = pow((target.x - sour.x), 2) + pow((target.y - sour.y), 2);
		float dis_ran = pow(range, 2);

		return (distance <= dis_ran);
	}

	inline POINTF MakePointF(float x, float y)
	{
		POINTF pf = { x, y };
		return pf;
	}

	inline POINTF MakePointF(int x, int y)
	{
		POINTF pf = { static_cast<float>(x), static_cast<float>(y) };
		return pf;
	}

	inline POINTF MakePointF(LONG x, LONG y)
	{
		POINTF pf = { static_cast<float>(x), static_cast<float>(y) };
		return pf;
	}

}