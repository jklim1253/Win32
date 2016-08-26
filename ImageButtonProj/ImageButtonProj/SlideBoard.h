#pragma once

#include <Windows.h>
#include <tchar.h>
#include <memory>
#include "IControl.hpp"
#include "ImageButton.h"

class SlideBoardImpl;
class SlideBoard : public IControl<SlideBoard> {

public :
	SlideBoard();
	~SlideBoard();

public :
	HWND Create(HWND hParentWnd, const RECT& rc, const SIZE& cBlock);

	void AddItem(LPCTSTR title, std::shared_ptr<EventResponser> op);
	size_t length() const;
protected :

private :
	SlideBoardImpl* impl;
};