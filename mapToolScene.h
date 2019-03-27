#pragma once
#include "baseScene.h"





class mapTool;
class uiButton;
class mapToolScene:	public baseScene
{
private:

public:
	mapToolScene();
	~mapToolScene();

	HRESULT init()	override;
	void release()	override;
	void update()	override;
	void render()	override;

	void initUI()	override;
};

