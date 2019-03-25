#pragma once
#include "singletonBase.h"
#include "baseScene.h"
#include <map>

//���� ���� ���ӳ�� ���漱��
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
	static baseScene*	_currentScene;	//���� ��
	mapSceneList		_mSceneList;

	map<eSceneName, SCENEDATA*> _fileNameMap;		// 

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

	// ���ϸ�
	string getSceneFileName(eSceneName name);

	// Ȯ��
	bool isInGameScene(eSceneName name);
};

