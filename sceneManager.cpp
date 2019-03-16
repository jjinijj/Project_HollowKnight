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
	//씬이 없다면 널값을 반환해라
	if (!scene) 
		return nullptr;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

HRESULT sceneManager::changeScene(eSceneName sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	//변경할 씬을 못찾았다
	if (find == _mSceneList.end())
		return E_FAIL;

	// 현재씬
	if (find->second == _currentScene) 
		return S_OK;

	// 이전씬의 ui 밀어주기
	UIMANAGER->release();

	//성공적으로 씬이 변경이되면 init함수를 실행
	if (SUCCEEDED(find->second->init()))
	{
		//현재 어떤 씬의 정보가 들어있을수도 있으므로 릴리즈 시켜주고
		if (_currentScene) 
			_currentScene->release();

		//바꾸려는 씬을 현재 씬으로 교체한다.
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
