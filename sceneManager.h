#pragma once
#include "singletonBase.h"
#include "baseScene.h"
#include <map>

//���� ���� ���ӳ�� ���漱��
class gameNode;

class sceneManager : public singletonBase<sceneManager>
{
public:
	typedef map<eSceneName, baseScene*>			 mapSceneList;
	typedef map<eSceneName, baseScene*>::iterator mapSceneIter;

private:
	static baseScene*	_currentScene;	//���� ��
	mapSceneList		_mSceneList;

public:
	sceneManager();
	~sceneManager();

	HRESULT init();
	void release();
	void update();
	void render();

	//�� �߰� �Ծ�
	baseScene* addScene(eSceneName sceneName, baseScene* scene);

	//�� ���� �Ծ�
	HRESULT changeScene(eSceneName sceneName);

	// ���� ��
	baseScene* getCurrentScene() { return _currentScene; }

};

