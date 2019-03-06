#pragma once
#include "singletonBase.h"
#include "baseScene.h"
#include <map>

//¾ÀÀ» À§ÇÑ °ÔÀÓ³ëµå Àü¹æ¼±¾ð
class gameNode;

class sceneManager : public singletonBase<sceneManager>
{
public:
	typedef map<eSceneName, baseScene*>			 mapSceneList;
	typedef map<eSceneName, baseScene*>::iterator mapSceneIter;

private:
	static baseScene*	_currentScene;	//ÇöÀç ¾À
	mapSceneList		_mSceneList;

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

	// ÇöÀç ¾À
	baseScene* getCurrentScene() { return _currentScene; }

};

