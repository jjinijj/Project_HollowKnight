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
		script.push_back(L"흠? 아, 안녕하세요.");
		script.push_back(L"이 아름다운 폐허를 탐험하러 내려왔나요?\n난 신경쓰지 말아요.");
		script.push_back(L"나도 탐험을 좋아해요.\n 길을 잃었다가 다시 찾는 것은 무엇보다 큰 즐거움이죠.");
		script.push_back(L"길을 잃었다가 다시 찾는 것은 무엇보다 큰 즐거움이죠.\n당신과 나는 매우 운이 좋은 것 같아요.");
		script.push_back(L"나는 직업이 지도 제작자이고 지금 이 지역에 대한 지도를 작성하고 있어요.");
		script.push_back(L"지금까지 작업한 것을 한 부 살래요?");

		_dialogMap.insert(make_pair(0, script));
	}

	{
		vector<wstring> script;
		script.push_back(L"오, 그런데 아직 내 소개를 안 했죠? 미안해요.");
		script.push_back(L" 자신만의 생각에 빠져 혼자서 오랜 시간을 보내다 보면 대화하는 방법도 잊게 되죠.");
		script.push_back(L"내 이름은 코니퍼에요.\n항상 세상을 탐험하는 것을 좋아했어요.");
		script.push_back(L"난 부화하자마자 내 가족을 떠나 돌아다녔어요.\n 불쌍한 우리 어머니!");
		script.push_back(L"최근 아내와 함께 흙의 마을로 이사 왔어요.\n끝없는 거대한 왕국을 탐험할 기회라니, 반드시 와야겠다 생각했어요!");
		script.push_back(L"어쨌든 당신이 탐험에 전념할 수 있게 해 줄게요.\n운이 좋으면 다시 만나게 될 거에요.");
		script.push_back(L"안전하게 여행 잘해요!");

		_dialogMap.insert(make_pair(1, script));
	}

	{
		vector<wstring> script;
		script.push_back(L"내 지도가 어때요? 최대한 많이 채우려고 노력했지만,\n모든 지도는 현재 작업이 진행 중이에요.");
		script.push_back(L"지도의 일부를 직접 작성하기를 원한다면\n저기 위에 흙의 마을에서 내 아내로부터 지도 작성 물품을 구입할 수 있어요. ");
		script.push_back(L"우리는 간신히 연명하기 위해 작은 가게를 열었어요.");

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
