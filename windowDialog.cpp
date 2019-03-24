#include "stdafx.h"
#include "windowDialog.h"
#include "uiHeaders.h"

windowDialog::windowDialog()
{
}

windowDialog::~windowDialog()
{
}

HRESULT windowDialog::init()
{

	_background = new uiPanel;
	_background->init(0, WINSIZEY / 2.f, WINSIZEX, WINSIZEY / 2.f, IMAGEMANAGER->findImage("uiBG"));
	
	_text = new uiText;
	_text->init(10.f, 10.f);
	_text->setFontColor(RGB(255, 255, 255));
	_text->setFontSize(30);

	_background->insertChild(_text);
	insertUIObject(_background);

	return S_OK;
}

void windowDialog::release()
{
	_background = nullptr;
	_text = nullptr;
}

void windowDialog::clear()
{
}

void windowDialog::update()
{
	uiBase::update();
}

void windowDialog::render()
{
	uiBase::render();
}

void windowDialog::setText(wstring str)
{
	_text->setText(str);
}

wstring windowDialog::getText()
{
	return _text->getText();
}
