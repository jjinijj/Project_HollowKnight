#pragma once

class uiPanel;
class uiText;

class windowDialog : public uiBase
{
private:
	uiPanel* _background;
	uiText* _text;

public:
	windowDialog();
	~windowDialog();

	HRESULT init();
	void release();
	void clear();
	void update();
	void render();

	void setText(wstring str);
	wstring getText();
};