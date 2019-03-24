#pragma once


class loadingScene:	public baseScene
{
private:
	bool _iaDataAllLoad;

public:
	loadingScene();
	~loadingScene();

	HRESULT init() override;
	void release() override;
	void update()  override;
	void render()  override;

	bool _isSoundDataLoadFin;
private:
	void loadImageData();
	void loadSoundData();
	void loadData();


};

static DWORD CALLBACK ThreadFunction(LPVOID lpParameter);


