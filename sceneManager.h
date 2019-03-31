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
	static baseScene*	_currentScene;	//���� ��
	mapSceneList		_mSceneList;

	bool				_isChangeScene;
	
	int					_cursorCnt;			// Ŀ�� on/off�� ���

	eSceneName			_beforeSceneName;
	eSceneName			_nextSceneName;		// ���� ��
	baseScene*			_nextScene;

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
	HRESULT changeLoadScene();
	HRESULT changeNextScene();
	HRESULT createNextScene(mapData* m, actorManager* am);

	void setBeforeScene(eSceneName sceneName) { _beforeSceneName = sceneName; }
	void setNextScene(eSceneName sceneName);

	// �� ����
	baseScene* getCurrentScene()	{ return _currentScene;		}
	eSceneName getCurrentSceneName(){ return _currentScene->getSceneName();}
	eSceneName getBeforeSceneName()	{ return _beforeSceneName;	}
	eSceneName getNextSceneName()	{ return _nextSceneName;	}
	

	// ���ϸ�
	string getSceneFileName(eSceneName name);

	// Ȯ��
	bool isInGameScene(eSceneName name);

	bool isChangeScene() { return _isChangeScene; }

private:
	void setCursorVisible();
};

