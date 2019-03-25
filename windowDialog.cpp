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
	_background->init(WINSIZEX * 0.1f, WINSIZEY * 0.75f, WINSIZEX * 0.8f, WINSIZEY * 0.20f, IMAGEMANAGER->findImage("backboard"));
	
	_text = new uiText;
	_text->init(100.f, 50.f);
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
