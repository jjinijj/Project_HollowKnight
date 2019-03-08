#pragma once
#include "baseScene.h"



#define SAMPLABOARD_WIDTH 1010.f
#define UI_SPACE 10.f

class mapTool;
class uiButton;
class mapToolScene:	public baseScene
{
private:
	mapTool* _tool;
	
	uiButton* _qickOpen;
	uiButton* _beforeSample;
	uiButton* _nextSample;
	uiButton* _createCol;


	image* _uiBG[5];

	bool _isOpenSampleBoard;
	bool _isCloseSampleBoard;
	bool _isMoveMiniScope;
	
	RECTD2D _canvers;
	RECTD2D _sampleBoard;
	RECTD2D _miniMap;
	RECTD2D _miniScope;
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

