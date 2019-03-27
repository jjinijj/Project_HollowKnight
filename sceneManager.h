#pragma once
#include "singletonBase.h"
#include "baseScene.h"
#include <map>

//æ¿¿ª ¿ß«— ∞‘¿”≥ÎµÂ ¿¸πÊº±æ
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
	static baseScene*	_currentScene;	//«ˆ¿Á æ¿
	mapSceneList		_mSceneList;

	bool				_isChangeScene;

	eSceneName			_nextSceneName;		// ¥Ÿ¿Ω æ¿
	baseScene*			_nextScene;

	map<eSceneName, SCENEDATA*> _fileNameMap;		// 

public:
	sceneManager();
	~sceneManager();

	HRESULT init();
	void release();
	void update();
	void render();

	//æ¿ √ﬂ∞° «‘æ•
	baseScene* addScene(eSceneName sceneName, baseScene* scene);

	//æ¿ ∫Ø∞Ê «‘æ•
	HRESULT changeScene(eSceneName sceneName);
	HRESULT changeNextScene();

	void setNextScene(eSceneName sceneName);

	// æ¿ ¡§∫∏
	baseScene* getCurrentScene()	{ return _currentScene; }
	eSceneName getCurrentSceneName(){ return _currentScene->getSceneName();}
	eSceneName getNextSceneName()	{ return _nextSceneName;}
	

	// ∆ƒ¿œ∏Ì
	string getSceneFileName(eSceneName name);

	// »Æ¿Œ
	bool isInGameScene(eSceneName name);
};

