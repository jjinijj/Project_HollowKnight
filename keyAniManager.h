#pragma once
#include "singletonBase.h"
#include <map>

class image;
class animation;

class keyAniManager : public singletonBase<keyAniManager>
{
private:
	typedef map<string, animation*>				arrAnimation;
	typedef map<string, animation*>::iterator	iterAnimation;


	typedef map<string, arrAnimation>			arrAnimationType;
	typedef map<string, arrAnimation>::iterator iterAnimationType;



private:
	arrAnimationType _mTotalAnimation;
public:
	keyAniManager();
	~keyAniManager();

	HRESULT init();
	void release();
	void update(string type);
	void render();

	void addAnimationType(string type);

	//디폴트 애니메이션
	void addDefaultFrameAnimation(string type, string animationKeyName, const char* imageKeyName, int fps, bool reverse, bool loop);
	void addDefaultFrameAnimation(string type, string animationKeyName, const char* imageKeyName, int fps, bool reverse, bool loop, void* cbFunction);
	void addDefaultFrameAnimation(string type, string animationKeyName, const char* imageKeyName, int fps, bool reverse, bool loop, void* cbFunction, void* obj);

	//배열 애니메이션
	void addArrayFrameAnimation(string type, string animationKeyName, const char* imageKeyName, int* arr, int arrLen, int fps, bool loop);
	void addArrayFrameAnimation(string type, string animationKeyName, const char* imageKeyName, int* arr, int arrLen, int fps, bool loop, void* cbFunction);
	void addArrayFrameAnimation(string type, string animationKeyName, const char* imageKeyName, int* arr, int arrLen, int fps, bool loop, void* cbFunction, void* obj);

	//구간 애니메이션
	void addCoordinateFrameAnimation(string type, string animationKeyName, const char* imageKeyName, int start, int end, int fps, bool reverse, bool loop);
	void addCoordinateFrameAnimation(string type, string animationKeyName, const char* imageKeyName, int start, int end, int fps, bool reverse, bool loop, void* cbFunction);
	void addCoordinateFrameAnimation(string type, string animationKeyName, const char* imageKeyName, int start, int end, int fps, bool reverse, bool loop, void* cbFunction, void* obj);

	void start(string type, string animationKeyName);
	void stop(string type, string animationKeyName);
	void pause(string type, string animationKeyName);
	void resume(string type, string animationKeyName);

	animation* findAnimation(string type, string animationKeyName);

	void deleteAll();

};

