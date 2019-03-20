#pragma once
#include "singletonBase.h"


class animation;
class  animationManager : public singletonBase<animationManager>
{
private:
	typedef map<UINT, animation*>			aniMap;
	typedef map<UINT, animation*>::iterator iterM;

	typedef map<UINT, aniMap*>				totalAniMap;
	typedef map<UINT, aniMap*>::iterator		iterTM;

private:
	totalAniMap _totalAniMap;

public:
	animationManager();
	~animationManager();

	HRESULT init();
	void release();

	void addDefaultFrameAnimation(UINT uid, UINT key, const char* imageKeyName, int fps, bool reverse, bool loop);
	void addArrayFrameAnimation(UINT uid, UINT key, const char* imageKeyName, int startIdx, int endIdx, int fps, bool loop);
	void addArrayFrameAnimation(UINT uid, UINT key, const char* imageKeyName, int* arr, int arrLen, int fps, bool loop);
	void addArrayFrameAnimation(UINT uid, UINT key, const char* imageKeyName, int startIdx, int endIdx, int eventFrame, int fps, bool loop);
	void addArrayFrameAnimation(UINT uid, UINT key, const char* imageKeyName, int* arr, int arrLen, int eventFrame, int fps, bool loop);

	void start(UINT uid, UINT key);
	void stop(UINT uid, UINT key);
	void pause(UINT uid, UINT key);
	void resume(UINT uid, UINT key);

	animation* findAnimation(UINT uid, UINT key);

	void clear();
};

