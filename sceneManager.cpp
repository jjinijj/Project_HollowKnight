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
	_cursorCnt = 0;
	//{
	//	SCENEDATA* data = new SCENEDATA;
	//	data->setInfo(false, false, eSceneName_Loading, "Loading");
	//	_fileNameMap.insert(make_pair(eSceneName_Loading, data));
	//}
	//{
	//	SCENEDATA* data = new SCENEDATA;
	//	data->setInfo(false, false, eSceneName_MapTool, "MapTool");
	//	_fileNameMap.insert(make_pair(eSceneName_MapTool, data));
	//}
	//{
	//	SCENEDATA* data = new SCENEDATA;
	//	data->setInfo(false, false, eSceneName_Title, "Title");
	//	_fileNameMap.insert(make_pair(eSceneName_Title, data));
	//}
	//{
	//	SCENEDATA* data = new SCENEDATA;
	//	data->setInfo(false, false, eSceneName_Load, "Load");
	//	_fileNameMap.insert(make_pair(eSceneName_Load, data));
	//}
	{
		SCENEDATA* data = new SCENEDATA;
		data->setInfo(true, false, eSceneName_DirtMouth, "dirthMouth");
		_fileNameMap.insert(make_pair(eSceneName_DirtMouth, data));
	}
	{
		SCENEDATA* data = new SCENEDATA;
		data->setInfo(true, false, eSceneName_CrossLoad_01, "crossLoad_01");
		_fileNameMap.insert(make_pair(eSceneName_CrossLoad_01, data));
	}
	{
		SCENEDATA* data = new SCENEDATA;
		data->setInfo(true, true, eSceneName_Iselda_Store, "iseldaStore");
		_fileNameMap.insert(make_pair(eSceneName_Iselda_Store, data));
	}
	{
		SCENEDATA* data = new SCENEDATA;
		data->setInfo(true, false, eSceneName_Test, "test");
		_fileNameMap.insert(make_pair(eSceneName_Test, data));
	}

	_nextSceneName = eSceneName_None;
	_isChangeScene = false;
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
	if (_isChangeScene)
	{
		_isChangeScene = false;
		changeScene(eSceneName_Load);
	}
	else
	{
		if (_currentScene) 
			_currentScene->update();
	}
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
	UIMANAGER->init();

	//���������� ���� �����̵Ǹ� init�Լ��� ����
	if (SUCCEEDED(find->second->init()))
	{
		//���� � ���� ������ ����������� �����Ƿ� ������ �����ְ�
		if (_currentScene)
			_currentScene->release();

		//�ٲٷ��� ���� ���� ������ ��ü�Ѵ�.
		_currentScene = find->second;
		
		// ui ����
		_currentScene->initUI();

		if (eSceneName_MapTool == sceneName)
		{
			if (_cursorCnt < 0)
			{
				for( ; _cursorCnt != 0; ++_cursorCnt)
					ShowCursor(true);
			}
			
			if (0 == _cursorCnt)
			{
				ShowCursor(true);
				++_cursorCnt;
			}
		}
		else
		{
			if(0 < _cursorCnt)
			{
				for(; _cursorCnt != 0; --_cursorCnt)
					ShowCursor(false);
			}

			if (0 == _cursorCnt)
			{
				ShowCursor(false);
				--_cursorCnt;
			}
		}

		return S_OK;
	}
	return E_FAIL;
}

HRESULT sceneManager::changeNextScene()
{
	return changeScene(_nextSceneName);
}

void sceneManager::setNextScene(eSceneName sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);
	if(_mSceneList.end() == find)
		return;

	if(_currentScene->getSceneName() == sceneName)
		return;

	_nextSceneName = sceneName;
	_isChangeScene = true;
}

string sceneManager::getSceneFileName(eSceneName name)
{
	string fileName;
	fileName.clear();

	if( _fileNameMap.find(name) != _fileNameMap.end() )
		fileName = _fileNameMap[name]->nameText;

	return fileName;
}

bool sceneManager::isInGameScene(eSceneName name)
{
	bool check = false;

	if (_fileNameMap.find(name) != _fileNameMap.end())
		check = _fileNameMap[name]->isInGameScene;

	return check;
}
