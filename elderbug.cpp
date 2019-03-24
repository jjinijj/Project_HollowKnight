#include "stdafx.h"
#include "elderbug.h"
#include "elderbugState.h"


elderbug::elderbug()
{
}


elderbug::~elderbug()
{
}

HRESULT elderbug::init()
{
	_subType = eNpc_Elderbug;

	_width = ELDERBUG_WIDTH;
	_colWidth = ELDERBUG_WIDTH;
	_height = ELDERBUG_HEIGHT;
	_colHeight = ELDERBUG_HEIGHT;

	return S_OK;
}

HRESULT elderbug::init(UINT uid, float x, float y)
{
	npc::init(uid, x, y);
	_subType = eNpc_Elderbug;
	_name = L"Elderbug";
	{
		image* img = IMAGEMANAGER->findImage("elderbug");
		ANIMANAGER->addArrayFrameAnimation( uid, eIdle, "elderbug"
										   ,0, 3, ELDERBUG_ANI_SPEED, true);
	}
	{
		image* img = IMAGEMANAGER->findImage("elderbug");
		ANIMANAGER->addArrayFrameAnimation( uid, eTalk, "elderbug"
										   ,6, 11, ELDERBUG_ANI_SPEED, true);
	}

	_width = ELDERBUG_WIDTH;
	_colWidth = ELDERBUG_WIDTH;
	_height = ELDERBUG_HEIGHT;
	_colHeight = ELDERBUG_HEIGHT;

	elderbugIdle* idleState = new elderbugIdle;
	idleState->init(this);

	_dir = eRIGHT;
	
	_state = idleState;
	_state->start();

	_dialogIdx = NULL;

	updateRect();

	return S_OK;
}

void elderbug::release()
{
	npc::release();
}

void elderbug::update()
{
	npc::update();
}

void elderbug::render()
{
	npc::render();
}

bool elderbug::isTalkEnd()
{
	if(_idx < _dialogMap[_dialogIdx].size())
		return false;
	else 
		return true;
}

void elderbug::talkStart()
{
	elderbugState* state = new elderbugTalk;
	state->init(this);

	if(0 != _dialogIdx)
	{
		_dialogIdx = RND->getFromIntTo(1, _dialogMap.size());
	}

	_idx = 0;

	changeState(state);
}

void elderbug::talkEnd()
{
	elderbugState* state = new elderbugIdle;
	state->init(this);

	changeState(state);
	_dialogIdx = 1;
}

wstring elderbug::getNextDialog()
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

void elderbug::initSetDialog()
{
	{
		vector<wstring> script;
		script.push_back(L"�̺��ÿ�, ������.");
		script.push_back(L"�����ڸ� ������ �� �ִ� �̴� ���ۿ� �� ���� �� ����. \n���ٽ��� ������ ����������.");
		script.push_back(L"�ٸ� �ֹε��� ��� �������.\n�� �칰�� ���� �Ʒ���, �ϳ���, �Ʒ��� �ִ� ������.");
		script.push_back(L"�츮 ���� �Ʒ����� ������ �ձ��� �־���.\n���㰡 ���� ��������, ������ �� ���� ������ ���� �̵��� ��������.");
		script.push_back(L"�ο� ������ ������, �� ����� ��� ���� ����ϴ� ��ó�� ������.\n�и� ��ŵ� �� �Ʒ��� �ִ� ���� �Ѱ� �ְ���.");
		script.push_back(L"�����Ͽ�.\n���ܿ� ���Ⱑ �װ��� ä��� �ּ�.\n����ü���� ���İ��� �����ڵ��� ����� ���ѱ���.");
		script.push_back(L"�Ƹ��� ���� �׷��� ����� ���� �ƴ�����...");

		_dialogMap.insert(make_pair(0, script));
	}

	{
		vector<wstring> script;
		script.push_back(L"��ſ��Դ� �̻��� ������ �ּ�.");
		script.push_back(L"��� ������ �ڵ��� �Ʒ��� �������� �� �ôµ�,");
		script.push_back(L"�����... ���� �ͱ�� �ѵ�, \n�и� �츮�� ���� ���� ����.");

		_dialogMap.insert(make_pair(1, script));
	}

	{
		vector<wstring> script;
		script.push_back(L"�ǰ��Ͽ�? �� ���ڰ� ��� �Ǿ� ������ ���� �� ���Ͽ�.");
		script.push_back(L"�Ʒ��� �������� ���� ������ �����ϱ⿡ �ȼ������̿�.");
		script.push_back(L"�׸��� �� ���� ���� �־ ����.\n���� ���� Ÿ���� �ƴ� �� ������.");

		_dialogMap.insert(make_pair(2, script));
	}

	{
		vector<wstring> script;
		script.push_back(L"�ű� �Ʒ��� �� ������� �����ٸ�\n�׵��� �ҽ��� ��ΰ� ���� ������ �ܷӰ� ������ ���·� �������ٰ� �˷��ֿ�.\n");
		script.push_back(L"���� �� �ҽ�����!...");
		script.push_back(L"��, ��� �� ������.");
		script.push_back(L"������ �׵��� ���ƿ����� �����ϴ� �� �ణ�� ��å����... \n�׵��� ���� ��� �ִٴ� ���� �Ͽ� ���̿�.");

		_dialogMap.insert(make_pair(3, script));
	}
}
