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
		script.push_back(L"이보시오, 여행자.");
		script.push_back(L"여행자를 맞이할 수 있는 이는 나밖에 안 남은 것 같소. \n보다시피 마을이 조용해졌소.");
		script.push_back(L"다른 주민들은 모두 사라졌소.\n저 우물을 통해 아래로, 하나씩, 아래에 있는 동굴로.");
		script.push_back(L"우리 마을 아래에는 위대한 왕국이 있었소.\n폐허가 된지 오래지만, 아직도 그 깊은 곳으로 많은 이들을 유인하지.");
		script.push_back(L"부와 영광과 깨달음, 저 어둠이 모든 것을 약속하는 것처럼 보이지.\n분명 당신도 저 아래에 있는 꿈을 쫓고 있겠지.");
		script.push_back(L"조심하오.\n역겨운 공기가 그곳을 채우고 있소.\n생명체들은 미쳐가고 여행자들은 기억을 빼앗기지.");
		script.push_back(L"아마도 꿈은 그렇게 대단한 것이 아닐지도...");

		_dialogMap.insert(make_pair(0, script));
	}

	{
		vector<wstring> script;
		script.push_back(L"당신에게는 이상한 뭔가가 있소.");
		script.push_back(L"모든 유형의 자들이 아래로 지나가는 걸 봤는데,");
		script.push_back(L"당신은... 낯이 익기는 한데, \n분명 우리가 만난 적은 없소.");

		_dialogMap.insert(make_pair(1, script));
	}

	{
		vector<wstring> script;
		script.push_back(L"피곤하오? 저 의자가 쇠로 되어 있을지 몰라도 꽤 편하오.");
		script.push_back(L"아래로 내려가기 전에 생각을 정리하기에 안성맞춤이오.");
		script.push_back(L"그리고 난 누가 옆에 있어서 좋고.\n말이 많은 타입은 아닌 것 같지만.");

		_dialogMap.insert(make_pair(2, script));
	}

	{
		vector<wstring> script;
		script.push_back(L"거기 아래서 내 동료들을 만난다면\n그들의 불쌍한 장로가 여기 위에서 외롭게 버려진 상태로 남겨졌다고 알려주오.\n");
		script.push_back(L"나도 참 불쌍하지!...");
		script.push_back(L"아, 사실 난 괜찮소.");
		script.push_back(L"하지만 그들을 돌아오도록 설득하는 데 약간의 죄책감이... \n그들이 아직 살아 있다는 가정 하에 말이요.");

		_dialogMap.insert(make_pair(3, script));
	}
}
