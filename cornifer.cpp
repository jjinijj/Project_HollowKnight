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
	_dir = eRIGHT;

	_vSound.push_back("Cornifer_01");
	_vSound.push_back("Cornifer_02");
	_vSound.push_back("Cornifer_03");

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
	humming();
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

	string addr = "sound/";
	addr.append(_vSound[_dialogIdx]);
	addr.append(".wav");

	//SOUNDMANAGER->addSound(_vSound[_dialogIdx], addr, false, false);
	SOUNDMANAGER->play(_vSound[_dialogIdx]);
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

void cornifer::humming()
{
	float distance = pow((PLAYER->getPosX() - _x), 2) + pow((PLAYER->getPosY() - _y), 2);

	DEVTOOL->pushBackDebugText(format(L"%.2f", distance));
	if(distance < 3000000.f)
	{
		if(!SOUNDMANAGER->isPlaySound("Cornifer_Hum"))
			SOUNDMANAGER->play("Cornifer_Hum");

		if (_state->getState() == eIdle)
		{
			float volume = distance / 3000000.f;
			DEVTOOL->pushBackDebugText(format(L"%.2f", volume));

			SOUNDMANAGER->setVolume("Cornifer_Hum", 1.f - volume);
		}
		else
			SOUNDMANAGER->stop("Cornifer_Hum");
	}
	else
	{
		if(SOUNDMANAGER->isPlaySound("Cornifer_Hum"))
			SOUNDMANAGER->stop("Cornifer_Hum");
	}
}
