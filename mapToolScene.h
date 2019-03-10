#pragma once
#include "baseScene.h"





class mapTool;
class uiButton;
class mapToolScene:	public baseScene
{
private:
	mapTool* _tool;

	RECTD2D _fileZone;
	RECTD2D _hierarchy;
	RECTD2D _inspector;

public:
	mapToolScene();
	~mapToolScene();

	HRESULT init() override;
	void release() override;
	void update()  override;
	void render()  override;

};

