#pragma once
#include "singletonBase.h"
#include "baseScene.h"
#include <map>

class mapData;
class actorManager;

typedef struct tagSceneData
{
	bool		isInGameScene;
	bool		isEnterBuild;
	eSceneName	name;
	string		nameText;

	void setInfo(bool inIsInGameScene,  bool isEnter, eSceneName inName, string inText)
	{
		isInGameScene = inIsInGameScene;
		isEnterBuild = isEnter;
		name = inName;
		nameText = inText;
	}
}SCENEDATA;

class sceneManager : public singletonBase<sceneManager>
{
public:
	typedef map<eSceneName, baseScene*>			 mapSceneList;
	typedef map<eSceneName, baseScene*>::iterator mapSceneIter;

private:
	static baseScene*	_currentScene;	//ÇöÀç ¾À
	mapSceneList		_mSceneList;

	bool				_isChangeScene;
	
	int					_cursorCnt;			// Ä¿¼­ on/off¿¡ »ç¿ë

	eSceneName			_beforeSceneName;
	eSceneName			_nextSceneName;		// ´ÙÀ½ ¾À
	baseScene*			_nextScene;

	map<eSceneName, SCENEDATA*> _fileNameMap;		// 

public:
	sceneManager();
	~sceneManager();

	HRESULT init();
	void release();
	void update();
	void render();

	//¾À Ãß°¡ ÇÔ¾¥
	baseScene* addScene(eSceneName sceneName, baseScene* scene);

	//¾À º¯°æ ÇÔ¾¥
	HRESULT changeScene(eSceneName sceneName);
	HRESULT changeLoadScene();
	HRESULT changeNextScene();
	HRESULT createNextScene(mapData* m, actorManager* am);

	void setBeforeScene(eSceneName sceneName) { _beforeSceneName = sceneName; }
	void setNextScene(eSceneName sceneName);

	// ¾À Á¤º¸
	baseScene* getCurrentScene()	{ return _currentScene;		}
	eSceneName getCurrentSceneName(){ return _currentScene->getSceneName();}
	eSceneName getBeforeSceneName()	{ return _beforeSceneName;	}
	eSceneName getNextSceneName()	{ return _nextSceneName;	}
	

	// ÆÄÀÏ¸í
	string getSceneFileName(eSceneName name);

	// È®ÀÎ
	bool isInGameScene(eSceneName name);

	bool isChangeScene() { return _isChangeScene; }

private:
	void setCursorVisible();
};

