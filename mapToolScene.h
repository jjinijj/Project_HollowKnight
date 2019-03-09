#pragma once
#include "baseScene.h"





class mapTool;
class uiButton;
class mapToolScene:	public baseScene
{
private:
	mapTool* _tool;
	



	image* _uiBG[5];

	bool _isOpenSampleBoard;
	bool _isCloseSampleBoard;
	bool _isMoveMiniScope;
	
	RECTD2D _fileZone;
	RECTD2D _hierarchy;
	RECTD2D _inspector;


	float _miniScopeWidth;
	float _miniScopeHeight;
	

	const float _sampleBoardOpenX;
	const float _sampleBoardCloseX;

	const float _sampleBoardSpeed;


public:
	mapToolScene();
	~mapToolScene();

	HRESULT init() override;
	void release() override;
	void update()  override;
	void render()  override;

private:
	void closingSampleBoard();
	void openningSampleBoard(bool isSkip = false);

	void openSampleBoard();
	void closeSampleBoard();
};

