#pragma once
#include "singletonBase.h"
#include "baseScene.h"
#include <map>

//¾ÀÀ» À§ÇÑ °ÔÀÓ³ëµå Àü¹æ¼±¾ð
class gameNode;

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

	eSceneName			_nextScene;		// ´ÙÀ½ ¾À

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
	HRESULT changeNextScene();

	void setNextScene(eSceneName sceneName);

	// ÇöÀç ¾À
	baseScene* getCurrentScene() { return _currentScene; }

	// ÆÄÀÏ¸í
	string getSceneFileName(eSceneName name);

	// È®ÀÎ
	bool isInGameScene(eSceneName name);
};

