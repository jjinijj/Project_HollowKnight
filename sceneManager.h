#pragma once
#include "singletonBase.h"
#include "baseScene.h"
#include <map>

//씬을 위한 게임노드 전방선언
class gameNode;

class sceneManager : public singletonBase<sceneManager>
{
public:
	typedef map<eSceneName, baseScene*>			 mapSceneList;
	typedef map<eSceneName, baseScene*>::iterator mapSceneIter;

private:
	static baseScene*	_currentScene;	//현재 씬
	mapSceneList		_mSceneList;

	map<eSceneName, string> _fileNameMap;		// 

public:
	sceneManager();
	~sceneManager();

	HRESULT init();
	void release();
	void update();
	void render();

	//씬 추가 함쑤
	baseScene* addScene(eSceneName sceneName, baseScene* scene);

	//씬 변경 함쑤
	HRESULT changeScene(eSceneName sceneName);

	// 현재 씬
	baseScene* getCurrentScene() { return _currentScene; }

	// 파일명
	string getSceneFileName(eSceneName name);
};

