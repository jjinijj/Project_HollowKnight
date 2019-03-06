#pragma once
#include "singletonBase.h"
#include <bitset>
#include <map>

using namespace std;

#define KEYMAX 256 //Ű�� �������� ��� ���������� 256��
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

	//Ű�� �ѹ��� ������
	bool isOnceKeyDown(int key);
	//Ű�� ������ ����
	bool isOnceKeyUp(int key);
	//Ű�� ��� ������ ������
	bool isStayKeyDown(int key);
	//����̸��� ��� üũ�ǰ��ִ� ���� �ƴѰ���? ����������
	bool isToggleKey(int key);
	
	bitset<KEYMAX> getKeyUp() { return _keyUp; }
	bitset<KEYMAX> getKeyDown() { return _keyDown; }

	void setKeyDown(int key, bool state) { _keyDown.set(key, state); }
	void setKeyUp(int key, bool state) { _keyUp.set(key, state); }

	// Ű �Է� ���/����
	void setKeyHold()		{ _isHold = true; }
	void releaseKeyHold()	{ _isHold = false;}

	// Ű �Է� ��������� Ȯ��
	bool isKeyHold()		{ return _isHold;}
};

