#pragma once
#include "singletonBase.h"
#include <bitset>
#include <map>

using namespace std;

#define KEYMAX 256 //키가 눌러지는 모든 포함조건이 256임
#define TERM_TIME 0.5f

class keyManager : public singletonBase<keyManager>
{
private:
	typedef map<WPARAM, float>::iterator itWF;

private:
	bitset<KEYMAX> _keyUp;
	bitset<KEYMAX> _keyDown;


	bool _isHold;

public:
	keyManager();
	~keyManager();

	HRESULT init();
	void release();

	//키를 한번만 누르면
	bool isOnceKeyDown(int key);
	//키를 눌렀다 떼면
	bool isOnceKeyUp(int key);
	//키를 계속 누르고 있으면
	bool isStayKeyDown(int key);
	//토글이면은 계속 체크되고있는 상태 아닌가요? ㅎㅎㅎㅎㅎ
	bool isToggleKey(int key);
	
	bitset<KEYMAX> getKeyUp() { return _keyUp; }
	bitset<KEYMAX> getKeyDown() { return _keyDown; }

	void setKeyDown(int key, bool state) { _keyDown.set(key, state); }
	void setKeyUp(int key, bool state) { _keyUp.set(key, state); }

	// 키 입력 잠금/해제
	void setKeyHold()		{ _isHold = true; }
	void releaseKeyHold()	{ _isHold = false;}

	// 키 입력 잠금중인지 확인
	bool isKeyHold()		{ return _isHold;}
};

