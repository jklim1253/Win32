#pragma once

#include <Windows.h>
#include <tchar.h>
#include <memory>
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
	virtual void operator()() = 0;
};
class ButtonImpl;
class Button : public IControl<Button> {
public :
	Button();
	virtual ~Button();

	HWND Create(HWND hParentWnd, Rect rc, LPCTSTR title);

	inline LPCTSTR GetText() const;
	inline void SetText(LPCTSTR szText);

	BUTTON_STATE GetState() const;
	unsigned long GetId() const;

	void setResponseNormal(std::shared_ptr<EventResponser> res);
	void setResponseHover(std::shared_ptr<EventResponser> res);
	void setResponseClicked(std::shared_ptr<EventResponser> res);
	void setResponsePressing(std::shared_ptr<EventResponser> res);
private :
	std::shared_ptr<ButtonImpl> impl;
};