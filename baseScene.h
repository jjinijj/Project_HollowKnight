#pragma once

enum eSceneName
{
	eSceneName_Loading,
	eSceneName_MapTool,
	eSceneName_Title,
	eSceneName_Load,
	sSceneName_DirtMouth,
	eSceneName_CrossLoad,

	eSceneName_None,
	eSceneName_Count = eSceneName_None
};

class baseScene
{
protected:
	eSceneName _sceneName;

public:
	baseScene();
	~baseScene();

	virtual HRESULT init()  = 0;
	virtual void release()  = 0;
	virtual void update()	= 0;
	virtual void render()	= 0;

	eSceneName getSceneName() { return _sceneName; }
};

