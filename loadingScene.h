#pragma once


class loadingScene:	public baseScene
{
private:
	bool _iaDataAllLoad;
	image* _bg;
	image* _back;

	float _time;
	float _alpha;

public:
	loadingScene();
	~loadingScene();

	HRESULT init()		override;
	void release()		override;
	void update()		override;
	void render()		override;
	
	void startScene()	override;
	void initUI()		override;

	bool _isSoundDataLoadFin;
private:
	void loadImageData();
	void loadSoundData();
	void loadData();


};

static DWORD CALLBACK ThreadFunction(LPVOID lpParameter);


