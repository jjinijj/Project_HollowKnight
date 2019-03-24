#include "stdafx.h"
#include "cornifer.h"
#include "corniferState.h"


cornifer::cornifer()
{
}

cornifer::~cornifer()
{
}

HRESULT cornifer::init()
{
	_subType = eNpc_Cornifer;
	_width = CORNIFER_WIDTH;
	_height = CORNIFER_HEIGHT;
	_colWidth = CORNIFER_WIDTH;
	_colHeight = CORNIFER_HEIGHT;

	return S_OK;
}

HRESULT cornifer::init(UINT uid, float x, float y)
{
	npc::init(uid, x, y);
	init();

	_name= L"Cornifer";
	{
		ANIMANAGER->addArrayFrameAnimation( uid, eIdle, "cornifer"
										   ,7, 13, CORNIFER_ANI_SPEED, true);
	}
	{
		ANIMANAGER->addArrayFrameAnimation( uid, eTalk_Left, "cornifer"
										   ,0, 6, CORNIFER_ANI_SPEED, true);
	}
	{
		ANIMANAGER->addArrayFrameAnimation( uid, eTalk_Right, "cornifer"
										   ,14, 20, CORNIFER_ANI_SPEED, true);
	}

	corniferState* state = new corniferIdle;
	state->init(this);
	_state = state;
	_state->start();

	_dir = eRIGHT;

	_dialogIdx = NULL;
	updateCollision();

	return S_OK;
}

void cornifer::release()
{
	npc::release();
}

void cornifer::update()
{
	npc::update();
}

void cornifer::render()
{
	npc::render();
}

bool cornifer::isTalkEnd()
{
	if(_idx < _dialogMap[_dialogIdx].size())
		return false;
	else 
		return true;
}

void cornifer::talkStart()
{
	corniferState* state = nullptr;

	if(PLAYER->getPosX() < _x)
		state = new corniferTalkLeft;
	else
		state = new corniferTalkRight;
	
	state->init(this);

	if(0 != _dialogIdx)
	{
		_dialogIdx = RND->getFromIntTo(1, _dialogMap.size());
	}

	_idx = 0;

	changeState(state);
}

void cornifer::talkEnd()
{
	corniferState* state = new corniferIdle;
	state->init(this);

	changeState(state);
	++_dialogIdx;
	if(2 < _dialogIdx)
		_dialogIdx = 2;
}

wstring cornifer::getNextDialog()
{
	wstring str;
	str.clear();

	if(!isTalkEnd())
	{
		str.append(_dialogMap[_dialogIdx][_idx]);
		++_idx;
	}

	return str;
}

void cornifer::initSetDialog()
{
	{
		vector<wstring> script;
		script.push_back(L"��? ��, �ȳ��ϼ���.");
		script.push_back(L"�� �Ƹ��ٿ� ���㸦 Ž���Ϸ� �����Գ���?\n�� �Ű澲�� ���ƿ�.");
		script.push_back(L"���� Ž���� �����ؿ�.\n ���� �Ҿ��ٰ� �ٽ� ã�� ���� �������� ū ��ſ�����.");
		script.push_back(L"���� �Ҿ��ٰ� �ٽ� ã�� ���� �������� ū ��ſ�����.\n��Ű� ���� �ſ� ���� ���� �� ���ƿ�.");
		script.push_back(L"���� ������ ���� �������̰� ���� �� ������ ���� ������ �ۼ��ϰ� �־��.");
		script.push_back(L"���ݱ��� �۾��� ���� �� �� �췡��?");

		_dialogMap.insert(make_pair(0, script));
	}

	{
		vector<wstring> script;
		script.push_back(L"��, �׷��� ���� �� �Ұ��� �� ����? �̾��ؿ�.");
		script.push_back(L" �ڽŸ��� ������ ���� ȥ�ڼ� ���� �ð��� ������ ���� ��ȭ�ϴ� ����� �ذ� ����.");
		script.push_back(L"�� �̸��� �ڴ��ۿ���.\n�׻� ������ Ž���ϴ� ���� �����߾��.");
		script.push_back(L"�� ��ȭ���ڸ��� �� ������ ���� ���ƴٳ���.\n �ҽ��� �츮 ��Ӵ�!");
		script.push_back(L"�ֱ� �Ƴ��� �Բ� ���� ������ �̻� �Ծ��.\n������ �Ŵ��� �ձ��� Ž���� ��ȸ���, �ݵ�� �;߰ڴ� �����߾��!");
		script.push_back(L"��·�� ����� Ž�迡 ������ �� �ְ� �� �ٰԿ�.\n���� ������ �ٽ� ������ �� �ſ���.");
		script.push_back(L"�����ϰ� ���� ���ؿ�!");

		_dialogMap.insert(make_pair(1, script));
	}

	{
		vector<wstring> script;
		script.push_back(L"�� ������ ���? �ִ��� ���� ä����� ���������,\n��� ������ ���� �۾��� ���� ���̿���.");
		script.push_back(L"������ �Ϻθ� ���� �ۼ��ϱ⸦ ���Ѵٸ�\n���� ���� ���� �������� �� �Ƴ��κ��� ���� �ۼ� ��ǰ�� ������ �� �־��. ");
		script.push_back(L"�츮�� ������ �����ϱ� ���� ���� ���Ը� �������.");

		_dialogMap.insert(make_pair(2, script));
	}
}
