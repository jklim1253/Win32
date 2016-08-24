#pragma once

#include <Windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include "IControl.hpp"
#include "type.h"

enum BUTTON_STATE {
	NORMAL,
	HOVER,
	PRESSED,
	CLICKED,
	PRESSING,
	BST_MAX
};

enum BUTTON_MESSAGE {
	BM_CLICKED = BM_CLICK,
	BM_NORMAL = WM_USER + 1,
	BM_HOVER,
	BM_PRESSING,
};
struct EventResponser {
	HWND hParent;
	EventResponser(HWND _hParent = NULL) : hParent(_hParent) {}
	virtual void operator()() = 0;
};


class ButtonImpl;
class Button : public IControl<Button> {
public :
	typedef LRESULT(Button::*MessageHandler)(HWND, WPARAM, LPARAM);

	Button();
	virtual ~Button();

	HWND Create(HWND hParentWnd, Rect rc, LPCTSTR szText);

	std::wstring GetText() const;
	void SetText(const std::wstring& szText);

	BUTTON_STATE GetState() const;
	unsigned long GetId() const;

	void setResponseNormal(std::shared_ptr<EventResponser> res);
	void setResponseHover(std::shared_ptr<EventResponser> res);
	void setResponseClicked(std::shared_ptr<EventResponser> res);
	void setResponsePressing(std::shared_ptr<EventResponser> res);
protected :
	void SetMessageHandler(Button* obj, UINT uMsg, MessageHandler handler);
	LPCTSTR GetButtonClassName() const;
private :
	std::shared_ptr<ButtonImpl> impl;
};