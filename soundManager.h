#pragma once
#include "singletonBase.h"
#include <unordered_map>

#include "inc/fmod.hpp"	//fmod.hpp파일 인클루드
#pragma comment(lib, "lib/fmodex64_vc.lib")	//라이브러리 파일 등록

using namespace FMOD;

#define SOUNDBUFFER 100
#define EXTRACHANNELBUFFER 50

#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

class soundManager: public singletonBase<soundManager>
{
private:
	typedef unordered_map<string, Sound**> arrSounds;
	typedef unordered_map<string, Sound**>::iterator arrSoundsIter;
	typedef unordered_map<string, Channel**> arrChannels;
	typedef unordered_map<string, Channel**>::iterator arrChannelsIter;

private:
	System* _system;
	Sound** _sound;
	Channel** _channel;

	arrSounds _mTotalSounds;

	ChannelGroup* _bgmGroup;
	ChannelGroup* _effectGroup;

	SoundGroup* _bgmSoundGroup;
	SoundGroup* _effectSoundGroup;

public:
	HRESULT init();
	void release();
	void update();

	void addSound(string keyName, string soundName, bool bgm, bool loop);
	void play(string keyName, float volume = 1.0f); // 0.0 ~ 1.0f -> 0 ~ 255
	void stop(string keyName);
	void pause(string keyName);
	void resume(string keyName);

	bool isPlaySound(string keyName);
	bool isPauseSound(string keyName);

	// effect voluem control
	void setEffectVolume(float volume);
	// bgm volume control
	void setBgmVolume(float volume);
	// 모든 sound volume control
	void setAllSoundVolume(float volume);

	// pause
	void setEffectPause();
	void setBgmPause();
	void setAllSoundPause();

	// resume
	void setEffectResume();
	void setBgmResume();
	void setAllSoundResume();


	soundManager();
	~soundManager();
};

