#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

baseScene* sceneManager::_currentScene = nullptr;

sceneManager::sceneManager()
{
	_mSceneList.clear();
	_fileNameMap.clear();
}

sceneManager::~sceneManager()
{
}


HRESULT sceneManager::init()
{
	_currentScene = nullptr;

	_fileNameMap.insert(make_pair(eSceneName_DirtMouth, "dirthMouth"));

	return S_OK;
}

void sceneManager::release()
{
	mapSceneIter iter = _mSceneList.begin();

	for (; iter != _mSceneList.end(); )
	{
		baseScene* scene = iter->second;

		if (nullptr != scene)
		{
			if (_currentScene == scene) 
				scene->release();

			iter = _mSceneList.erase(iter);
			SAFE_DELETE(scene);
		}
		else 
			++iter;
	}

	_mSceneList.clear();
}

void sceneManager::update()
{
	if (_currentScene) 
		_currentScene->update();
}

void sceneManager::render()
{
	if (_currentScene)
		_currentScene->render();
}

baseScene* sceneManager::addScene(eSceneName sceneName, baseScene* scene)
{
	//���� ���ٸ� �ΰ��� ��ȯ�ض�
	if (!scene) 
		return nullptr;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

HRESULT sceneManager::changeScene(eSceneName sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	//������ ���� ��ã�Ҵ�
	if (find == _mSceneList.end())
		return E_FAIL;

	// �����
	if (find->second == _currentScene) 
		return S_OK;

	// �������� ui �о��ֱ�
	UIMANAGER->release();

	//���������� ���� �����̵Ǹ� init�Լ��� ����
	if (SUCCEEDED(find->second->init()))
	{
		//���� � ���� ������ ����������� �����Ƿ� ������ �����ְ�
		if (_currentScene) 
			_currentScene->release();

		//�ٲٷ��� ���� ���� ������ ��ü�Ѵ�.
		_currentScene = find->second;

		return S_OK;
	}
	return E_FAIL;
}

string sceneManager::getSceneFileName(eSceneName name)
{
	string fileName;
	fileName.clear();

	if( _fileNameMap.find(name) != _fileNameMap.end() )
		fileName = _fileNameMap[name];

	return fileName;
}
