#pragma once

class actorManager;
class mapData;
class baseScene
{
protected:

	bool _isInGameScene;
	
	eSceneName _sceneName;

	actorManager* _actorM;
	mapData* _mapData;

public:
	baseScene();
	~baseScene();

	virtual HRESULT init()  = 0;
	virtual void release()  = 0;
	virtual void update()	= 0;
	virtual void render()	= 0;

	virtual void initUI()	= 0;

	eSceneName getSceneName()	{ return _sceneName;		}
	bool isInGameScene()		{ return _isInGameScene;	}
};
