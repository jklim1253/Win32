#pragma once

#include <Windows.h>
#include <tchar.h>
#include <memory>
#include "Button.h"
#include "type.h"
#include "utility.hpp"

class ImageButtonImpl;
class ImageButton : public Button {
public :
	ImageButton();
	virtual ~ImageButton();

public :
	HWND Create(HWND hParentWnd, Rect rc, LPCTSTR szText, LPCTSTR szImageFileName, Size cBlock);

protected :
	LRESULT OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam);
private :
	std::shared_ptr<ImageButtonImpl> impl;
};