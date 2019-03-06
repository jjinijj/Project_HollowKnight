#pragma once
#include "image.h"


class gameNode
{
private:

	//매니저를 이닛할껀지 여부
	bool _managerInit;

public:
	gameNode();
	virtual ~gameNode();

	//마소 전용 -> 디버그 모드에서 초기화가 잘되었는지 에러가 났는지 여부 등을
	//S_OK, E_FAIL, SUCCEDED 등으로 알랴줌.
	virtual HRESULT init();
	virtual HRESULT init(bool managerInit);
	virtual void release();
	virtual void update();
	virtual void render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	

};

